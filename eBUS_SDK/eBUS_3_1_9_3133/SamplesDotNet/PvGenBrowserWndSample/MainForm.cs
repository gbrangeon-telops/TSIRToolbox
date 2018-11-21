// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
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


namespace PvGenBrowserWndSample
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            // Communication parameters are alaways available, even if not connected
            communicationBrowser.GenParameterArray = mDevice.GenLink;
        }

        PvDevice mDevice = new PvDevice();

        private void connectButton_Click(object sender, EventArgs e)
        {
            // Select the device
            PvDeviceFinderForm lForm = new PvDeviceFinderForm();
            DialogResult lDR = lForm.ShowDialog();
            if ((lDR != DialogResult.OK) || (lForm.Selected == null))
            {
                return;
            }

            Cursor lOldCursor = Cursor;
            Cursor = Cursors.WaitCursor;

            try
            {
                // Connect device
                mDevice.Connect(lForm.Selected);

                // Assign device parameters to browser
                deviceBrowser.GenParameterArray = mDevice.GenParameters;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }
            finally
            {
                Cursor = lOldCursor;
            }

            connectButton.Enabled = false;
            disconnectButton.Enabled = true;
        }

        private void Disconnect()
        {
            if (!mDevice.IsConnected)
            {
                return;
            }

            try
            {
                // Release device parameters from browser
                deviceBrowser.GenParameterArray = null;

                // Disconnect device
                mDevice.Disconnect();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }

            connectButton.Enabled = true;
            disconnectButton.Enabled = false;
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            Disconnect();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Disconnect();
        }
    }
}

