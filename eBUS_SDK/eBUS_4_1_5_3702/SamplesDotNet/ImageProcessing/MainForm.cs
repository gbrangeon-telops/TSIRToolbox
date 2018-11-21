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


namespace ImageProcessing
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
        private UInt32 mBufferCount;
        List<PvBuffer> mBuffers = new List<PvBuffer>();
        Dictionary<UInt64, Bitmap> mBitmaps = new Dictionary<UInt64, Bitmap>();
        Dictionary<UInt64, System.Drawing.Imaging.BitmapData> mBitmapData = new Dictionary<UInt64, System.Drawing.Imaging.BitmapData>();
        PvBuffer[] mConvertedBuffers;

        int mWidth = 1280;
        int mHeight = 720;

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
                    PvDeviceInfo lDeviceInfo = lForm.Selected as PvDeviceInfo;
                    PvDeviceInfoGEV lDeviceInfoGEV = lDeviceInfo as PvDeviceInfoGEV;
                    PvDeviceInfoU3V lDeviceInfoU3V = lDeviceInfo as PvDeviceInfoU3V;

                    // Connect device.
                    mDevice = PvDevice.CreateAndConnect(lDeviceInfo);
                   
                    // Open stream.
                    mStream = PvStream.CreateAndOpen(lDeviceInfo.ConnectionID);
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
                Int64 lPayloadSize = mDevice.Parameters.GetIntegerValue("PayloadSize");

                // Read width, height to properly allocate our buffers
                mWidth = (int)mDevice.Parameters.GetIntegerValue("Width");
                mHeight = (int)mDevice.Parameters.GetIntegerValue("Height");

                // Older devices are using RGB8Packed, newer devices RGB8
                bool lFound = false;
                PvGenEnum lPixelFormat = mDevice.Parameters.GetEnum("PixelFormat");
                foreach (PvGenEnumEntry lEE in lPixelFormat)
                {
                    string lValue = lEE.ValueString;
                    if ((lValue == "RGB8Packed") ||
                        (lValue == "RGB8"))
                    {
                        lPixelFormat.ValueString = lValue;
                        lFound = true;
                        break;
                    }
                }

                if (!lFound)
                {
                    MessageBox.Show("This device does not support the RGB8 pixel type which is required by this sample.");

                    Close();
                    return;
                }

                // Get minimum buffer count, creates and allocates buffer.
                mBufferCount = (mStream.QueuedBufferMaximum < cBufferCount) ? mStream.QueuedBufferMaximum : cBufferCount;
                mConvertedBuffers = new PvBuffer[mBufferCount];
                for (UInt32 i = 0; i < mBufferCount; i++)
                {
                    PvBuffer lBuffer = new PvBuffer();
                    mBuffers.Add(lBuffer);
                    AttachRawBuffer(lBuffer);
                }

                // Queue all buffers in the stream.
                foreach (PvBuffer lBuffer in mBuffers)
                {
                    mStream.QueueBuffer(lBuffer);
                }
            }
            catch (PvException ex)
            {
                Step6Disconnecting();
                MessageBox.Show(ex.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                Close();
            }
        }

        private unsafe void AttachRawBuffer(PvBuffer aBuffer)
        {
            // The bitmap is using the same size and pixel type as what the device is sending
            Bitmap lBitmap = new Bitmap(mWidth, mHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            // We lock the memory before attaching it to the PvBuffer - and keep it locked for 
            // as long as the PvBuffer is held by the PvPipeline/PvStream
            System.Drawing.Imaging.BitmapData lBmpData = lBitmap.LockBits(new Rectangle(0, 0, mWidth, mHeight),
                System.Drawing.Imaging.ImageLockMode.ReadWrite,
                lBitmap.PixelFormat);

            // Attach the bitmap pointer to the PvBuffer
            aBuffer.Image.Attach((byte*)lBmpData.Scan0.ToPointer(), (uint)lBmpData.Width, (uint)lBmpData.Height, PvPixelType.RGB8);

            // Save bitmap and bitmap data in map, indexed by the buffer ID
            UInt64 lIndex = (UInt64)mBitmaps.Count;
            mBitmaps[lIndex] = lBitmap;
            mBitmapData[lIndex] = lBmpData;
            aBuffer.ID = lIndex;
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
                if (mThread != null)
                {
                    mThread.Join();
                    mThread = null;
                }
               
                // Abort all buffers in the stream and dequeue
                mStream.AbortQueuedBuffers();
                while (mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult).IsOK);
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

        private unsafe static void ThreadProc(object aParameters)
        {
            object[] lParameters = (object[])aParameters;
            MainForm lThis = (MainForm)lParameters[0];

            ImageProcessing.ColorMatrix lColorMatrix = CreateBrightnessColorMatrix(128.0f);

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
                        PvImage lImage = lBuffer.Image;

                        // Get bitmap, locked bitmap data from buffer ID
                        Bitmap lBitmap = lThis.mBitmaps[lBuffer.ID];
                        System.Drawing.Imaging.BitmapData lBitmapData = lThis.mBitmapData[lBuffer.ID];

                        // Unlock bits for the time we work on the bitmap
                        lBitmap.UnlockBits(lBitmapData);

                        // With our simple imaging library, work on bitmap data
                        lColorMatrix.Apply(lBitmap);

                        // Lock the bitmap data again before using the PvBuffer with the display, re-queue it in the PvStream
                        lThis.mBitmapData[lBuffer.ID] = lBitmap.LockBits(new Rectangle(0, 0, lThis.mWidth, lThis.Height), 
                            System.Drawing.Imaging.ImageLockMode.ReadWrite, lBitmap.PixelFormat);

                        // Re-attach the buffer. It is not safe to assume we are getting the same pointer back
                        lImage.Attach((byte*)lThis.mBitmapData[lBuffer.ID].Scan0.ToPointer(), 
                            lImage.Width, lImage.Height, lImage.PixelType);

                        lThis.displayControl.Display(lBuffer);
                    }

                    // We have an image - do some processing (...) and VERY IMPORTANT,
                    // re-queue the buffer in the stream object.
                    lThis.mStream.QueueBuffer(lBuffer);
                }
            }
        }

        private static ImageProcessing.ColorMatrix CreateBrightnessColorMatrix( float aBrightness )
        {
            float lFinalValue = aBrightness / 255.0f;

            ImageProcessing.ColorMatrix lColorMatrix = new ImageProcessing.ColorMatrix();
            lColorMatrix.Matrix = new float[][]
            {
                new float[]{1,0,0,0,0},
                new float[]{0,1,0,0,0},
                new float[]{0,0,1,0,0},
                new float[]{0,0,0,1,0},
                new float[]{lFinalValue,lFinalValue,lFinalValue,1,1},
            };

            return lColorMatrix;
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


