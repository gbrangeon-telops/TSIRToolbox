// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Collections;
using System.IO;
using PvDotNet;
using PvGUIDotNet;


namespace TransmitTiledImages
{
    // Stats information
    /// <summary>
    /// Transmit statistic information.
    /// </summary>
    public struct TransmitterStats
    {
        public UInt64 BlocksTransmitted;
        public Single InstantaneousTransmissionRate;
        public Single InstantaneousPayloadThroughput;
        public UInt32 DisplayFrameRate;
    }

    /// <summary>
    /// Thread to manage the transmission.
    /// </summary>
    class TransmitterThread
    {
        public const UInt16 cBufferPoolSize = 8;
        public const UInt32 cMaxTileRows = 2;
        public const UInt32 cMaxTileColumns = 2;

        public TransmitterThread()
        {
            mPeriod = 0;
            mNextExpectedStop = 0;
            mDisplayRate.Reset();

            mBitmaps[0, 0] = null;
            mBitmaps[0, 1] = null;
            mBitmaps[1, 0] = null;
            mBitmaps[1, 1] = null;
        }

        private Thread mThread = null;

        // Flag is used to start stop thread works.
        private bool mStopTransmit = false;

        // Period to respect
        private Double mPeriod;

        // Next expected period elapsing
        private Double mNextExpectedStop;

        // Object that keep track of the last buffer for each stream.
        private CurrentBuffersTable mCurrentBuffersTable = null;

        // Current Transmitter configurated.
        private TransmitterConfig mTransmitterRawConfig = null;

        // Transmitter engine
        private PvTransmitterGEV mTransmitterRaw = new PvTransmitterGEV();

        // Display control
        private PvDisplayControl mDisplay = null;

        // Used to keep track of the display frame rate
        private PvFPSStabilizer mDisplayRate = new PvFPSStabilizer();

        // .NET Bitmap objects (intermediate buffers)
        private Bitmap[,] mBitmaps = new Bitmap[2, 2];

        // Object to be used in critical section. 
        private Object mTransmitterObject = new Object();

        /// <summary>
        /// Return the time to reach the period.
        /// </summary>
        /// <returns>The time to wait in ms</returns>
        private UInt64 WaitTime()
        {
            Double lRemainingTime;

            lRemainingTime = mNextExpectedStop - Environment.TickCount;
            if ( lRemainingTime > 1.0F )
            {
                return (UInt64)lRemainingTime;
            }

            return 0;
        }
        
