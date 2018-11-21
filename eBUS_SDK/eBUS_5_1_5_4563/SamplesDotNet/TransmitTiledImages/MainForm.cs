// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using PvDotNet;
using PvGUIDotNet;


namespace TransmitTiledImages
{
    public partial class MainForm : Form
    {
        const UInt16 cWidthResolution = 640;
        const UInt16 cHeightResolution = 480;
        const UInt16 cBufferPoolSize = 4;
        const UInt16 cFPS = 30;
        const UInt16 cPacketSize = 1476;
        const UInt16 cMaxRows = 2;
        const UInt16 cMaxColumns = 2;
        const string cMulticastGroupIP = "239.192.1.1";
        const string cMulticastGroupPort = "1042";
        const string cMaintainAspectRatio = "Maintain Aspect Ratio";
        const string cMaintainResolution = "Maintain Resolution";
        const string cStrecthToFit = "Stretch to Fit";

        // Delegate to update button.
        private delegate void UpdateButtonDelegate(ref Button aButton, string aNewText, bool aEnable);
        // Delegate to update text box.
        private delegate void UpdateTextBoxDelegate(ref TextBox aTextBox, string aNewText, bool aEnable);

        private ConnectionLabels[,] mConnectionLabels = new ConnectionLabels[cMaxRows, cMaxColumns];
        private Button[,] mConnectButton = new Button[cMaxRows, cMaxColumns];
        private StreamThread[,] mStreamThreads = new StreamThread[cMaxRows, cMaxColumns];
        private TransmitterConfig mTransmitterConfig = new TransmitterConfig();  
        private TransmitterThread mTransmitterThread = new TransmitterThread();
        private BrowserForm[, ,] mBrowserForms = new BrowserForm[cMaxRows, cMaxColumns, 3];
        private PvVirtualDeviceGEV mVirtualDevice = new PvVirtualDeviceGEV();

        private string mLastFileName;

        // Object that keep track of the last buffer for each stream.
        private CurrentBuffersTable mCurrentBuffersTable = new CurrentBuffersTable();

        private delegate void DisconnectHandler(int aRow, int aColumn);
        private DisconnectHandler mDisconnectedHandler = null;

