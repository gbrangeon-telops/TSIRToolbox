// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using PvDotNet;

namespace TransmitTiledImages
{
    /// <summary>
    /// Thread responsible of connecting a device and opening a stream. Performed
    /// in a thread in order not to lock the UI while this is happening. This allows
    /// the UI to stay responsive.
    /// </summary>
    class ConnectionThread
    {

        /// <summary>
        /// Constructors
        /// </summary>
        /// <param name="aSetup">Setup object contains information of the connection settings.</param>
        /// <param name="aDeviceInfo">PvDeviceInfo object contains device information.</param>
        /// <param name="aDevice">PvDevice object.</param>
        /// <param name="aStream">PvStream object.</param>
        public ConnectionThread(Setup aSetup, PvDeviceInfoGEV aDeviceInfo, PvDeviceGEV aDevice, PvStreamGEV aStream)
        {
            mSetup = aSetup;
            mDeviceInfo = aDeviceInfo;
            mDevice = aDevice;
            mStream = aStream;
        }

        // Stream setup information
        private Setup mSetup;

        // Device information
        private PvDeviceInfoGEV mDeviceInfo;

        // Device to connect
        private PvDeviceGEV mDevice;

        // Stream to open
        private PvStreamGEV mStream;

        // Connect result
        private PvResult mResult;

        private bool mFinishedConnect = false;

        // Process form
        private ProgressForm mProgressForm = new ProgressForm();

        public PvResult Result
        {
            get
            {
                return mResult;
            }
        }

        /// <summary>
        /// To do connection thread work.
        /// </summary>
        /// <param name="aParameters">Parameters contain ConnectionThread object.</param>
        private static void DoConnectionThreadWork(object aParameters)
        {
            object[] lParameters = (object[])aParameters;
            ConnectionThread lThis = (ConnectionThread)lParameters[0];
            PvResult lResult = new PvResult(PvResultCode.OK);
            try
            {
                // Connect the device if any device required
                if (lThis.mSetup.Role == Setup.cRoleCtrlData)
                {
                    lThis.ConnectDevice();
                    if (!lThis.mResult.IsOK)
                    {
                        return;
                    }
                }
                // Open stream - and retry if it fails
                lThis.OpenStream();
                if (!lThis.mResult.IsOK)
                {
                    return;
                }
                // Now we need to make the link between the thread and the stream
                if (lThis.mSetup.Role == Setup.cRoleCtrlData)
                {
                    switch (lThis.mSetup.Destination)
                    {
                        case Setup.cDestinationUnicastAuto:
                        case Setup.cDestinationUnicastSpecific:
                            lThis.mDevice.SetStreamDestination(lThis.mStream.LocalIPAddress, lThis.mStream.LocalPort);
                            break;

                        case Setup.cDestinationMulticast:
                            lThis.mDevice.SetStreamDestination(lThis.mSetup.IPAddress, lThis.mSetup.Port);
                            break;
                    }
                }
            }
            catch (Exception lEx)
            {
                PvException lPvException = lEx as PvException;
                if (lPvException != null)
                {
                    lResult = lPvException.Result;
                }
                else
                {
                    lResult = new PvResult(PvResultCode.ABORTED, lEx.Message);
                }
            }

            lThis.mResult = lResult;

            lThis.mProgressForm.TaskDone();
            lThis.mFinishedConnect = true;
        }
       
