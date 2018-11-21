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
using System.Threading;
using PvDotNet;
using PvGUIDotNet;


namespace PvDualSourceSample
{
    public interface INotifications
    {
        void UpdateAcquisitionState();
        void UpdateAcquisitionMode(bool aSynchronous);
    }

    public partial class MainForm : Form, INotifications
    {
        public MainForm()
        {
            InitializeComponent();

            // Handlers used to callbacks in the UI thread
            mDisconnectedHandler += new GenericHandler(OnDisconnected);
            mAcquisitionModeChangedHandler += new GenericHandler(OnAcquisitionModeChanged);
            mAcquisitionStateChangedHandler += new GenericHandler(OnAcquisitionStateChanged);

            // Set browser form owners
            mCommBrowser.Owner = this;
            mDeviceBrowser.Owner = this;
            Stream1Browser.Owner = this;
            Stream2Browser.Owner = this;
        }

        // Handler used to bring link disconnected event in the main UI thread
        public delegate void GenericHandler();
        private GenericHandler mDisconnectedHandler = null;

        // Handler used to bring acquisition mode changed event in the main UI thread
        private GenericHandler mAcquisitionModeChangedHandler = null;

        // Handler used to bring acquisition state manager callbacks in the main UI thread
        private GenericHandler mAcquisitionStateChangedHandler = null;

        // Device controller
        private PvDevice mDevice = new PvDevice();

        // Source class. Implemented in this project.
        private Source mSource1 = null;
        private Source mSource2 = null;

        // Parameter browsers
        private BrowserForm mCommBrowser = new BrowserForm();
        private BrowserForm mDeviceBrowser = new BrowserForm();
        private BrowserForm Stream1Browser = new BrowserForm();
        private BrowserForm Stream2Browser = new BrowserForm();

        /// <summary>
        /// Exit menu handler. Quits if the user is sure about it.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult lDR = MessageBox.Show("Are you sure you want to exit the application?",
                Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (lDR == DialogResult.Yes)
            {
                Close();
            }
        }

        private void selectConnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // create a device finder wnd and open the select device dialog
            PvDeviceFinderForm lFinder = new PvDeviceFinderForm();
            lFinder.Text = "GEV Device Selection";

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

        private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Show hourglass
            Cursor lOld = Cursor;
            Cursor = Cursors.WaitCursor;

            // Disconnect from device
            Disconnect();