        /// <summary>
        /// Constructors.
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Method to initialize options and class data members.
        /// </summary>
        private void InitializesOptionsAndDataMembers()
        {
            mDisconnectedHandler += new DisconnectHandler(OnDisconnectHandler);

            widthTextBox.Text = cWidthResolution.ToString();
            heightTextBox.Text = cHeightResolution.ToString();
            destinationAddressTextBox.Text = cMulticastGroupIP;
            destinationPortTextBox.Text = cMulticastGroupPort;
            desiredFrameRateTextBox.Text = cFPS.ToString();
            maxPacketSizeTextBox.Text = cPacketSize.ToString(); 
            if (!PopulateNICAddress())
            {
                MessageBox.Show("No valid interfaces were found. Make sure that there is at least one network adapter connected to a network.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                Close();
                return;
            }

            if (tilingModeComboBox.Items.Count > 0)
            {
                tilingModeComboBox.SelectedIndex = 0;
            }

            // Initializes 2x2 connection text box array.
            mConnectionLabels[0, 0] = new ConnectionLabels(ipLabel1, macLabel1, serialNumber1, modelLabel1);
            mConnectionLabels[0, 1] = new ConnectionLabels(ipLabel2, macLabel2, serialNumber2, modelLabel2);
            mConnectionLabels[1, 0] = new ConnectionLabels(ipLabel3, macLabel3, serialNumber3, modelLabel3);
            mConnectionLabels[1, 1] = new ConnectionLabels(ipLabel4, macLabel4, serialNumber4, modelLabel4);

            // Initializes 2x2 connect button array.
            mConnectButton[0, 0] = connect11Button;
            mConnectButton[0, 1] = connect12Button;
            mConnectButton[1, 0] = connect21Button;
            mConnectButton[1, 1] = connect22Button;

            // Initializes GenICam browsers array.
            for (UInt16 i = 0; i < cMaxRows; i++)
            {
                for (UInt16 j = 0; j < cMaxColumns; j++)
                {
                    for (UInt16 k = 0; k < 3; k++)
                    {
                        BrowserForm lForm = new BrowserForm();
                        lForm.Owner = this;

                        mBrowserForms[i, j, k] = lForm;
                    }
                }
            }

            // Initializes stream threads.
            for (UInt16 i = 0; i < cMaxRows; i++)
            {
                for (UInt16 j = 0; j < cMaxColumns; j++)
                {
                    mStreamThreads[i, j] = new StreamThread(pvDisplayControl);
                    mStreamThreads[i, j].OnLinkDisconnectedEvent += new StreamThread.OnLinkDisconnectedDelegate(OnLinkDisconnectedMessage);
                    mStreamThreads[i, j].Initialize(i, j, ref mCurrentBuffersTable);
                }
            }

            SetTransmitterConfig(mTransmitterConfig);
            mTransmitterThread.Initialize(pvDisplayControl, ref mCurrentBuffersTable);
        }

        /// <summary>
        /// To response to the delegate UpdateButtonDelegate.
        /// </summary>
        /// <param name="aButton">Button to be updated.</param>
        /// <param name="aNewText">New text.</param>
        /// <param name="aEnable">true - Enable button. false - Disable butto.n</param>
        private void UpdateButton(ref Button aButton, string aNewText, bool aEnable)
        {
            if (aButton.InvokeRequired == true)
            {
                UpdateButtonDelegate lUpdateButton = new UpdateButtonDelegate(UpdateButton);
                aButton.Invoke(lUpdateButton, aButton, aNewText, aEnable);
            }
            else
            {
                aButton.Text = aNewText;
                aButton.Enabled = aEnable;
            }
        }

        /// <summary>
        /// To response to the delegate UpdateTextDelegate.
        /// </summary>
        /// <param name="aTextBox">Text box to be updated.</param>
        /// <param name="aNewText">New text.</param>
        /// <param name="aEnable">True - Enable button. False - Disable button.</param>
        private void UpdateTextBox(ref TextBox aTextBox, string aNewText, bool aEnable)
        {

            if (aTextBox.InvokeRequired == true)
            {
                UpdateTextBoxDelegate lUpdateTextBox = new UpdateTextBoxDelegate(UpdateTextBox);
                aTextBox.Invoke(lUpdateTextBox, aTextBox, aNewText, aEnable);
            }
            else
            {
                aTextBox.Text = aNewText;
                aTextBox.Enabled = aEnable;
            }
        }

        private void SetTransmitterConfig(TransmitterConfig aTransmitterConfig)
        {
            widthTextBox.Text = aTransmitterConfig.Width.ToString();
            heightTextBox.Text = aTransmitterConfig.Height.ToString();
            desiredFrameRateTextBox.Text = aTransmitterConfig.Fps.ToString();
            destinationAddressTextBox.Text = aTransmitterConfig.DestinationIPAddress;
            destinationPortTextBox.Text = aTransmitterConfig.DestinationPort.ToString();
            maxPacketSizeTextBox.Text = aTransmitterConfig.PacketSize.ToString();

            tilingModeComboBox.SelectedItem = aTransmitterConfig.Mode;

            switch (aTransmitterConfig.Mode)
            {
                case TilingMode.TILING_MODE_RATIO:
                    tilingModeComboBox.Text = cMaintainAspectRatio;
                    break;
                case TilingMode.TILING_MODE_CROP:
                    tilingModeComboBox.Text = cMaintainResolution;
                    break;
                case TilingMode.TILING_MODE_STRETCH:
                    tilingModeComboBox.Text = cStrecthToFit;
                    break;
                default:
                    break;
            }

            string lIfAddress = aTransmitterConfig.SourceIPAddress;
            if (lIfAddress == "")
            {
                interfaceAddressComboBox.SelectedIndex = 0;
            }
            else
            {
                for (int i = 0; i < interfaceAddressComboBox.Items.Count; i++)
                {
                    if (interfaceAddressComboBox.Items[i].ToString() == lIfAddress)
                    {
                        interfaceAddressComboBox.SelectedIndex = i;
                        break;
                    }
                }
            }
            mVirtualDevice.StartListening(interfaceAddressComboBox.Text);
        }

        void GetTransmitterConfig(TransmitterConfig aTransmitterConfig)
        {
            // Read the Information from the GUI on the configuration of the stream.
            aTransmitterConfig.Width = Convert.ToUInt32(widthTextBox.Text);
            aTransmitterConfig.Height = Convert.ToUInt32(heightTextBox.Text);
            aTransmitterConfig.Fps = Convert.ToUInt32(desiredFrameRateTextBox.Text);
            aTransmitterConfig.DestinationIPAddress = destinationAddressTextBox.Text;
            aTransmitterConfig.PacketSize = Convert.ToUInt32(maxPacketSizeTextBox.Text);
            aTransmitterConfig.SourceIPAddress = interfaceAddressComboBox.SelectedItem.ToString();

            switch (tilingModeComboBox.Text)
            {
                case cMaintainAspectRatio:
                    mTransmitterConfig.Mode = TilingMode.TILING_MODE_RATIO;
                    break;
                case cMaintainResolution:
                    mTransmitterConfig.Mode = TilingMode.TILING_MODE_CROP;
                    break;
                case cStrecthToFit:
                    mTransmitterConfig.Mode = TilingMode.TILING_MODE_STRETCH;
                    break;
                default:
                    break;
            }
        }
        /// <summary>
        /// To set the GUI controls state.
        /// </summary>
        private void SetGUIControlState(bool aEnabled)
        {
            for (UInt16 i = 0; i < cMaxRows; i++)
            {
                for (UInt16 j = 0; j < cMaxColumns; j++)
                {
                    mConnectButton[i, j].Enabled = aEnabled;
                }
            }

            startButton.Enabled = aEnabled;
            stopButton.Enabled = !aEnabled;

            destinationAddressTextBox.Enabled = aEnabled;
            destinationPortTextBox.Enabled = aEnabled;
            interfaceAddressComboBox.Enabled = aEnabled;
            desiredFrameRateTextBox.Enabled = aEnabled;
            maxPacketSizeTextBox.Enabled = aEnabled;
            widthTextBox.Enabled = aEnabled;
            heightTextBox.Enabled = aEnabled;
            tilingModeComboBox.Enabled = aEnabled;
            startButton.Enabled = aEnabled;
            stopButton.Enabled = !aEnabled;
        }

        private void UpdateTransmitterStats()
        {
            StringBuilder lSB = new StringBuilder();
            TransmitterStats lStats = new TransmitterStats();

            // Get the stats.
            mTransmitterThread.GetStats(ref lStats);

            // Format the stats.
            lSB.Append("Transmitted: " + lStats.BlocksTransmitted.ToString() + " images   ");
            lSB.Append(lStats.InstantaneousTransmissionRate.ToString("f1") + " FPS   ");
            lSB.Append(lStats.InstantaneousPayloadThroughput.ToString("f1") + " Mbps   ");
            lSB.Append("Display: " + lStats.DisplayFrameRate.ToString() + "FPS");

            // Display the stats.
            transmitterStatusLabel.Text = lSB.ToString();
        }

        /// <summary>
        /// To find NIC IP address and add to interface address ComboBox.
        /// </summary>
        private bool PopulateNICAddress()
        {
            bool lFoundNic = false;
            PvSystem lSystem = new PvSystem();
            lSystem.Find();
            foreach (PvInterface lInterface in lSystem)
            {
                PvNetworkAdapter lNetworkAdapter = lInterface as PvNetworkAdapter;
                if (lNetworkAdapter == null)
                {
                    continue;
                }

                PvIPConfiguration[] lConfigs = lNetworkAdapter.IPConfigurations;
                foreach (PvIPConfiguration lConfig in lConfigs)
                {
                    if (lConfig.IPAddress != "0.0.0.0")
                    {
                        interfaceAddressComboBox.Items.Add(lConfig.IPAddress);
                        lFoundNic = true;
                    }
                }
            }
            if (interfaceAddressComboBox.Items.Count > 0)
            {
                interfaceAddressComboBox.SelectedIndex = 0;
            }
            
            return lFoundNic;
        }

        /// <summary>
        /// To select the device.
        /// </summary>
        /// <returns>PvDeviceInfo of the selected device.</returns>
        private PvDeviceInfo SelectDevice()
        {
            PvDeviceFinderForm lPvDevDinder = new PvDeviceFinderForm();
            // Show device finder
            if ((lPvDevDinder.ShowDialog() != DialogResult.OK) ||
                (lPvDevDinder.Selected == null))
            {
                return null;
            }
            // return selected device.
            return lPvDevDinder.Selected;
        }

        /// <summary>
        /// To connect to the selected device.
        /// </summary>
        /// <param name="aRow">Row index.</param>
        /// <param name="aColumn">Column index.</param>
        private void ConnectDevice(UInt16 aRow, UInt16 aColumn)
        {
            Cursor lOldCursor;
            lOldCursor = Cursor;

            PvDeviceInfo lPvDevInfo = null;

            if (aRow < 0 || aRow >= cMaxRows || aColumn < 0 || aColumn >= cMaxColumns)
            {
                return;
            }
            try
            {
                lPvDevInfo = SelectDevice();
                if (lPvDevInfo == null)
                {
                    return;
                }

                mStreamThreads[aRow, aColumn].Connect(lPvDevInfo);
                Cursor = Cursors.WaitCursor;
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message, Text);
                Cursor = lOldCursor;
                return;
            }

            if (mStreamThreads[aRow, aColumn].IsConnected == true)
            {
                mConnectionLabels[aRow, aColumn].IPAddress = mStreamThreads[aRow, aColumn].IPAddress;
                mConnectionLabels[aRow, aColumn].MACAddress = mStreamThreads[aRow, aColumn].MACAddress;
                mConnectionLabels[aRow, aColumn].SerialNumber = mStreamThreads[aRow, aColumn].SerialNumber;
                mConnectionLabels[aRow, aColumn].Model = mStreamThreads[aRow, aColumn].Model;
                mConnectionLabels[aRow, aColumn].UserDefinedName = mStreamThreads[aRow, aColumn].UserDefinedName;
                mConnectionLabels[aRow, aColumn].Update();
                mConnectButton[aRow, aColumn].Text = "Disconnect";
            }
            else
            {
                MessageBox.Show("Cannot connect to the device.", Text, MessageBoxButtons.OK);
            }
            
            Cursor = lOldCursor;
        }

