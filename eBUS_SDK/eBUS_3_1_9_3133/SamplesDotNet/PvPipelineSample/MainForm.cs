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


namespace PvPipelineSample
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private PvDevice mDevice = new PvDevice();
        private PvStream mStream = new PvStream();
        private PvPipeline mPipeline = null;

        private Thread mThread = null;
        private bool mIsStopping = false;
        private int mStep = 1;

        private void MainForm_Load(object sender, EventArgs e)
        {
            timer.Start();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (mPipeline.IsStarted)
            {
                Step5StoppingStream();
            }

            if (mDevice.IsConnected)
            {
                Step6Disconnecting();
            }
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            mStep++;
            timer.Start();
        }

        private void Step1Connecting()
        {
            oneLabel.Enabled = true;

            // Pop device finder, let user select a device
            PvDeviceFinderForm lForm = new PvDeviceFinderForm();
            if (lForm.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    // Connect device
                    mDevice.Connect(lForm.Selected);

                    // Open stream
                    mStream.Open(lForm.Selected.IPAddress);

                    // Set browser parameters
                    browser.GenParameterArray = mStream.Parameters;

                    // Create pipeline
                    mPipeline = new PvPipeline(mStream);
                }
                catch (PvException ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Close();
                }
            }
            else
            {
                Close();
            }
        }

        private void Step2Configuring()
        {
            oneLabel.Enabled = false;
            twoLabel.Enabled = true;

            try
            {
                // Negotiate packet size. Failure means default value as configured on 
                // the device is used.
                mDevice.NegotiatePacketSize();
            }
            catch (PvException ex)
            {
                MessageBox.Show(ex.Message, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            // Set stream destination
            mDevice.SetStreamDestination(mStream.LocalIPAddress, mStream.LocalPort);

            // Read payload size, pre-allocate buffers of the pipeline
            Int64 lPayloadSize = mDevice.GenParameters.GetIntegerValue("PayloadSize");
            mPipeline.BufferSize = (UInt32)lPayloadSize;

            // Set buffer count. Use more buffers (at expense of using more memory) to 
            // eleminate missing block IDs
            mPipeline.BufferCount = 16;
        }

        private void Step3StartingStream()
        {
            twoLabel.Enabled = false;
            threeLabel.Enabled = true;

            // Start (arm) the pipeline
            mPipeline.Start();

            // Start display thread
            mThread = new Thread(new ParameterizedThreadStart(ThreadProc));
            MainForm lP1 = this;
            object[] lParameters = new object[] { lP1 };
            mIsStopping = false;
            mThread.Start(lParameters);

            // Set TLParamsLocked to 1
            mDevice.GenParameters.SetIntegerValue("TLParamsLocked", 1);

            // Start acquisition on the device
            mDevice.GenParameters.ExecuteCommand("AcquisitionStart");
        }

        private void Step4Streaming()
        {
            threeLabel.Enabled = false;
            fourLabel.Enabled = true;
            stopButton.Enabled = true;
        }

        private void Step5StoppingStream()
        {
            stopButton.Enabled = false;
            fourLabel.Enabled = false;
            fiveLabel.Enabled = true;

            // Stop acquisition
            mDevice.GenParameters.ExecuteCommand("AcquisitionStop");

            // Release TLParamsLocked
            mDevice.GenParameters.SetIntegerValue("TLParamsLocked", 0);

            // Stop display thread
            mIsStopping = true;
            mThread.Join();
            mThread = null;

            // Stop the pipeline
            mPipeline.Stop();
        }

        private void Step6Disconnecting()
        {
            fiveLabel.Enabled = false;
            sixLabel.Enabled = true;

            // Release browser parameters reference
            browser.GenParameterArray = null;

            // Close stream
            mStream.Close();

            // Disconnect device
            mDevice.Disconnect();
        }

        private static void ThreadProc(object aParameters)
        {
            object[] lParameters = (object[])aParameters;
            MainForm lThis = (MainForm)lParameters[0];

            PvBuffer lBuffer = null;

            for (; ; )
            {
                if (lThis.mIsStopping)
                {
                    // Signaled to terminate thread, return
                    return;
                }

                // Retrieve next buffer from acquisition pipeline
                PvResult lResult = lThis.mPipeline.RetrieveNextBuffer(ref lBuffer);
                if (lResult.IsOK)
                {
                    // Operation result of buffer is OK, display
                    if (lBuffer.OperationResult.IsOK)
                    {
                        lThis.displayControl.Display(lBuffer);
                    }

                    // We got a buffer (good or not) we must release it back
                    lThis.mPipeline.ReleaseBuffer(lBuffer);
                }
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            timer.Stop();
            switch (mStep)
            {
                case 1:
                    Step1Connecting();
                    break;

                case 2:
                    Step2Configuring();
                    break;

                case 3:
                    Step3StartingStream();
                    break;

                case 4:
                    Step4Streaming();
                    return;

                case 5:
                    Step5StoppingStream();
                    break;

                case 6:
                    Step6Disconnecting();
                    break;

                default:
                    Close();
                    return;
            }

            mStep++;
            timer.Start();
        }
    }
}

