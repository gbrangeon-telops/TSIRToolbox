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

namespace MulticastSlave
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private const string cMulticastGroupIP = "239.192.1.1";
        private const UInt16 cMulticastGroupPort = 1042;

        private PvStreamGEV mStream = new PvStreamGEV();
        private PvPipeline mPipeline = null;
        private string mIPAddress = "";
        private bool mStopReceiveBufferThread = false;
        private Thread mThread = null;

        private BrowserForm mBrowserForm = new BrowserForm();

        // Method to select the device to receive the data.
        private bool SelectDevice()
        {
            // Instantiates the PvDeviceFinderForm object.
            PvDeviceFinderForm lPvDeviceFinder = new PvDeviceFinderForm();
            // Shows PvDeviceFinderForm 
            if (lPvDeviceFinder.ShowDialog(this) == DialogResult.OK)
            {
                PvDeviceInfoGEV lDeviceInfoGEV = lPvDeviceFinder.Selected as PvDeviceInfoGEV;
                if (lDeviceInfoGEV == null)
                {
                    MessageBox.Show("This sample only supports GigE Vision devices.", Text);
                    return false;
                }

                mIPAddress = lDeviceInfoGEV.IPAddress;
            }
            else
            {
                Close();
            }
            return true;
        }

        /// <summary>
        /// Start streaming.
        /// </summary>
        private void StartStreaming()
        {
            if (!SelectDevice())
            {
                Close();
            }

            mThread = new Thread(DoThreadWork);
            mStopReceiveBufferThread = false;

            try
            {
                // Opens the stream of the group of multicast IP address 239.192.1.1, port 1024.
                mStream.Open(mIPAddress, cMulticastGroupIP, cMulticastGroupPort);

                // If the RequestMissingPackets feature is available, disable it.
                PvGenBoolean lRequestMissingPackets = mStream.Parameters.GetBoolean("RequestMissingPackets");
                if (lRequestMissingPackets != null)
                {
                    bool lValue = true;
                    if (lRequestMissingPackets.TryGetValue(ref lValue).IsOK)
                    {
                        lRequestMissingPackets.Value = false;
                    }       
                }  

                // Initializes pipeline
                mPipeline = new PvPipeline(mStream);
                mPipeline.OnBufferTooSmall += new OnBufferTooSmall(OnBufferTooSmall);

                // Initializes buffer count for pipeline. To eliminate missing block IDs, increasing the buffer count.
                mPipeline.BufferCount = 16;

                // Starts pipeline.
                mPipeline.Start();
                statusControl.Stream = mStream;

                // Starts thread to retrieve data and display on the control display.
                mThread.Start();

                // Update window title.
                Text = "MulticastSlave - Multicast Group " + cMulticastGroupIP + " - Port " + cMulticastGroupPort.ToString();
            }
            catch (PvException lPvE)
            {
                MessageBox.Show(lPvE.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            streamToolStripMenuItem.Enabled = true;
        }

        /// <summary>
        /// Event triggered by the pipeline when a buffer too small error is encountered.
        /// We let the pipeline know that we want all buffers to be reallocated immediately
        /// and raise the buffer reallocated warning in the status control.
        /// </summary>
        /// <param name="aPipeline"></param>
        /// <param name="aReallocAll"></param>
        /// <param name="aResetStats"></param>
        void OnBufferTooSmall(PvPipeline aPipeline, ref bool aReallocAll, ref bool aResetStats)
        {
            aReallocAll = true;
            aResetStats = true;

            statusControl.BuffersReallocated = true;
        }

        /// <summary>
        /// Stops streaming.
        /// </summary>
        private void StopStreaming()
        {
            // Signal the thread to stop retrieving data.
            mStopReceiveBufferThread = true;

            // Main thread sleeps for 1ms so the worker thread can finish its tasks.
            if (mThread != null)
            {
                // Blocks the calling thread until the thread terminated.
                mThread.Join();
            }

            // Stop pipeline and close stream.
            StopPipeLine();

            mThread = null;
        }

        /// <summary>
        /// Stops the pipeline.
        /// </summary>
        private void StopPipeLine()
        {
            if (mPipeline == null)
            {
                // Pipe line is not initialized yet so just return.
                return;
            }
            try
            {
                if (mPipeline.IsStarted)
                {
                    // Stop pipeline.
                    mPipeline.Stop();
                }

                if (mStream.IsOpen)
                {
                    // Close stream.
                    mStream.Close();
                }
            }
            catch (PvException lPvE)
            {
                MessageBox.Show(lPvE.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }  
        }

        /// <summary>
        /// Display thread.
        /// </summary>
        private void DoThreadWork()
        {
            PvBuffer lPvBuffer = null;
            while (mStopReceiveBufferThread == false)
            {
                // Retrieve the buffer.
                PvResult lPvResult = mPipeline.RetrieveNextBuffer(ref lPvBuffer, 100);
                if (lPvResult.IsOK)
                {
                    if (lPvBuffer.OperationResult.IsOK)
                    {
                        // Process the image in the PvBuffer.
                        // ....

                        // Displays the image.
                        pvDisplayControl1.Display(lPvBuffer);
                    }

                    // Release buffer to PvPipeline.
                    mPipeline.ReleaseBuffer(lPvBuffer);
                }    
            }           
        }
        
        /// <summary>
        /// Form closing menu handler. Just stop streaming.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Stop pipeline and close stream.
            StopStreaming();
        }

        /// <summary>
        /// Exit menu handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult lDR = MessageBox.Show("Are you sure you want to exit the application?", Text, 
                MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (lDR == DialogResult.Yes)
            {
                Close();
            }
        }

        /// <summary>
        /// Stream parameters menu handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void streamToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mBrowserForm.Visible == true)
            {
                mBrowserForm.Hide();
                return;
            }
            mBrowserForm.Text = "Stream Control";
            mBrowserForm.GenBrowserControl.GenParameterArray = mStream.Parameters;
            mBrowserForm.Show();
        }

        /// <summary>
        /// Main form load handler.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            StartStreaming();
        }

    }
}