        /// <summary>
        /// To disconnect the selected device and updates GUI controls.
        /// </summary>
        /// <param name="aRow">Row number.</param>
        /// <param name="aColumn">Column number.</param>
        private void DisconnectDevice(int aRow, int aColumn)
        {
            if (aRow < 0 || aRow >= cMaxRows || aColumn < 0 || aColumn >= cMaxColumns)
            {
                return;
            }
            if (mStreamThreads[aRow, aColumn] == null || mStreamThreads[aRow, aColumn].Device == null)
            {
                return;
            }

            try
            {
                Application.UseWaitCursor = true;
                mStreamThreads[aRow, aColumn].Disconnect();
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message, Text);
            }

            mConnectionLabels[aRow, aColumn].Reset();
            mConnectButton[aRow, aColumn].Text = "Select/Connect";

            for (UInt16 k = 0; k < 3; k++)
            {
                mBrowserForms[aRow, aColumn, k].Browser.GenParameterArray = null;
                mBrowserForms[aRow, aColumn, k].Hide();
            }

            Application.UseWaitCursor = false;
        }

        /// <summary>
        /// Start transmitting tiled image.
        /// </summary>
        private void Start()
        {
            UInt32 lMaxWidth;
            UInt32 lMaxHeight;
            UInt32 lCurrentWidth = 0;;
            UInt32 lCurrentHeight = 0;
            bool lResult;

            Cursor lOldCursor;

            if (GatherParameters() != true)
            {
                return;
            }

            lOldCursor = Cursor;

            // Update the transmitter configuration from the GUI.
            GetTransmitterConfig(mTransmitterConfig);

            try
            {
                PvNetworkUtils.IPStrToDWORD(mTransmitterConfig.DestinationIPAddress);
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid destination IP address!", Text);
                return;
            }

            // Here we will start by initializing all the threads up front. 
            // This allows to fail when out of the memory without too much side effects!
            lMaxWidth = cWidthResolution;
            lMaxHeight = cHeightResolution;

            Cursor = Cursors.WaitCursor;
            for (int i = 0; i < cMaxRows; i++)
            {
                for (int j = 0; j < cMaxColumns; j++)
                {
                    if (mStreamThreads[i, j].IsConnected)
                    {
                        lResult = mStreamThreads[i, j].Configure(ref lCurrentWidth, ref lCurrentHeight);
                        if (!lResult)
                        {
                            MessageBox.Show("Unable to configure the stream thread", "Configuration failure", MessageBoxButtons.OK);
                            Cursor = lOldCursor;
                            return;
                        }

                        lMaxWidth = Math.Max(lMaxWidth, lCurrentWidth);
                        lMaxHeight = Math.Max(lMaxHeight, lCurrentHeight);
                    }
                }
            }                       
            // Configure the transmitter.
            if (mTransmitterThread.Configure(mTransmitterConfig, (Int32)lMaxWidth, (Int32)lMaxHeight) != true)
            {
                Stop();
                MessageBox.Show("Unable to configure the transmitter thread", "Configuration failure", MessageBoxButtons.OK);
                Cursor = lOldCursor;
                return;
            }

            // Now that all the threads are initialized, we can start them.
            for (int i = 0; i < cMaxRows; i++)
            {
                for (int j = 0; j < cMaxColumns; j++)
                {
                    if (mStreamThreads[i, j].IsConnected)
                    {
                        lResult = mStreamThreads[i, j].Start();
                        if (!lResult)
                        {
                            Stop();
                            MessageBox.Show("Unable to start the stream thread", "Configuration failure", MessageBoxButtons.OK);
                            Cursor = lOldCursor;
                            return;
                        }
                    }
                }
            }
        
            // Start the transmitter.
            mTransmitterThread.Start();
            // Start timer to display transmit status.
            timer.Start();
            // Set GUI control state.
            SetGUIControlState(false);
            transmitterStatusLabel.Text = "";
            // Set to old cursor.
            Cursor = lOldCursor;
        }