            // Restore cursor
            Cursor = lOld;
        }

        private void communicationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenWindow(mCommBrowser, mDevice.GenLink, "Communication Control");
        }

        private void deviceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenWindow(mDeviceBrowser, mDevice.GenParameters, "Device Control");
        }

        private void source1StreamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenWindow(Stream1Browser, mSource1.Parameters, "Stream 1 Control");
        }

        private void source2StreamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenWindow(Stream2Browser, mSource2.Parameters, "Stream 2 Control");
        }

        private void modeComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetAcquisitionMode(modeComboBox1, "Source1");
        }

        private void playButton1_Click(object sender, EventArgs e)
        {
            mSource1.StartAcquisition();
        }

        private void stopButton1_Click(object sender, EventArgs e)
        {
            mSource1.StopAcquisition();
        }

        private void modeComboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetAcquisitionMode(modeComboBox2, "Source2");
        }

        private void SetAcquisitionMode(ComboBox aComboBox, string aSource)
        {
            PvGenEnum lSourceSelector = mDevice.GenParameters.GetEnum("SourceSelector");
            if (lSourceSelector != null)
            {
                // Select source
                lSourceSelector.ValueString = aSource;
            }

            PvGenEnum lAcquisitionMode = mDevice.GenParameters.GetEnum("AcquisitionMode");
            if ((lAcquisitionMode != null) && (aComboBox.SelectedItem != null))
            {
                try
                {
                    lAcquisitionMode.ValueString = (string)aComboBox.SelectedItem;
                }
                catch (PvException ex)
                {
                    MessageBox.Show(ex.Message, "Error",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void playButton2_Click(object sender, EventArgs e)
        {
            mSource2.StartAcquisition();
        }

        private void stopButton2_Click(object sender, EventArgs e)
        {
            mSource2.StopAcquisition();
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

                // Create sources
                mSource1 = new Source(0, this, mDevice, display1, statusControl1);
                mSource2 = new Source(1, this, mDevice, display2, statusControl2);

                // Fill acquisition mode combo box
                modeComboBox1.Items.Clear();
                modeComboBox2.Items.Clear();
                PvGenEnum lMode = mDevice.GenParameters.GetEnum("AcquisitionMode");
                if (lMode != null)
                {
                    foreach (PvGenEnumEntry lEntry in lMode)
                    {
                        if (lEntry.IsAvailable)
                        {
                            modeComboBox1.Items.Add(lEntry.ValueString);
                            modeComboBox2.Items.Add(lEntry.ValueString);
                        }
                    }
                }

                // Open channels
                PvGenEnum lSourceSelector = mDevice.GenParameters.GetEnum("SourceSelector");
                PvGenInteger lSourceStreamChannel = mDevice.GenParameters.GetInteger("SourceStreamChannel");
                Source[] lSources = { mSource1, mSource2 };
                if (lSourceSelector != null)
                {
                    for (UInt16 i = 0; i < 2; i++)
                    {
                        try
                        {
                            lSourceSelector.ValueString = "Source" + (i + 1).ToString();
                            UInt16 lChannel = (UInt16)lSourceStreamChannel.Value;

                            lSources[i].Open(aDI.IPAddress, lChannel);
                        }
                        catch (Exception)
                        {
                            // Ignore errors, just move on
                        }
                    }
                }
                else
                {
                    // No source selector, if transmitter assume 1 on channel 0
                    mSource1.Open(aDI.IPAddress, 0);
                }
            }
            catch (PvException ex)
            {
                // Failure at some point, display and abort
                MessageBox.Show(ex.Message);
                Disconnect();

                return;
            }

            // Sync the UI with our new status
            EnableInterface();
        }

        /// <summary>
        /// Disconnects from the device
        /// </summary>
        private void Disconnect()
        {
            // Close all configuration child windows
            mDeviceBrowser.Browser.GenParameterArray = null;
            CloseGenWindow(mDeviceBrowser);
            Stream1Browser.Browser.GenParameterArray = null;
            CloseGenWindow(Stream1Browser);
            Stream2Browser.Browser.GenParameterArray = null;
            CloseGenWindow(Stream2Browser);

            // If streaming, stop streaming, close
            if (mSource1 != null)
            {
                mSource1.Close();
                mSource1 = null;
            }
            if (mSource2 != null)
            {
                mSource2.Close();
                mSource2 = null;
            }

            mDevice.Disconnect();

            display1.Clear();
            display2.Clear();

            statusControl1.Warning = "";
            statusControl2.Warning = "";

            // Sync the UI with our new status
            EnableInterface();
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
            mSource1.StopStreaming();
            mSource2.StopStreaming();
            Disconnect();
        }

        /// <summary>
        /// Method that syncs the whole UI enabled state
        /// </summary>
        private void EnableInterface()
        {
            // Connect and disconnect button
            selectConnectToolStripMenuItem.Enabled = !mDevice.IsConnected;
            disconnectToolStripMenuItem.Enabled = mDevice.IsConnected;

            // Tree browser buttons
            EnableTreeBrowsers();

            // Acquisition controls
            EnableControls();
        }

        /// <summary>
        /// Enable the tree browser buttons
        /// </summary>
        private void EnableTreeBrowsers()
        {
            // Device and stream: enabled if connected
            deviceToolStripMenuItem.Enabled = mDevice.IsConnected;
            source1StreamToolStripMenuItem.Enabled = (mSource1 != null ) && mSource1.IsOpened;
            source2StreamToolStripMenuItem.Enabled = ( mSource2 != null ) && mSource2.IsOpened;

            // Communication button is always enabled
            communicationToolStripMenuItem.Enabled = true;
        }

        /// <summary>
        /// Enables the acquisitio controls
        /// </summary>
        private void EnableControls()
        {
            if ((mSource1 == null) || (mSource1.AcquisitionState == PvAcquisitionState.Unknown))
            {
                // Not connected: acquisition state manager not available, directly disable all
                modeComboBox1.Enabled = false;
                playButton1.Enabled = false;
                stopButton1.Enabled = false;
            }
            else
            {
                // Read current acquisition state
                bool lLocked = mSource1.AcquisitionState == PvAcquisitionState.Locked;

                // Mode and play are enabled if not locked
                modeComboBox1.Enabled = !lLocked;
                playButton1.Enabled = !lLocked;

                // Stop is enabled only if locked
                stopButton1.Enabled = lLocked;
            }

            if ((mSource2 == null) || (mSource2.AcquisitionState == PvAcquisitionState.Unknown))
            {
                // Not connected: acquisition state manager not available, directly disable all
                modeComboBox2.Enabled = false;
                playButton2.Enabled = false;
                stopButton2.Enabled = false;
            }
            else
            {
                // Read current acquisition state
                bool lLocked = mSource2.AcquisitionState == PvAcquisitionState.Locked;

                // Mode and play are enabled if not locked
                modeComboBox2.Enabled = !lLocked;
                playButton2.Enabled = !lLocked;

                // Stop is enabled only if locked
                stopButton2.Enabled = lLocked;
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            EnableInterface();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
        }

        public void UpdateAcquisitionMode(bool aSynchronous)
        {
            if (aSynchronous)
            {
                Invoke(mAcquisitionModeChangedHandler);
            }
            else
            {
                BeginInvoke(mAcquisitionModeChangedHandler);
            }
        }

        private void OnAcquisitionModeChanged()
        {
            // Which source?
            ComboBox lComboBox = modeComboBox1; // Default
            PvGenEnum lSourceSelector = mDevice.GenParameters.GetEnum("SourceSelector");
            if (lSourceSelector != null)
            {
                string lValue = lSourceSelector.ValueString;
                if (lValue == "Source1")
                {
                    lComboBox = modeComboBox1;
                }
                else if (lValue == "Source2")
                {
                    lComboBox = modeComboBox2;
                }
            }

            // Select right value in combo box
            PvGenEnum lAcquisitionMode = mDevice.GenParameters.GetEnum("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                string lValue = lAcquisitionMode.ValueString;
                foreach (string lItem in lComboBox.Items)
                {
                    if ((lValue == lItem) && (lItem != (string)lComboBox.SelectedItem))
                    {
                        lComboBox.SelectedItem = lItem;
                    }
                }
            }
        }

        public void UpdateAcquisitionState()
        {
            BeginInvoke(mAcquisitionStateChangedHandler);
        }

        private void OnAcquisitionStateChanged()
        {
            EnableControls();
        }
    }
}

