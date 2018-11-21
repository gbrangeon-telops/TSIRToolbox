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
using System.Threading;
using PvDotNet;
using PvGUIDotNet;


namespace PvStreamSample
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private const UInt16 cBufferCount = 16;

        private PvDevice mDevice = null;
        private PvStream mStream = null;

        private Thread mThread = null;
        private bool mIsStopping = false;
        private int mStep = 1;

        private void MainForm_Load(object sender, EventArgs e)
        {
            timer.Start();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (mDevice == null || mStream == null)
            {
                return;
            }

            if (mStream.IsOpen)
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

            // Pop device finder, let user select a device.
            PvDeviceFinderForm lForm = new PvDeviceFinderForm();
            if ((lForm.ShowDialog() == DialogResult.OK) && (lForm.Selected != null))
            {
                try
                {
                    PvDeviceInfo lDeviceInfo = lForm.Selected;

                    // Connect device.
                    mDevice = PvDevice.CreateAndConnect(lDeviceInfo);

                    // Open stream.
                    mStream = PvStream.CreateAndOpen(lDeviceInfo);
                    if (mStream == null)
                    {
                        MessageBox.Show("Unable to open stream.", Text);
                        return;
                    }

                    // Set browser parameters.
                    browser.GenParameterArray = mStream.Parameters;
                }
                catch (PvException ex)
                {
                    Step6Disconnecting();
                    MessageBox.Show(ex.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
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
                // Perform GigE Vision only configuration
                PvDeviceGEV lDGEV = mDevice as PvDeviceGEV;
                if (lDGEV != null)
                {
                    // Negotiate packet size
                    lDGEV.NegotiatePacketSize();

                    // Set stream destination.
                    PvStreamGEV lSGEV = mStream as PvStreamGEV;
                    lDGEV.SetStreamDestination(lSGEV.LocalIPAddress, lSGEV.LocalPort);
                }

                // Read payload size, preallocate buffers of the pipeline.
                Int64 lPayloadSize = mDevice.PayloadSize;

                // Get minimum buffer count, creates and allocates buffer.
                UInt32 lBufferCount = (mStream.QueuedBufferMaximum < cBufferCount) ? mStream.QueuedBufferMaximum : cBufferCount;
                PvBuffer[] lBuffers = new PvBuffer[lBufferCount];
                for (UInt32 i = 0; i < lBufferCount; i++)
                {
                    lBuffers[i] = new PvBuffer();
                    lBuffers[i].Alloc((UInt32)lPayloadSize);
                }

                // Queue all buffers in the stream.
                for (UInt32 i = 0; i < lBufferCount; i++)
                {
                    mStream.QueueBuffer(lBuffers[i]);
                }
            }
            catch (PvException ex)
            {
                Step6Disconnecting();
                MessageBox.Show(ex.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                Close();
            }
        }

        private void Step3StartingStream()
        {
            twoLabel.Enabled = false;
            threeLabel.Enabled = true;

            // Start display thread.
            mThread = new Thread(new ParameterizedThreadStart(ThreadProc));
            MainForm lP1 = this;
            object[] lParameters = new object[] { lP1 };
            mIsStopping = false;
            mThread.Start(lParameters);

            // Enables streaming before sending the AcquisitionStart command.
            mDevice.StreamEnable();

            // Start acquisition on the device
            mDevice.Parameters.ExecuteCommand("AcquisitionStart");
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

            try
            {
                PvBuffer lBuffer = null;
                PvResult lOperationResult = new PvResult(PvResultCode.OK);
                PvResult lResult = new PvResult(PvResultCode.OK);

                // Stop acquisition.
                mDevice.Parameters.ExecuteCommand("AcquisitionStop");

                // Disable streaming after sending the AcquisitionStop command.
                mDevice.StreamDisable();

                // Stop display thread.
                mIsStopping = true;
                mThread.Join();
                mThread = null;

                // Abort all buffers in the stream and dequeue
                mStream.AbortQueuedBuffers();
                for (int i = 0; i < mStream.QueuedBufferCount; i++)
                {
                    lResult = mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult);
                    if (lResult.IsOK)
                    {
                        lBuffer = null;
                    }
                }
            }
            catch (PvException lExc)
            {
                MessageBox.Show(lExc.Message, Text);
            }
        }

        private void Step6Disconnecting()
        {
            fiveLabel.Enabled = false;
            sixLabel.Enabled = true;

            // Release browser parameters reference.
            browser.GenParameterArray = null;

            if (mStream != null)
            {
                // Close and release stream
                mStream.Close();
                mStream = null;
            }
            
            if (mDevice != null)
            {
                // Disconnect and release device
                mDevice.Disconnect();
                mDevice = null;
            }
        }

        private static void ThreadProc(object aParameters)
        {
            object[] lParameters = (object[])aParameters;
            MainForm lThis = (MainForm)lParameters[0];

            for (;;)
            {
                if (lThis.mIsStopping)
                {
                    // Signaled to terminate thread, return.
                    return;
                }

                PvBuffer lBuffer = null;
                PvResult lOperationResult = new PvResult(PvResultCode.OK);
                
                // Retrieve next buffer from acquisition pipeline
                PvResult lResult = lThis.mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult, 100);
                if (lResult.IsOK)
                {
                    // Operation result of buffer is OK, display.
                    if (lOperationResult.IsOK)
                    {
                        lThis.displayControl.Display(lBuffer);
                    }

                    // We have an image - do some processing (...) and VERY IMPORTANT,
                    // re-queue the buffer in the stream object.
                    lThis.mStream.QueueBuffer(lBuffer);
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