        /// <summary>
        /// Stop transmitting tiled image.
        /// </summary>
        private void Stop()
        {
            Cursor lOldCursor = Cursor;
            Cursor = Cursors.WaitCursor;

            timer.Stop();

            mTransmitterThread.Stop();

            // Stop the stream thread.
            for (int i = 0; i < cMaxRows; i++)
            {
                for (int j = 0; j < cMaxColumns; j++)
                {
                    mStreamThreads[i, j].Stop();
                }
            }

            // And now we can restore the access to the GUI.
            SetGUIControlState(true);
            
            Cursor = lOldCursor;
        }

        private void Open()
        {
            PvConfigurationReader lReader = new PvConfigurationReader();
            string lFileName;
            TransmitterConfig lTransmitterConfig = new TransmitterConfig();
            Cursor lOldCursor = Cursor;

            OpenFileDialog lOpenFileDlg = new OpenFileDialog();
            lOpenFileDlg.InitialDirectory = "";
            lOpenFileDlg.Filter = "PvTransmitTileImage Sample Configuration Files (*.pvt)|*.pvt|All files (*.*)|*.*" ;
            lOpenFileDlg.FilterIndex = 0;
            lOpenFileDlg.Title = ( "Open Project" );

            if (lOpenFileDlg.ShowDialog() != DialogResult.OK)
            {
                return;
            }

            Stop();

            Cursor = Cursors.WaitCursor;
            lFileName = lOpenFileDlg.FileName; 
            try
            {
                // We will close all the connections first.
                for (UInt16 i = 0; i < cMaxRows; i++)
                {
                    for (UInt16 j = 0; j < cMaxColumns; j++)
                    {
                        // Ensure the stream is disconnected and the GUI is disabled.
                        DisconnectDevice(i, j);
                    }
                }
                
                // Now load the file.
                lReader.Load( lFileName );

                // Parse the file.
                for(UInt16 i = 0; i < cMaxRows; i++)
                {
                    for(UInt16 j = 0; j < cMaxColumns; j++)
                    {
                        // Load the connection from the XML.
                        mStreamThreads[i, j].Restore(lReader); 

                        // Update the GUI
                        if (mStreamThreads[i, j].IsConnected)
                        {
                            mConnectButton[i, j].Text = "Disconnect";
                            mConnectButton[i, j].Enabled = true;

                            mConnectionLabels[i, j].IPAddress = mStreamThreads[i, j].IPAddress;
                            mConnectionLabels[i, j].MACAddress = mStreamThreads[i, j].MACAddress;
                            mConnectionLabels[i, j].Model = mStreamThreads[i, j].Model;
                            mConnectionLabels[i, j].UserDefinedName = mStreamThreads[i, j].UserDefinedName;
                            mConnectionLabels[i, j].Update();
                        }
                    }
                }

                // Read the configuration and reset the GUI.
                lTransmitterConfig.Restore(ref lReader);
                SetTransmitterConfig( lTransmitterConfig );

                // Update the save menu state.
                mLastFileName = lFileName;
                Text = "TransmitTiledImages - " + lFileName;
                saveToolStripMenuItem.Enabled = true;
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message, Text, MessageBoxButtons.OK);
            }
            Cursor = lOldCursor;
        }

        private void SaveAs( string aFileName )
        {
            PvConfigurationWriter lWriter = new PvConfigurationWriter();
            TransmitterConfig lTransmitterConfig = new TransmitterConfig();
            Cursor lOldCursor = Cursor;

            if( aFileName.Length <= 0 )
            {
                SaveFileDialog lSaveFileDlg = new SaveFileDialog();
                lSaveFileDlg.InitialDirectory = "";
                lSaveFileDlg.Filter = "PvTransmitTileImage Sample Configuration Files (*.pvt)|*.pvt|All files (*.*)|*.*" ;
                lSaveFileDlg.FilterIndex = 0;
                lSaveFileDlg.Title = ( "Save Project" );

                if (lSaveFileDlg.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                Cursor = Cursors.WaitCursor;
                aFileName = lSaveFileDlg.FileName;         
            }
            try
            {
                for (int i = 0; i < cMaxRows; i++)
                {
                    for (int j = 0; j < cMaxColumns; j++)
                    {
                        mStreamThreads[i, j].Store(ref lWriter);
                    }
                }

                // Now read the Transmitter configuration and save it.
                GetTransmitterConfig(lTransmitterConfig);
                lTransmitterConfig.Store(ref lWriter);

                // Store the configuration.
                lWriter.Save(aFileName);
                mLastFileName = aFileName;
                Text = "TransmitTiledImages - " + aFileName;
                saveToolStripMenuItem.Enabled = true;
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message, Text, MessageBoxButtons.OK);
            }
            Cursor = lOldCursor;
        }

        void OpenBrowser(StreamThread aST, int aParameters)
        {
            BrowserForm lForm = mBrowserForms[aST.Row, aST.Column, aParameters];
            if (lForm.Visible)
            {
                lForm.Hide();
            }

            int aIndex = aST.Row * 2 + aST.Column + 1;

            string lTitle = "GenICamBrowser";
            PvGenParameterArray lParams = null;
            switch (aParameters)
            {
                case 0:
                    lTitle = aIndex.ToString() + ": Communication parameters";
                    lParams = aST.Device.CommunicationParameters;
                    break;

                case 1:
                    lTitle = aIndex.ToString() + ": Device parameters";
                    lParams = aST.Device.Parameters;
                    break;

                case 2:
                    lTitle = aIndex.ToString() + ": Image stream parameters";
                    lParams = aST.Stream.Parameters;
                    break;
            }

            // Creates, assigns parameters, set title and show modeless.
            lForm.Text = lTitle;
            lForm.Browser.GenParameterArray = lParams;
            lForm.Show();
        }

        private bool GatherParameters()
        {
            bool lResult = true;

            if (errorProvider.GetError(widthTextBox) != "")
            {
                widthTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(widthTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (errorProvider.GetError(heightTextBox) != "")
            {
                heightTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(heightTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (errorProvider.GetError(destinationAddressTextBox) != "")
            {
                destinationAddressTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(destinationAddressTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (errorProvider.GetError(destinationPortTextBox) != "")
            {
                destinationPortTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(destinationPortTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (errorProvider.GetError(desiredFrameRateTextBox) != "")
            {
                desiredFrameRateTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(desiredFrameRateTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (errorProvider.GetError(maxPacketSizeTextBox) != "")
            {
                maxPacketSizeTextBox.Focus();
                MessageBox.Show(errorProvider.GetError(maxPacketSizeTextBox), Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return lResult;
        }

        /// <summary>
        /// Event received from stream thread. Use BeginInvoke to bring it back in the 
        /// main UI thread context.
        /// </summary>
        /// <param name="aRow"></param>
        /// <param name="aColumn"></param>
        public void OnLinkDisconnectedMessage(int aRow, int aColumn)
        {
            object[] lParameters = new object[] { aRow, aColumn };
            BeginInvoke(mDisconnectedHandler, lParameters);
        }

        /// <summary>
        /// To display error message when the OnLinkDisconnectedEvent occurred.
        /// </summary>
        /// <param name="aRow">Row number.</param>
        /// <param name="aColumn">Column number.</param>
        public void OnDisconnectHandler(int aRow, int aColumn)
        {
            if (aRow < 0 || aRow >= cMaxRows || aColumn < 0 || aColumn >= cMaxColumns)
            {
                return;
            }
            DisconnectDevice(aRow, aColumn);

            int lCountOfConnected = 0;
            for (UInt16 i = 0; i < cMaxRows; i++)
            {
                for (UInt16 j = 0; j < cMaxColumns; j++)
                {
                    if (mStreamThreads[i, j].IsConnected)
                    {
                        lCountOfConnected++;
                    }
                }
            }
            if ( lCountOfConnected == 0 )
            {
                Stop();
            }

            string lTitle = "Lost connection";
            if (!string.IsNullOrEmpty(mStreamThreads[aRow, aColumn].IPAddress))
            {
                // Show GEV information
                lTitle = string.Format("Lost connection at: ( {0}, {1} ) {2}, {3} [ {4} ]", aRow + 1, aColumn + 1, mStreamThreads[aRow, aColumn].Model, mStreamThreads[aRow, aColumn].MACAddress, mStreamThreads[aRow, aColumn].IPAddress);
            }
            else if (!string.IsNullOrEmpty(mStreamThreads[aRow, aColumn].SerialNumber))
            {
                // Show U3V information
                lTitle = string.Format("Lost connection at: ( {0}, {1} ) {2}, {3}", aRow + 1, aColumn + 1, mStreamThreads[aRow, aColumn].Model, mStreamThreads[aRow, aColumn].SerialNumber);
            }
            MessageBox.Show(lTitle, Text, MessageBoxButtons.OK);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult lDR = MessageBox.Show("Are you sure you want to exit the application?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (lDR == DialogResult.Yes)
            {
                Stop();
                Close();
            }
        }

        private void connect11Button_Click(object sender, EventArgs e)
        {
            if (connect11Button.Text == "Select/Connect")
            {
                ConnectDevice(0, 0);
            }
            else if (connect11Button.Text == "Disconnect")
            {
                DisconnectDevice(0, 0);
            }
        }

        private void connect12Button_Click(object sender, EventArgs e)
        {
            if (connect12Button.Text == "Select/Connect")
            {
                ConnectDevice(0, 1);
            }
            else if (connect12Button.Text == "Disconnect")
            {
                DisconnectDevice(0, 1);
            }
        }

        private void connect21Button_Click(object sender, EventArgs e)
        {
            if (connect21Button.Text == "Select/Connect")
            {
                ConnectDevice(1, 0);
            }
            else if (connect21Button.Text == "Disconnect")
            {
                DisconnectDevice(1, 0);
            }
        }

        private void connect22Button_Click(object sender, EventArgs e)
        {
            if (connect22Button.Text == "Select/Connect")
            {
                ConnectDevice(1, 1);
            }
            else if (connect22Button.Text == "Disconnect")
            {
                DisconnectDevice(1, 1);
            }
        }

        private void startButton_Click(object sender, EventArgs e)
        {
            Start();
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            Stop();
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            UpdateTransmitterStats();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Open();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveAs(mLastFileName);
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveAs("");
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutForm lForm = new AboutForm();
            lForm.ShowDialog( this );
        }

        private void interfaceAddressComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            mVirtualDevice.StopListening();
            mVirtualDevice.StartListening(interfaceAddressComboBox.Text);
        }

        private void browserButton_Click(object sender, EventArgs e)
        {
            Button lButton = sender as Button;
            if (lButton == null)
            {
                return;
            }

            StreamThread lST = null;
            if (lButton == browserButton1) lST = mStreamThreads[0, 0];
            if (lButton == browserButton2) lST = mStreamThreads[0, 1];
            if (lButton == browserButton3) lST = mStreamThreads[1, 0];
            if (lButton == browserButton4) lST = mStreamThreads[1, 1];

            commMenuItem.Enabled = (lST.Device != null);
            commMenuItem.Tag = lST;

            deviceMenuItem.Enabled = (lST.Device != null) && lST.IsConnected;
            deviceMenuItem.Tag = lST;

            streamMenuItem.Enabled = (lST.Device != null) && lST.Stream.IsOpen;
            streamMenuItem.Tag = lST;

            System.Drawing.Point lP = lButton.Parent.PointToScreen(lButton.Location);
            lP.Offset(0, lButton.Height);

            browserMenu.Show(lP);
        }

        private void commMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem lItem = sender as ToolStripMenuItem;
            StreamThread lST = lItem.Tag as StreamThread;
            OpenBrowser(lST, 0);
        }

        private void deviceMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem lItem = sender as ToolStripMenuItem;
            StreamThread lST = lItem.Tag as StreamThread;
            OpenBrowser(lST, 1);
        }

        private void streamMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem lItem = sender as ToolStripMenuItem;
            StreamThread lST = lItem.Tag as StreamThread;
            OpenBrowser(lST, 2);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            InitializesOptionsAndDataMembers();
        }

        private void widthTextBox_Validating(object sender, CancelEventArgs e) 
        {
            String lErrMsg = "";
            try
            {
                ushort lWidth = ushort.Parse(widthTextBox.Text);
                if (lWidth < 64 || lWidth > 4096)
                {
                    lErrMsg = "Width value must be between 64 and 4096.";
                }
            }
            catch
            {
                lErrMsg = "Width value must be between 64 and 4096.";
            }
            if (lErrMsg != "")
            {
                widthTextBox.Focus();
            }
            errorProvider.SetError(widthTextBox, lErrMsg);
        }

        private void heightTextBox_Validating(object sender, CancelEventArgs e)
        {
            String lErrMsg = "";
            try
            {
                ushort lHeight = ushort.Parse(heightTextBox.Text);
                if (lHeight < 64 || lHeight > 4096)
                {
                    lErrMsg = "Height value must be between 64 and 4096.";
                }
            }
            catch
            {
                lErrMsg = "Height value must be between 64 and 4096.";
            }
            if (lErrMsg != "")
            {
                heightTextBox.Focus();
            }
            errorProvider.SetError(heightTextBox, lErrMsg);
        }

        private void destinationAddressTextBox_Validating(object sender, CancelEventArgs e)
        {
            String lErrMsg = "";
            try
            {
                uint lDesAddress = PvNetworkUtils.IPStrToDWORD(destinationAddressTextBox.Text);
            }
            catch
            {
                lErrMsg = "Invalid destination IP address.";
            }
            if (lErrMsg != "")
            {
                destinationAddressTextBox.Focus();
            }
            errorProvider.SetError(destinationAddressTextBox, lErrMsg);
        }

        private void destinationPortTextBox_Validating(object sender, CancelEventArgs e)
        {
            String lErrMsg = "";
            try
            {
                uint lDesPort = uint.Parse(destinationPortTextBox.Text);
            }
            catch
            {
                lErrMsg = "Invalid source port.";
            }
            if (lErrMsg != "")
            {
                destinationPortTextBox.Focus();
            }
            errorProvider.SetError(destinationPortTextBox, lErrMsg);
        }

        private void desiredFrameRateTextBox_Validating(object sender, CancelEventArgs e)
        {
            String lErrMsg = "";
            try
            {
                ushort lFrameRate = ushort.Parse(desiredFrameRateTextBox.Text);
                if (lFrameRate < 0 || lFrameRate > 1000)
                {
                    lErrMsg = "Desire frame rate must be between 0 and 1000.";
                }
            }
            catch
            {
                lErrMsg = "Desire frame rate must be between 0 and 1000.";
            }
            if (lErrMsg != "")
            {
                desiredFrameRateTextBox.Focus();
            }
            errorProvider.SetError(desiredFrameRateTextBox, lErrMsg);
        }

        private void maxPacketSizeTextBox_Validating(object sender, CancelEventArgs e)
        {
            String lErrMsg = "";
            try
            {
                ushort lPacketSize = ushort.Parse(maxPacketSizeTextBox.Text);
                if (lPacketSize < 72 || lPacketSize > 16260)
                {
                    lErrMsg = "Packet size must be between 72 and 9000.";
                }
            }
            catch
            {
                lErrMsg = "Packet size must be between 72 and 9000.";
            }
            if (lErrMsg != "")
            {
                maxPacketSizeTextBox.Focus();
            }
            errorProvider.SetError(maxPacketSizeTextBox, lErrMsg);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Stop();

            DisconnectDevice(0, 0);
            DisconnectDevice(0, 1);
            DisconnectDevice(1, 0);
            DisconnectDevice(1, 1);
        }
    }
}

