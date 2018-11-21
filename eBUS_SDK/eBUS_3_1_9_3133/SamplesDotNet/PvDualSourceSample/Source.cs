// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Text;
using PvDotNet;
using PvGUIDotNet;


namespace PvDualSourceSample
{
    class Source
    {
        public Source(UInt16 aSourceIndex, INotifications aNotifications, 
            PvDevice aDevice, PvDisplayControl aDisplayControl, PvStatusControl aStatusControl)
        {
            mSourceIndex = aSourceIndex;
            mNotifications = aNotifications;
            mDevice = aDevice;
            mDisplayControl = aDisplayControl;
            mStatusControl = aStatusControl;

            mPipeline = new PvPipeline(mStream);
            mPipeline.OnBufferTooSmall += new OnBufferTooSmall(OnBufferTooSmall);

            PvGenEnum lSourceSelector = mDevice.GenParameters.Get("SourceSelector") as PvGenEnum;
            if (lSourceSelector != null)
            {
                mMultiSource = (lSourceSelector.EntriesCount > 1);
            }

            mDisplayThread = new PvDisplayThread();
            mDisplayThread.OnBufferDisplay += new OnBufferDisplay(OnBufferDisplay);
        }

        // Owned by the source object
        private PvStream mStream = new PvStream();
        private PvPipeline mPipeline = null;

        // Referenced from outside
        private PvDevice mDevice = null;
        private PvDisplayControl mDisplayControl = null;
        private PvStatusControl mStatusControl = null;

        // Acquisition state manager
        private PvAcquisitionStateManager mAcquisitionManager = null;

        // Display thread
        private PvDisplayThread mDisplayThread = null;

        // Source index
        private UInt16 mSourceIndex = 0;

        // Used to send notifications back to the owner
        private INotifications mNotifications = null;

        // Is the device multisource?
        private bool mMultiSource = false;

        public PvAcquisitionState AcquisitionState
        {
            get 
            {
                if (mAcquisitionManager == null)
                {
                    return PvAcquisitionState.Unknown;
                }

                return mAcquisitionManager.State; 
            }
        }

        public bool IsOpened
        {
            get { return mStream.IsOpened; }
        }

        public PvGenParameterArray Parameters
        {
            get { return mStream.Parameters; }
        }

        public void Open(string aIPAddress, UInt16 aChannel)
        {
            // Open stream
            mStream.Open(aIPAddress, 0, aChannel);

            // Negotiate packet size
            mDevice.NegotiatePacketSize(aChannel, 1476);

            // Set stream destination
            mDevice.SetStreamDestination(mStream.LocalIPAddress, mStream.LocalPort, aChannel);

            // Register acquisition mode changed callback
            PvGenParameter lAcquisitionMode = mDevice.GenParameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate += new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Force acquisition mode to be synchronized
            PvGenEnum lSourceSelector = mDevice.GenParameters.GetEnum("SourceSelector");
            if (lSourceSelector != null)
            {
                lSourceSelector.ValueString = "Source" + (mSourceIndex + 1).ToString();
            }
            mNotifications.UpdateAcquisitionMode(true);

            // Ready image reception
            StartStreaming();
        }

        public void Close()
        {
            // Stop image reception
            StopStreaming();

            // Unregister acquisition mode changed callback
            PvGenParameter lAcquisitionMode = mDevice.GenParameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate -= new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Close stream
            if (mStream.IsOpened)
            {
                mStream.Close();
            }
        }

        public void StartStreaming()
        {
            if (!mStream.IsOpened || // Not initialized yet
                mPipeline.IsStarted) // Already streaming
            {
                return;
            }

            // Configure status control
            mStatusControl.Stream = mStream;
            mStatusControl.DisplayThread = mDisplayThread;

            // Start threads
            mDisplayThread.Start(mPipeline, mDevice.GenParameters);
            mDisplayThread.Priority = PvThreadPriority.AboveNormal;

            // Configure acquisition state manager
            mAcquisitionManager = new PvAcquisitionStateManager(mDevice, mStream, mSourceIndex);
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
            mDisplayControl.Display(aBuffer);
        }

        void  OnParameterChanged(PvGenParameter aParameter)
        {
            if (aParameter.Name == "AcquisitionMode")
            {
                mNotifications.UpdateAcquisitionMode(false);
            }
        }

        void  OnAcquisitionStateChanged(PvDevice aDevice, PvStreamBase aStream, uint aSource, PvAcquisitionState aState)
        {
            mNotifications.UpdateAcquisitionState();
        }

        public void StopStreaming()
        {
            if (!mStream.IsOpened || // Not initialized yet
                !mPipeline.IsStarted) // Not streaming
            {
                return;
            }

            // Unregister acquisition mode changed callback
            PvGenParameter lAcquisitionMode = mDevice.GenParameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate -= new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Stop display thread
            mDisplayThread.Stop(false);

            // Release acquisition manager
            mAcquisitionManager = null;

            // Stop pipeline
            mPipeline.Stop();

            // Wait on thread
            mDisplayThread.WaitComplete();
        }

        public void StartAcquisition()
        {
            using (PvGenStateStack lState = new PvGenStateStack(mDevice.GenParameters))
            {
                if (mMultiSource)
                {
                    // Push change on stack, will be reset when lState goes out of scope
                    lState.SetEnumValue("SourceSelector", mSourceIndex);
                }

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

                // Reset buffer reallocated warning from status control
                mStatusControl.BuffersReallocated = false;

                // Reset display thread stats (mostly frames displayed per seconds)
                mDisplayThread.ResetStatistics();

                // Use acquisition manager to send the acquisition start command to the device
                mAcquisitionManager.Start();
            }
        }

        public void StopAcquisition()
        {
            using (PvGenStateStack lState = new PvGenStateStack(mDevice.GenParameters))
            {
                if (mMultiSource)
                {
                    // Push change on stack, will be reset when lState goes out of scope
                    lState.SetEnumValue("SourceSelector", mSourceIndex);
                }
                // Use acquisition manager to send the acquisition stop command to the device
                mAcquisitionManager.Stop();
            }
        }

        /// <summary>
        /// Retrieve or guess the payload size
        /// </summary>
        private UInt32 PayloadSize
        {
            get
            {   
                using (PvGenStateStack lState = new PvGenStateStack(mDevice.GenParameters))
                {
                    if (mMultiSource)
                    {
                        // Push change on stack, will be reset when lState goes out of scope
                        lState.SetEnumValue("SourceSelector", mSourceIndex);
                    }

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
        }

        /// <summary>
        /// Pipeline buffer too small event handler. Let the pipeline know we want the buffers to
        /// be reallocated and set a flag to indicate buffers have been reallocated.
        /// </summary>
        /// <param name="aPipeline"></param>
        /// <param name="aReallocAll"></param>
        /// <param name="aResetStats"></param>
        void OnBufferTooSmall(PvPipeline aPipeline, ref bool aReallocAll, ref bool aResetStats)
        {
            aReallocAll = true;
            aResetStats = true;

            mStatusControl.BuffersReallocated = true;
        }
    }
}