        /// <summary>
        /// Convert image form PvImage to .NET Image, processed, tiled the image and then convert .NET Image to PvImage to display on PvDisplayCOntrol.
        /// </summary>
        /// <param name="aItem">TransmitItem object to hold a PvBuffer and the Bitmap it is attached to.</param>
        private void ConvertImage(TransmitItem aItem) 
        {
            // Graphics object
            using (Graphics lGraphics = Graphics.FromImage(aItem.Bitmap))
            {
                lGraphics.Clear(Color.Black);
                for (int i = 0; i < cMaxTileRows; i++)
                {
                    for (int j = 0; j < cMaxTileColumns; j++)
                    {
                        // Get snapshot PvBuffer.
                        PvBuffer lSrcBuffer = mCurrentBuffersTable.GetBufferFromSnapshot(i, j);
                        if ((lSrcBuffer != null) && (lSrcBuffer.PayloadType == PvPayloadType.Image))
                        {
                            int lWidth = (int)lSrcBuffer.Image.Width;
                            int lHeight = (int)lSrcBuffer.Image.Height;

                            // Get and adjust (if needed) bitmap
                            Bitmap lBitmap = mBitmaps[i, j];
                            if ((lBitmap == null) || (lBitmap.Width != lWidth) || (lBitmap.Height != lHeight))
                            {
                                // Only reallocate the bitmap when the input size changes
                                lBitmap = new Bitmap(lWidth, lHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                                mBitmaps[i, j] = lBitmap;
                            }

                            // Fill (convert) bitmap using PvBuffer
                            lSrcBuffer.Image.CopyToBitmap(lBitmap);

                            // Copy to destination bitmap
                            lGraphics.DrawImage(lBitmap, GetDestRect(lSrcBuffer.Image, i, j), GetSrcRect(lSrcBuffer.Image), GraphicsUnit.Pixel);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// To transmit images.
        /// </summary>
        private void DoTransmitDataWork()
        { 
            UInt64 lWaitTime = 0;
            PvBuffer lBuffer = null;

            // Reset display rate
            mDisplayRate.Reset();

            // Get the next expected stop time
            mNextExpectedStop = (double)Environment.TickCount + mPeriod;

            // Buffers used for transmission
            TransmitItem[] lBuffers = new TransmitItem[cBufferPoolSize];
            Queue<TransmitItem> lFreeBufferQueue = new Queue<TransmitItem>();
            for (int i = 0; i < cBufferPoolSize; i++)
            {
                lBuffers[i] = new TransmitItem(i, (int)mTransmitterRawConfig.Width, (int)mTransmitterRawConfig.Height);
                lFreeBufferQueue.Enqueue(lBuffers[i]);
            }

            while (!mStopTransmit)
            {
                if (lFreeBufferQueue.Count > 0)
                {
                    // If free buffers are available, get one
                    TransmitItem lItem = lFreeBufferQueue.Dequeue();

                    mNextExpectedStop += mPeriod;

                    // First we get a snapshot from the current buffer table
                    mCurrentBuffersTable.LockSnapshot();

                    // Convert the buffers into the tiling format
                    ConvertImage(lItem);

                    // Release snapshot
                    mCurrentBuffersTable.UnlockSnapshot();

                    // Attach bitmap content to PvBuffer
                    lItem.Attach();

                    // Transmit the image over the network
                    mTransmitterRaw.QueueBuffer(lItem.Buffer);
                }

                // Check if any buffer has been transmitted properly and can be retrieve
                // This is a while loop to ensure we can be late and recover if needed
                lWaitTime = WaitTime();

                if (mTransmitterRaw.QueuedBufferCount <= 0)
                {
                    Thread.Sleep(1);
                    continue;
                }

                for (;;)
                {
                    PvResult lResult = mTransmitterRaw.RetrieveFreeBuffer(ref lBuffer, (UInt32)lWaitTime);
                    if (lResult.Code == PvResultCode.TIMEOUT)
                    {
                        break;
                    }

                    if (lResult.IsOK && (lBuffer != null))
                    {
                        // If we need to display the image to reach target display FPS, do it
                        if ((lBuffer.OperationResult.IsOK) && mDisplayRate.IsTimeToDisplay(60))
                        {
                            mDisplay.Display(lBuffer);
                        }

                        // Retrieve transmit item (using buffer ID for list index)
                        TransmitItem lFreeItem = lBuffers[(int)lBuffer.ID];

                        // Detach PvBuffer from bitmap content
                        lFreeItem.Detach();

                        // Push transmit item back on free stack
                        lFreeBufferQueue.Enqueue(lFreeItem);

                        lBuffer = null;
                    }
                    else
                    {
                        Thread.Sleep(1);
                    }

                    // Get the next amount of ms too sleep
                    lWaitTime = WaitTime();
                    if (lWaitTime == 0)
                    {
                        break;
                    }
                }
            }

            try
            {
                // Aborts all pending buffers.
                mTransmitterRaw.AbortQueuedBuffers();

                // Retrieve the free buffer and release it.
                while (mTransmitterRaw.RetrieveFreeBuffer(ref lBuffer, 0).IsOK)
                {
                }

                // Close the transmitter
                mTransmitterRaw.Close();
            }
            catch
            {
            }

            // Dispose of buffers, too big to wait on garbage collector
            foreach (TransmitItem lItem in lBuffers)
            {
                lItem.Dispose();
            }
        }

        /// <summary>
        /// Configure the TransmitterThread.
        /// </summary>
        /// <param name="aTransmitterConfig">TransmitThread configuration information.</param>
        /// <param name="aMaxWidth">Maximun source image width..</param>
        /// <param name="aMaxHeight">Maximun source image height.</param>
        /// <returns>True - Configured the TransmitterThread successfully, otherwise false;</returns>
        /// 
        public bool Configure(TransmitterConfig aTransmitterConfig, Int32 aMaxWidth, Int32 aMaxHeight)
        {
            bool lResult = true;

            // Transmit configuration.
            mTransmitterRawConfig = aTransmitterConfig;

            try
            {
                // Open the transmitter connection
                if ((mTransmitterRawConfig.DestinationIPAddress != mTransmitterRaw.DestinationIPAddress) || 
                    (mTransmitterRawConfig.SourceIPAddress != mTransmitterRaw.SourceIPAddress) ||
                    (mTransmitterRaw.DestinationPort != mTransmitterRawConfig.DestinationPort))
                {
                    mTransmitterRaw.Close();

                    // Sets packet size
                    mTransmitterRaw.PacketSize = mTransmitterRawConfig.PacketSize;

                    // Initializes a socket to begin transmitting data.
                    mTransmitterRaw.Open(mTransmitterRawConfig.DestinationIPAddress, mTransmitterRawConfig.DestinationPort, mTransmitterRawConfig.SourceIPAddress, 0);
                }
                // Reset the statistic
                mTransmitterRaw.ResetStats();
            }
            catch
            {
                lResult = false;
            }
            return lResult;
        }
      
        /// <summary>
        /// Initialize the transmitter memory class. Must be called once at the beginning.
        /// </summary>
        /// <param name="aPvDisplay">PvDisplayControl object.</param>
        /// <param name="aCurrentBuffersTable">CurrentBuffersTable object.</param>
        /// <returns>True - Initialized transmitter memmory class sucessfully, otherwise false.</returns>
        public bool Initialize(PvDisplayControl aPvDisplay, ref CurrentBuffersTable aCurrentBuffersTable)
        {
            if (aPvDisplay == null || aCurrentBuffersTable == null)
            {
                return false;
            }

            mDisplay = aPvDisplay;
            mCurrentBuffersTable = aCurrentBuffersTable;     

            return true;
        }

        /// <summary>
        /// Start the TransmitThread
        /// </summary>
        public void Start()
        {
            mPeriod = 1000.0 / (Double)mTransmitterRawConfig.Fps;

            mThread = new Thread(DoTransmitDataWork);

            //Signal to start transmit data.
            mStopTransmit = false;
            mThread.Start();
        }

        /// <summary>
        /// Stop the TransmitThread
        /// </summary>
        /// <returns>True - Stopped the TransmitThread sucessfully, otherwise false.</returns>
        public bool Stop()
        {
            bool lResult = true;            
            if (mTransmitterRaw.IsOpened == false)
            {
                return lResult;
            }

            // Signal to stop transmit data.
            mStopTransmit = true;
            if (mThread != null)
            {
                // Blocks the calling thread untils the thread terminated
                mThread.Join();
                mThread = null;
            }

            return lResult;
        }

        /// <summary>
        /// Get the transmit statistic informations.
        /// </summary>
        /// <param name="aStats">Transmit statistic informations.</param>
        public void GetStats( ref TransmitterStats aStats )
        {
            aStats.BlocksTransmitted = mTransmitterRaw.BlocksTransmitted;
            aStats.InstantaneousTransmissionRate = (Single)mTransmitterRaw.InstantaneousTransmissionRate;
            aStats.InstantaneousPayloadThroughput = (Single)(mTransmitterRaw.InstantaneousPayloadThroughput / 1000000.0f);
            aStats.DisplayFrameRate = mDisplayRate.Average;
        }

        /// <summary>
        /// Returns the destination rectangle in the destination image for row, column
        /// </summary>
        /// <param name="aImage"></param>
        /// <param name="aRow"></param>
        /// <param name="aColumn"></param>
        /// <returns></returns>
        private Rectangle GetDestRect(PvImage aImage, int aRow, int aColumn)
        {
            int lDestWidth = (int)mTransmitterRawConfig.Width / 2;
            int lDestHeight = (int)mTransmitterRawConfig.Height / 2;

            int lX = 0;
            int lY = 0;
            int lWidth = lDestWidth;
            int lHeight = lDestHeight;

            switch (mTransmitterRawConfig.Mode)
            {
                case TilingMode.TILING_MODE_CROP:
                    
                    lWidth = Math.Min((int)aImage.Width, lWidth);
                    lHeight = Math.Min((int)aImage.Height, lHeight);

                    if (lWidth < lDestWidth)
                    {
                        lX = (lDestWidth - lWidth) / 2;
                    }

                    // Center Y
                    if (lHeight < lDestHeight)
                    {
                        lY = (lDestHeight - lHeight) / 2;
                    }

                    break;

                case TilingMode.TILING_MODE_RATIO:
                    {
                        double lTileAspectRatio = (double)lWidth / (double)lHeight;
                        double lImageAspectRatio = (double)aImage.Width/ (double)aImage.Height;

                        if (lImageAspectRatio > lTileAspectRatio)
                        {
                            lY = lHeight / 2;
                            lHeight = (int)((double)lWidth / lImageAspectRatio);
                            lY -= lHeight / 2;
                        }
                        else
                        {
                            lX = lWidth / 2;
                            lWidth = (int)((double)lHeight * lImageAspectRatio);
                            lX -= lWidth / 2;
                        }
                    }
                    break;

                case TilingMode.TILING_MODE_STRETCH:
                    break;
            }

            return new Rectangle(
                (aColumn * (int)mTransmitterRawConfig.Width / 2) + lX,
                (aRow * (int)mTransmitterRawConfig.Height / 2) + lY,
                lWidth, lHeight);
        }

        /// <summary>
        /// Returns source rect (used when copying in the destination image)
        /// </summary>
        /// <param name="aImage"></param>
        /// <returns></returns>
        private Rectangle GetSrcRect(PvImage aImage)
        {
            int lX = 0;
            int lY = 0;
            int lWidth = (int)aImage.Width;
            int lHeight = (int)aImage.Height;

            if (mTransmitterRawConfig.Mode == TilingMode.TILING_MODE_CROP)
            {
                lWidth = Math.Min((int)mTransmitterRawConfig.Width, lWidth);
                lHeight = Math.Min((int)mTransmitterRawConfig.Height, lHeight);

                // Center X
                if (lWidth < (int)aImage.Width)
                {
                    lX = ((int)aImage.Width - lWidth) / 2;
                }

                // Center Y
                if (lHeight < (int)aImage.Height)
                {
                    lY = ((int)aImage.Height - lHeight) / 2;
                }
            }

            return new Rectangle(lX, lY, lWidth, lHeight);
        }
    }
}

