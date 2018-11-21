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


namespace DualSource
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

            PvGenEnum lSourceSelector = mDevice.Parameters.Get("SourceSelector") as PvGenEnum;
            if (lSourceSelector != null)
            {
                mMultiSource = (lSourceSelector.EntriesCount > 1);
            }

            mDisplayThread = new PvDisplayThread();
            mDisplayThread.OnBufferDisplay += new OnBufferDisplay(OnBufferDisplay);
        }

        // Owned by the source object
        private PvStreamGEV mStream = null;
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
            get { return (mStream != null) && mStream.IsOpen; }
        }

        public PvGenParameterArray Parameters
        {
            get 
            {
                if (mStream != null)
                {
                    return mStream.Parameters;
                }

                return null;
            }
        }

        public void Open(PvDeviceInfo aDI, UInt16 aChannel)
        {
            mStream = new PvStreamGEV();
            mStream.Open(aDI, 0, aChannel, "", 64);

            mPipeline = new PvPipeline(mStream);
            mPipeline.OnBufferTooSmall += new OnBufferTooSmall(OnBufferTooSmall);

            if (aDI.Type == PvDeviceInfoType.GEV)
            {
                PvDeviceGEV lDeviceGEV = mDevice as PvDeviceGEV;
                PvStreamGEV lStreamGEV = mStream as PvStreamGEV;

                // Negotiate packet size
                lDeviceGEV.NegotiatePacketSize(aChannel, 1476);

                // Set stream destination
                lDeviceGEV.SetStreamDestination(lStreamGEV.LocalIPAddress, lStreamGEV.LocalPort, aChannel);
            }

            // Register acquisition mode changed callback
            PvGenParameter lAcquisitionMode = mDevice.Parameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate += new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Force acquisition mode to be synchronized
            PvGenEnum lSourceSelector = mDevice.Parameters.GetEnum("SourceSelector");
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
            PvGenParameter lAcquisitionMode = mDevice.Parameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate -= new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Release pipeline
            if (mPipeline != null)
            {
                mPipeline.Dispose();
                mPipeline = null;
            }

            // Close stream
            if (mStream != null)
            {
                mStream.Close();
                mStream.Dispose();
                mStream = null;
            }
        }

        public void StartStreaming()
        {
            if ((mStream == null) || // Not initialized yet
                mPipeline.IsStarted) // Already streaming
            {
                return;
            }

            // Configure status control
            mStatusControl.Stream = mStream;
            mStatusControl.DisplayThread = mDisplayThread;

            // Start threads
            mDisplayThread.Start(mPipeline, mDevice.Parameters);
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

        void  OnAcquisitionStateChanged(PvDevice aDevice, PvStream aStream, uint aSource, PvAcquisitionState aState)
        {
            mNotifications.UpdateAcquisitionState();
        }

        public void StopStreaming()
        {
            if ((mStream == null) || // Not initialized yet
                !mPipeline.IsStarted) // Not streaming
            {
                return;
            }

            // Status control no longer needs stream, display thread
            mStatusControl.Stream = null;
            mStatusControl.DisplayThread = null;

            // Unregister acquisition mode changed callback
            PvGenParameter lAcquisitionMode = mDevice.Parameters.Get("AcquisitionMode");
            if (lAcquisitionMode != null)
            {
                lAcquisitionMode.OnParameterUpdate -= new OnParameterUpdateHandler(OnParameterChanged);
            }

            // Stop display thread
            mDisplayThread.Stop(false);

            // Release acquisition manager
            mAcquisitionManager.Dispose();
            mAcquisitionManager = null;

            // Stop pipeline
            mPipeline.Stop();

            // Wait on thread
            mDisplayThread.WaitComplete();
        }

        public void StartAcquisition()
        {
            // Get payload size
            UInt32 lPayloadSize = mDevice.PayloadSize;

            // Propagate to pipeline to make sure buffers are big enough
            mPipeline.BufferSize = lPayloadSize;

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

        public void StopAcquisition()
        {
            // Use acquisition manager to send the acquisition stop command to the device
            mAcquisitionManager.Stop();
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