        /// <summary>
        /// Initialize connection process.
        /// </summary>
        public void Connect()
        {
            Thread lThread = new Thread(new ParameterizedThreadStart(DoConnectionThreadWork));
            ConnectionThread lConnectionThread = this;
            object[] lParameters = new object[] { lConnectionThread };

            lThread.Start(lParameters);
            mProgressForm.ShowDialog();
            lThread.Join();
        }
        /// <summary>
        /// To connection to the selected device.
        /// </summary>
        public void ConnectDevice()
        {
            try
            {
                mProgressForm.Message = "Building GenICam interface...";
                mDevice.Connect(mDeviceInfo, PvAccessType.Control);
                if (mDeviceInfo == null)
                {
                    return;
                }
                bool lEnabledAutoNegotiation = true;
                // Get AutoNegotiation feature
                PvGenBoolean lEnabled = mDevice.CommunicationParameters.GetBoolean("AutoNegotiation");
                if (lEnabled != null)
                {
                    lEnabledAutoNegotiation = lEnabled.Value;
                }
                Int64 lUserPacketSizeValue = 1476;
                //Get default packet size
                PvGenInteger lUserPacketSize =  mDevice.CommunicationParameters.GetInteger("DefaultPacketSize");
                if ((lUserPacketSize != null) && lUserPacketSize.IsReadable)
                {
                    lUserPacketSizeValue = lUserPacketSize.Value;
                }
                if (lEnabledAutoNegotiation) 
                {
                    // Perform automatic packet size negociation
                    mProgressForm.Message = "Optimizing streaming packet size...";
                    try
                    {
                        mDevice.NegotiatePacketSize(0, 1476);
                    }
                    catch(PvException)
                    {
                        mProgressForm.Message = "WARNING: streaming packet size optimization failure, using 1476 bytes!";
                        Thread.Sleep(3000);
                    }
                }
                else
                {
                    bool lManualPacketSizeSuccess = false;

                    // Start by figuring out if we can use GenICam to set the packet size
                    bool lUseGenICam = false;
                    PvGenInteger lPacketSize = mDevice.CommunicationParameters.GetInteger("GevSCPSPacketSize");
                    if (lPacketSize != null)
                    {
                        if (lPacketSize.IsWritable)
                        {
                            lUseGenICam = true;
                        }
                    }
                    if (lUseGenICam)
                    {
                        try
                        {
                            // Setting packet size through the GenICam interface
                            lPacketSize.Value =  lUserPacketSizeValue;
                            lManualPacketSizeSuccess = true;
                        }
                        catch(PvException)
                        {
                        }
                        if (lManualPacketSizeSuccess == true)
                        {
                            // Last resort default...
                            lPacketSize.Value =  576;
                        }
                    }
                    else
                    {
                        // Direct write, for GenICam challenged devices...
                        try
                        {
                            mDevice.WriteRegister(0x0D04, (UInt32)lUserPacketSizeValue);
                            lManualPacketSizeSuccess = true;
                        }
                        catch (PvException)
                        {
                        }

                        if (!lManualPacketSizeSuccess)
                        {
                            // Last resort default...
                            try
                            {
                                mDevice.WriteRegister(0x0D04, 576);
                            }
                            catch (PvException)
                            {
                            }
                            
                            lManualPacketSizeSuccess = false;
                        }
                    }
        
                    string lNewStr;
                    if (lManualPacketSizeSuccess)
                    {
                        lNewStr = "A packet of size of " + lUserPacketSizeValue +
                                    " bytes was configured for streaming. You may experience issues " +
                                    "if your system configuration cannot support this packet size.";
                    }
                    else
                    {                        
                        lNewStr = "WARNING: could not set streaming packet size to " + lUserPacketSizeValue +
                                    " bytes, using " + 576 + " bytes!";
                    }
                    mProgressForm.Message = lNewStr;
                    Thread.Sleep(3000);
                }
            }
            catch (PvException lPvExc)
            {
                throw lPvExc;
            }
        }

        /// <summary>
        /// Open stream to connected device.
        /// </summary>
        public void OpenStream()
        {
            mProgressForm.Message = "Opening eBUS stream to device...";
            try
            {
                PvNetworkAdapter lNetworkAdapter = mDeviceInfo.Interface as PvNetworkAdapter;
                Debug.Assert(lNetworkAdapter != null);

                // Open stream
                switch(mSetup.Destination)
                {
                case Setup.cDestinationUnicastAuto:
                    mStream.Open(mDeviceInfo.IPAddress, 0, 0, lNetworkAdapter.IPAddress);
                    break;

                case Setup.cDestinationUnicastSpecific:

                    mStream.Open(mDeviceInfo.IPAddress, mSetup.Port, 0, lNetworkAdapter.IPAddress);
                    break;

                case Setup.cDestinationMulticast:
                    mStream.Open(mDeviceInfo.IPAddress, mSetup.IPAddress, mSetup.Port, 0, lNetworkAdapter.IPAddress);
                    break;

                default:
                    Debug.Fail("Unexpected case.");
                    break;
                }
            }
            catch (PvException lPvExc)
            {
                mResult = lPvExc.Result;
                mProgressForm.Message = mResult.ToString();
                mFinishedConnect = true;
            }
        }
    }
    
}
