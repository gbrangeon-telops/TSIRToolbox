// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PvDotNet;
using PvGUIDotNet;
using System.Threading;
using System.Diagnostics;

namespace MulticastMaster
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
            mDeviceControl.Owner = this;
            mCommunicationControl.Owner = this;
        }

        private const string cMulticastGroupIP = "239.192.1.1";
        private const UInt16 cMulticastGroupPort = 1042;
        private const UInt32 cPacketSize = 1440;

        private PvDeviceGEV mDevice = new PvDeviceGEV();
        private PvDeviceInfo mDI;
        private BrowserForm mDeviceControl = new BrowserForm();
        private BrowserForm mCommunicationControl = new BrowserForm();

        /// <summary>
        /// Connects and configures the device.
        /// </summary>
        /// <returns></returns>
        private void ConnectDevice()
        {
            Cursor lOldCursor = Cursor;

            // Shows the device finder form.
            PvDeviceFinderForm lPvDevFinder = new PvDeviceFinderForm();
            if (lPvDevFinder.ShowDialog(this) == DialogResult.OK)
            {
                // Get the device information.
                mDI = lPvDevFinder.Selected;

                PvDeviceInfoGEV lDIGEV = mDI as PvDeviceInfoGEV;
                if (lDIGEV == null)
                {
                    MessageBox.Show("This sample only supports GigE Vision devices.", Text);
                    return;
                }

                try
                {
                    Cursor = Cursors.WaitCursor;

                    // Connect to the selected device.
                    mDevice.Connect(mDI);
                    ipAddressTextBox.Text = lDIGEV.IPAddress;
                    macAddressTextBox.Text = lDIGEV.MACAddress;
                    manufacturerTextBox.Text = mDI.ManufacturerInfo;
                    modelTextBox.Text = mDI.ModelName;
                    nameTextBox.Text = mDI.UserDefinedName;

                    // Setting the the group of multicast IP address.
                    mDevice.SetStreamDestination(cMulticastGroupIP, cMulticastGroupPort);
                    multicastIPTextBox.Text = cMulticastGroupIP;
                    multicastPortTextBox.Text = cMulticastGroupPort.ToString();

                    // Setting the packet size, in multicast auto negotiation package size can not be done.
                    mDevice.Parameters.SetIntegerValue("GevSCPSPacketSize", cPacketSize);
                    packetSizeTextBox.Text = cPacketSize + " bytes";
                }
                catch (PvException lPvE)
                {
                    MessageBox.Show("Error connecting and configuring to the device: " + lPvE.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);

                    Close();
                }
                finally
                {
                    Cursor = lOldCursor;
                }
            }
            else
            {
                Close();
            }
        }

        /// <summary>
        /// Disconnects the device.
        /// </summary>
        /// <returns></returns>
        private bool DisconnectDevice()
        {
            if (!mDevice.IsConnected)
            {
                return false;
            }

            mDeviceControl.GenBrowserControl.GenParameterArray = null;
            CloseGenForm(mDeviceControl);

            mCommunicationControl.GenBrowserControl.GenParameterArray = null;
            CloseGenForm(mCommunicationControl);

            mDevice.Disconnect();

            return true;
        }

        /// <summary>
        /// Start acquisition on the device: enables stream and send AcquisitionStart command.
        /// </summary>
        /// <returns></returns>
        private bool StartAcquisition()
        {
            try
            {
                // Enables stream before sending the AcquisitionStart command.
                mDevice.StreamEnable();                    

                // Sending AcquisitionStart command to the GEV device to start acquisition.
                mDevice.Parameters.ExecuteCommand("AcquisitionStart");
            }
            catch (PvException lPvE)
            {
                MessageBox.Show("Error starting acquisition: " + lPvE.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Stops acquisition on the device. sends AcquisitionStop command and disables stream..
        /// </summary>
        /// <returns></returns>
        private bool StopAcquisition()
        {
            if (!mDevice.IsConnected)
            {
                return false;
            }
            try
            {
                // Disable stream after sending the AcquisitionStop command.
                mDevice.StreamDisable();

                // Sending AcquisitionStop command the GEV device to stop acquisition.
                mDevice.Parameters.ExecuteCommand("AcquisitionStop");
            }
            catch (PvException lPvE)
            {
                MessageBox.Show("Error stopping acquisition: " + lPvE.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Show GenICam form.
        /// </summary>
        /// <param name="aForm"></param>
        /// <param name="aParameters"></param>
        /// <param name="aTitle"></param>
        private void ShowGenForm(BrowserForm aForm, PvGenParameterArray aParameters, string aTitle)
        {
            if (aForm.Visible == true)
            {
                CloseGenForm(aForm);
                return;
            }
            aForm.Text = aTitle;
            aForm.GenBrowserControl.GenParameterArray = aParameters;
            aForm.Show();
        }

        /// <summary>
        /// Closes a GenICam form.
        /// </summary>
        /// <param name="aBrowserForm"></param>
        private void CloseGenForm(Form aBrowserForm)
        {
            aBrowserForm.Hide();            
        }

        /// <summary>
        /// Play button handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnPlay_Click(object sender, EventArgs e)
        {
            playButton.Enabled = false;
            if (StartAcquisition() == true)
            {
                deviceToolStripMenuItem.Enabled = true;
                stopButton.Enabled = true;
            }
            else
            {
                playButton.Enabled = true;
            }
        }

        /// <summary>
        /// Stop button handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStop_Click_1(object sender, EventArgs e)
        {
            if (StopAcquisition() == true)
            {  
                //Close all configuration child windows.
                playButton.Enabled = true;
                stopButton.Enabled = false;
                deviceToolStripMenuItem.Enabled = false;
            }
        }

        /// <summary>
        /// Form closing event. Disconnects the device.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            stopButton.PerformClick();
            DisconnectDevice();
        }

        /// <summary>
        /// Exit menu handler.
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

        /// <summary>
        /// Device parameters menu handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void deviceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenForm(mDeviceControl, mDevice.Parameters, "Device Control");
        }

        /// <summary>
        /// Communication control menu handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void communicationCOntrolToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowGenForm(mCommunicationControl, mDevice.CommunicationParameters, "Communication Control");
        }

        /// <summary>
        /// Main form load handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            ConnectDevice();
        }

        /// <summary>
        /// Make the tools menu items enabled or not
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void parametersSettingToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            deviceToolStripMenuItem.Enabled = mDevice.IsConnected;
            communicationCOntrolToolStripMenuItem.Enabled = true;
        }
       
    }
}