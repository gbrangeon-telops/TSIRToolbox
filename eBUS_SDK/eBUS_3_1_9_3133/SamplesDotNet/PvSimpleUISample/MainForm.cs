// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using PvDotNet;
using PvGUIDotNet;


namespace PvSimpleUISample
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public MainForm()
        {
            InitializeComponent();

            // Handlers used to callbacks in the UI thread
            mDisconnectedHandler += new GenericHandler(OnDisconnected);
            mAcquisitionModeChangedHandler += new GenericHandler(OnAcquisitionModeChanged);
            mAcquisitionStateChangedHandler += new GenericHandler(OnAcquisitionStateChanged);

            // Create pipeline - requires stream
            mPipeline = new PvPipeline(mStream);

            // Set browser form owners
            mCommBrowser.Owner = this;
            mDeviceBrowser.Owner = this;
            mStreamBrowser.Owner = this;

            // Create display thread, hook display event
            mDisplayThread = new PvDisplayThread();
            mDisplayThread.OnBufferDisplay += new OnBufferDisplay(OnBufferDisplay);
        }

        // Handler used to bring link disconnected event in the main UI thread
        private delegate void GenericHandler();
        private GenericHandler mDisconnectedHandler = null;

        // Handler used to bring acquisition mode changed event in the main UI thread
        private GenericHandler mAcquisitionModeChangedHandler = null;

        // Handler used to bring acquisition state manager callbacks in the main UI thread
        private GenericHandler mAcquisitionStateChangedHandler = null;

        // Main application objects: device, stream, pipeline
        private PvDevice mDevice = new PvDevice();
        private PvStream mStream = new PvStream();
        private PvPipeline mPipeline = null;

        // Acquisition state manager
        private PvAcquisitionStateManager mAcquisitionManager = null;

        // Display thread
        private PvDisplayThread mDisplayThread = null;

        // Parameter browsers
        private BrowserForm mCommBrowser = new BrowserForm();
        private BrowserForm mDeviceBrowser = new BrowserForm();
        private BrowserForm mStreamBrowser = new BrowserForm();

        /// <summary>
        /// Method that syncs the whole UI enabled state
        /// </summary>
        private void EnableInterface()
        {
            // Connect and disconnect button
            connectButton.Enabled = !mDevice.IsConnected && !mStream.IsOpened;
            disconnectButton.Enabled = mDevice.IsConnected || mStream.IsOpened;

            // Tree browser buttons
            EnableTreeBrowsers();

            // Acquisition controls
            EnableControls();
        }

        /// <summary>
        /// Connect button click handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void connectButton_Click(object sender, EventArgs e)
        {
            // create a device finder wnd and open the select device dialog
            PvDeviceFinderForm lFinder = new PvDeviceFinderForm();

            // Show device finder
            if ((lFinder.ShowDialog() != DialogResult.OK) ||
                (lFinder.Selected == null))
            {
                return;
            }

            // Display hourglass
            Cursor lOld = Cursor;
            Cursor = Cursors.WaitCursor;

            // Connect to device
            Connect(lFinder.Selected);

            // Restore cursor
            Cursor = lOld;
        }

        /// <summary>
        /// Disconnect button event handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void disconnectButton_Click(object sender, EventArgs e)
        {
            // Show hourglass
            Cursor lOld = Cursor;
            Cursor = Cursors.WaitCursor;

            // Disconnect from device
            Disconnect();

            // Restore cursor
            Cursor = lOld;
        }

        /// <summary>
        /// Enable the tree browser buttons
        /// </summary>
        private void EnableTreeBrowsers()
        {
            // Device and stream: enabled if connected
            deviceButton.Enabled = mDevice.IsConnected;
            streamButton.Enabled = mStream.IsOpened;

            // Communication button is always enabled
            communicationButton.Enabled = true;
        }

        /// <summary>
        /// Enables the acquisition controls
        /// </summary>
        private void EnableControls()
        {
            if (!mDevice.IsConnected)
            {
                // Not connected: acquisition state manager not available, directly disable all
                modeComboBox.Enabled = false;
                playButton.Enabled = false;
                stopButton.Enabled = false;
            }
            else
            {
                // Read current acquisition state
                bool lLocked = mAcquisitionManager.State == PvAcquisitionState.Unlocked;

                // Mode and play are enabled if not locked
                modeComboBox.Enabled = lLocked;
                playButton.Enabled = lLocked;

                // Stop is enabled only if locked
                stopButton.Enabled = !lLocked;
            }
        }

        /// <summary>
        /// Form load event handler. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            // Just syncs the enabled state of the UI
            EnableInterface();
        }

        /// <summary>
        /// Connects to the device from a device info
        /// </summary>
        /// <param name="aDI"></param>
        void Connect(PvDeviceInfo aDI)
        {
            // Just in case we came here still connected...
            Disconnect();

            try
            {
                // Connect to device using device info
                mDevice.Connect(aDI);

                // Open stream using device IP address
                mStream.Open(aDI.IPAddress);

                // Negotiate packet size
                mDevice.NegotiatePacketSize();

                // Set stream destination to our stream object
                mDevice.SetStreamDestination(mStream.LocalIPAddress, mStream.LocalPort);
            }
            catch (PvException ex)
            {
                // Failure at some point, display and abort
                MessageBox.Show(ex.Message);
                Disconnect();

                return;
            }

            if (mDevice.IsConnected)
            {
                mDevice.OnEventGenICam += new OnEventGenICam(mDevice_OnEventGenICam);

                // Register to all events of the parameters in the device's node map
                foreach (PvGenParameter lParameter in mDevice.GenParameters)
                {
                    lParameter.OnParameterUpdate += new OnParameterUpdateHandler(OnParameterChanged);
                }

                // Connect link disconnection handler
                mDevice.OnLinkDisconnected += new OnLinkDisconnectedHandler(OnLinkDisconnected);

                // Add device information
                manufacturerTextBox.Text = aDI.Vendor;
                modelTextBox.Text = aDI.Model;
                ipAddressTextBox.Text = aDI.IPAddress;
                macAddressTextBox.Text = aDI.MACAddress;
                nameTextBox.Text = aDI.UserDefinedName;

                if (aDI.UserDefinedName == "")
                {
                    nameTextBox.Text = "N/A";
                }
                else
                {
                    nameTextBox.Text = aDI.UserDefinedName;
                }

                // Fill acquisition mode combo box
                modeComboBox.Items.Clear();
                PvGenEnum lMode = mDevice.GenParameters.GetEnum("AcquisitionMode");
                if (lMode != null)
                {
                    foreach (PvGenEnumEntry lEntry in lMode)
                    {
                        if (lEntry.IsAvailable)
                        {
                            int lIndex = modeComboBox.Items.Add(lEntry.ValueString);
                            if (lEntry.ValueInt == lMode.ValueInt)
                            {
                                modeComboBox.SelectedIndex = lIndex;
                            }
                        }
                    }
                }
            }

            if (mStream.IsOpened)
            {
                // Ready image reception
                StartStreaming();
            }

            // Sync the UI with our new status
            EnableInterface();
        }

        void mDevice_OnEventGenICam(PvDevice aDevice, ushort aEventID, ushort aChannel, ulong aBlockID, ulong aTimestamp, List<PvGenParameter> aData)
        {
            string lLog = "Event ID: 0x" + aEventID.ToString("X4") + "\n";
            if (aData != null)
            {
                foreach (PvGenParameter p in aData)
                {
                    lLog += "\t" + p.Name + ": " + p.ToString() + "\n";
                }
            }

            Debug.WriteLine(lLog);
        }

        /// <summary>
        /// Disconnects from the device
        /// </summary>
        private void Disconnect()
        {
            // Close all configuration child windows
            mDeviceBrowser.Browser.GenParameterArray = null;
            CloseGenWindow(mDeviceBrowser);
            mStreamBrowser.Browser.GenParameterArray = null;
            CloseGenWindow(mStreamBrowser);

            // If streaming, stop streaming
            if (mStream.IsOpened)
            {
                StopStreaming();
                mStream.Close();
            }

            if (mDevice.IsConnected)
            {
                // Disconnect events
                mDevice.OnLinkDisconnected -= new OnLinkDisconnectedHandler(OnLinkDisconnected);
                foreach (PvGenParameter lP in mDevice.GenParameters)
                {
                    lP.OnParameterUpdate -= new OnParameterUpdateHandler(OnParameterChanged);
                }

                mDevice.Disconnect(); 
            }

            display.Clear();

            manufacturerTextBox.Text = "";
            modelTextBox.Text = "";
            ipAddressTextBox.Text = "";
            macAddressTextBox.Text = "";
            nameTextBox.Text = "";

            statusControl.Warning = "";

            // Sync the UI with our new status
            EnableInterface();
        }

        /// <summary>
        /// GenICam parameter invalidation event, registered for all parameters
        /// </summary>
        /// <param name="aParameter"></param>
        void OnParameterChanged(PvGenParameter aParameter)
        {
            string lName = aParameter.Name;
            if (lName == "AcquisitionMode")
            {
                // Have main UI thread update the acquisition mode combo box status
                BeginInvoke(mAcquisitionModeChangedHandler);
            }
        }

        /// <summary>
        /// Acquisition mode event handler in main thread.
        /// </summary>
        private void OnAcquisitionModeChanged()
        {
            // Get parameter
            PvGenEnum lParameter = mDevice.GenParameters.GetEnum("AcquisitionMode");

            // Update value: find which matches in the combo box
            string lValue = lParameter.ValueString;
            foreach (string lString in modeComboBox.Items)
            {
                if (lValue == lString)
                {
                    modeComboBox.SelectedValue = lString;
                }
            }
        }

        /// <summary>
        /// Direct acquisition state changed handler. Bring back to main UI thread.
        /// </summary>
        /// <param name="aDevice"></param>
        /// <param name="aStream"></param>
        /// <param name="aSource"></param>
        /// <param name="aState"></param>
        void OnAcquisitionStateChanged(PvDevice aDevice, PvStreamBase aStream, uint aSource, PvAcquisitionState aState)
        {
            // Invoke event in main UI thread
            BeginInvoke(mAcquisitionStateChangedHandler);
        }
        
        /// <summary>
        /// OnAcquisitionStateChanged main UI thread counterpart. Syncs acquisition control status.
        /// </summary>
        private void OnAcquisitionStateChanged()
        {
            EnableControls();
        }

        /// <summary>
        /// Displays a GenICam browser form. If already visible, close it for toggle effect.
        /// </summary>
        /// <param name="aForm"></param>
        /// <param name="aParams"></param>
        /// <param name="aTitle"></param>
        private void ShowGenWindow(BrowserForm aForm, PvGenParameterArray aParams, string aTitle)
        {
            if (aForm.Visible)
            {
                CloseGenWindow(aForm);
                return;
            }

            // Create, assigne parameters, set title and show modeless
            aForm.Text = aTitle;
            aForm.Browser.GenParameterArray = aParams;
            aForm.Show();
        }

        /// <summary>
        /// Closes a GenICam browser form.
        /// </summary>
        /// <param name="aForm"></param>
        private void CloseGenWindow(Form aForm)
        {
            aForm.Hide();
        }

        /// <summary>
        /// Direct device disconnect handler. Just jump back to main UI thread.
        /// </summary>
        /// <param name="aDevice"></param>
        private void OnLinkDisconnected(PvDevice aDevice)
        {
            BeginInvoke(mDisconnectedHandler);
        }

        /// <summary>
        /// Reaction to device disconnected event: stop streaming, close device connection.
        /// </summary>
        private void OnDisconnected()
        {
            MessageBox.Show("Connection to device lost.", Text, 
                MessageBoxButtons.OK, MessageBoxIcon.Information);

            StopStreaming();
            Disconnect();
        }

        /// <summary>
        /// Setups streaming. After calling this method the application is ready to receive data.
        /// StartAcquisition will instruct the device to actually start sending data.
        /// </summary>
        private void StartStreaming()
        {
            // Configure status control
            statusControl.Stream = mStream;
            statusControl.DisplayThread = mDisplayThread;

            // Start threads
            mDisplayThread.Start(mPipeline, mDevice.GenParameters);
            mDisplayThread.Priority = PvThreadPriority.AboveNormal;

            // Configure acquisition state manager
            mAcquisitionManager = new PvAcquisitionStateManager(mDevice, mStream);
            mAcquisitionManager.OnAcquisitionStateChanged += new OnAcquisitionStateChanged(OnAcquisitionStateChanged);

            // Start pipeline
            mPipeline.Start();
        }

        /// <summary>
        /// Callback from display thread delegating buffer display task
        /// </summary>
        /// <param name="aDisplayThread">Display thread</param>
        /// <param name="aBuffer">Buffer to display</param>
        void OnBufferDisplay(PvDisplayThread aDisplayThread, PvBuffer aBuffer)
        {
            display.Display(aBuffer);
        }

        /// <summary>
        /// Stops streaming. After calling this method the application is no longer armed or ready
        /// to receive data.
        /// </summary>
        private void StopStreaming()
        {
            if (!mDisplayThread.IsRunning)
            {
                return;
            }

            // Stop display thread
            mDisplayThread.Stop(false);

            // Release acquisition manager
            mAcquisitionManager = null;

            // Stop pipeline
            if (mPipeline.IsStarted)
            {
                mPipeline.Stop();
            }

            // Wait on display thread
            mDisplayThread.WaitComplete();
        }

        /// <summary>
        /// Starts acquisition.
        /// </summary>
        private void StartAcquisition()
        {
            // Get payload size
            UInt32 lPayloadSize = PayloadSize;
            if (lPayloadSize > 0)
            {
                // Propagate to pipeline to make sure buffers are big enough
                mPipeline.BufferSize = lPayloadSize;
            }

            // Reset pipeline
            mPipeline.Reset();

            // Reset stream statistics
            PvGenCommand lResetStats = mStream.Parameters.GetCommand("Reset");
            lResetStats.Execute();

            // Reset display thread stats (mostly frames displayed per seconds)
            mDisplayThread.ResetStatistics();

            // Use acquisition manager to send the acquisition start command to the device
            mAcquisitionManager.Start();
        }

        /// <summary>
        /// Stops the acquisition.
        /// </summary>
        private void StopAcquisition()
        {
            // Use acquisition manager to send the acquisition stop command to the device
            mAcquisitionManager.Stop();
        }

        /// <summary>
        /// Retrieve or guess the payload size
        /// </summary>
        private UInt32 PayloadSize
        {
            get
            {
                // Get parameters required
                PvGenInteger lPayloadSize = mDevice.GenParameters.GetInteger("PayloadSize");
                PvGenInteger lWidth = mDevice.GenParameters.GetInteger("Width");
                PvGenInteger lHeight = mDevice.GenParameters.GetInteger("Height");
                PvGenEnum lPixelFormat = mDevice.GenParameters.GetEnum("PixelFormat");

                // Try getting the payload size from the PayloadSize mandatory parameter
                Int64 lPayloadSizeValue = 0;
                if (lPayloadSize != null)
                {
                    lPayloadSizeValue = lPayloadSize.Value;
                }

                // Compute poor man's payload size - for devices not maintaining PayloadSize properly
                Int64 lPoorMansPayloadSize = 0;
                if ((lWidth != null) && (lHeight != null) && (lPixelFormat != null))
                {
                    Int64 lWidthValue = lWidth.Value;
                    Int64 lHeightValue = lHeight.Value;

                    Int64 lPixelFormatValue = lPixelFormat.ValueInt;
                    Int64 lPixelSizeInBits = PvImage.GetPixelBitCount((PvPixelType)lPixelFormatValue);

                    lPoorMansPayloadSize = (lWidthValue * lHeightValue * lPixelSizeInBits) / 8;
                }

                // Take max
                Int64 lBestPayloadSize = Math.Max(lPayloadSizeValue, lPoorMansPayloadSize);
                if ((lBestPayloadSize > 0) && (lBestPayloadSize < UInt32.MaxValue))
                {
                    // Round up to make it mod 32 (works around an issue with some devices)
                    if ((lBestPayloadSize % 32) != 0)
                    {
                        lBestPayloadSize = ((lBestPayloadSize / 32) + 1) * 32;
                    }

                    return (UInt32)lBestPayloadSize;
                }

                // Could not compute/retrieve payload size...
                return 0;
            }
        }

        /// <summary>
        /// Communication button event handler. Display communication parameters browser form.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void communicationButton_Click(object sender, EventArgs e)
        {
            ShowGenWindow(mCommBrowser, mDevice.GenLink, "Communication Control");
        }

        /// <summary>
        /// Device button event handler. Display device parameters browser form.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void deviceButton_Click(object sender, EventArgs e)
        {
            ShowGenWindow(mDeviceBrowser, mDevice.GenParameters, "Device Control");
        }

        /// <summary>
        /// Stream button event handler. Display stream parameters browser form.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void streamButton_Click(object sender, EventArgs e)
        {
            ShowGenWindow(mStreamBrowser, mStream.Parameters, "Image Stream Control");
        }

        /// <summary>
        /// Acquisition mode combo box changed. Propagate to device.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void modeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Nothing selected? Unexpected, just do nothing...
            if (modeComboBox.SelectedIndex < 0)
            {
                return;
            }

            PvGenEnum lMode = mDevice.GenParameters.GetEnum("AcquisitionMode");

            // Take selection, propagate to parameter
            string lSelected = (string)modeComboBox.SelectedItem;
            lMode.ValueString = lSelected;
        }

        /// <summary>
        /// Play button click handler. Start acquisition.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void playButton_Click(object sender, EventArgs e)
        {
            StartAcquisition();
        }

        /// <summary>
        /// Stop button click handler. Stop acquisition.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void stopButton_Click(object sender, EventArgs e)
        {
            StopAcquisition();
        }
    }
}

