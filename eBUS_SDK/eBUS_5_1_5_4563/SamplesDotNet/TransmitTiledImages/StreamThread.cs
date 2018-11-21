// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;
using PvDotNet;
using PvGUIDotNet;

namespace TransmitTiledImages
{
    /// <summary>
    /// Thread to manage the integration with a streaming device source.
    /// </summary>
    class StreamThread
    {
        public delegate void OnLinkDisconnectedDelegate(int aRow, int aColumn);
        public event OnLinkDisconnectedDelegate OnLinkDisconnectedEvent;

        // Pool size definition.
        private const UInt16 cRxPoolSize = 4;
        private const UInt16 cTxPoolSize = 4;

        private const UInt32 cWidthResolution = 640;
        private const UInt32 cHeightResolution = 480;
        private const UInt32 cPacketSize = 1476;
        private const UInt32 cDefaultRxBufferAllocationSize = cWidthResolution * cHeightResolution * cRxPoolSize;

        /// <summary>
        /// Constructor.
        /// </summary>
        public StreamThread(PvDisplayControl aPvDisplay) 
        {
            mDisplay = aPvDisplay;
            mRow = -1;
            mColumn = -1;
            mStart = null;
            mStop = null;
            mConnected = false;
            mLastPayloadSize = 0;
        }

        // Device to stream from.
        PvDevice mDevice = null;

        // Stream recording the data.
        private PvStream mStream = null;

        private bool mConnected = false;

        // Cache the last payload size to avoid resizing the buffer.
        private UInt32 mLastPayloadSize;

        // Current information on the stream connection.
        private Setup mSetup = new Setup();
        private string mMACAddress;
        private string mIPAddress;
        private string mSerialNumber;
        private string mModel;
        private string mUserDefinedName;

        // Control command for the device streaming.
        private PvGenCommand mStart;
        private PvGenCommand mStop;

        // Return queue for the buffers.
        Queue<SmartBuffer> mReturnBufferQueues = null;

        // Storage for the allocated buffer for this application.
        SmartBuffer[] mBuffers = null;

        // Object that keep track of the last buffer for each stream.
        CurrentBuffersTable mCurrentBuffersTable = null;

        // Signal to stop streaming.
        bool mStopStreaming = false;

        // This is the unique index of the thread.
        // Use to map to the current image table.
        private Int32 mRow;
        private Int32 mColumn;

        private Thread mThread = null;

        // To display images.
        private PvDisplayControl mDisplay = null;

        public bool IsConnected
        {
            get { return mConnected; }
            set { value = mConnected; }
        }

        public PvDevice Device
        {
            get { return mDevice; }
        }

        public PvStream Stream
        {
            get { return mStream; }
        }

        public string IPAddress
        {
            get { return mIPAddress; }
        }

        public string MACAddress
        {
            get { return mMACAddress; }
        }

        public string SerialNumber
        {
            get { return mSerialNumber; }
        }

        public string Model
        {
            get { return mModel; }
        }

        public string UserDefinedName
        {
            get { return mUserDefinedName; }
        }

        public int Row
        {
            get { return mRow; }
        }

        public int Column
        {
            get { return mColumn; }
        }

        /// <summary>
        /// Free buffers, not waiting for garbage collection
        /// </summary>
        void FreeBuffers()
        {
            if (mBuffers != null)
            {
                for (int i = 0; i < mBuffers.Length; i++)
                {
                    if (mBuffers[i] != null)
                    {
                        mBuffers[i].Dispose();
                        mBuffers[i] = null;
                    }
                }
            }

            mBuffers = null;
            mLastPayloadSize = 0;
        }

        /// <summary>
        /// Alllocates new buffers
        /// </summary>
        /// <param name="aPayloadSize"></param>
        /// <returns></returns>
        bool CreateNewBuffers( UInt32 aPayloadSize )
        {
            FreeBuffers();

            // Allocate the buffers pool
            mBuffers = new SmartBuffer[cRxPoolSize];
            for ( int i = 0; i < mBuffers.Length; i++ )
            {
                mBuffers[i]= new SmartBuffer(ref mReturnBufferQueues);
                if ( mBuffers[i] == null )
                {
                    if (i > 0)
                    {
                        for ( int j = 0; j < i; j++ )
                        {
                            mBuffers[ j ] = null;
                        }
                    }
                    return false;
                }
            }

            // Now we can set the size of the buffers.
            for ( int i = 0; i < mBuffers.Length; i++ )
            {
                try
                {
                    mBuffers[i].Alloc(aPayloadSize);
                }
                catch (PvException)
                {
                    for (int j = 0; j <= i; j++)
                    {
                        mBuffers[j] = null;
                    }
                    return false;
                }
            }
    
            return true;
        }

        /// <summary>
        /// To do data acquisition.
        /// </summary>
        private void DoStreamThreadWork()
        {
            PvImage lImage;
            PvResult lOperationResult = new PvResult(PvResultCode.OK);
            PvResult lResult;

            while (!mStopStreaming)
            {
                // The 50ms timeout is to allow the system to check the stop state.
                PvBuffer lBuffer = null;
                lResult = mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult, 50);
                if (lResult.IsOK)
                {
                    if (lOperationResult.IsOK)
                    {
                        // We will keep this image as the last acquired.
                        // The mCurrentBuffersTable will manage the return of the buffer into the stream.
                        mCurrentBuffersTable.Set((SmartBuffer)lBuffer, mRow, mColumn);
                    }
                    else if (lOperationResult.Code == PvResultCode.BUFFER_TOO_SMALL)
                    {
                        // The buffer is too small to retrieve the image from the source
                        // we will need to re-allocate the buffer to fit the image. 
                        lImage = lBuffer.Image;
                        if (lImage == null)
                        {
                            continue;
                        }

                        mLastPayloadSize = lImage.RequiredSize;
                        lBuffer.Alloc(mLastPayloadSize);

                        mStream.QueueBuffer(lBuffer);
                    }
                    else
                    {
                         mStream.QueueBuffer(lBuffer);
                    }
                }
               
                // Requeue any available buffer.
                while (mReturnBufferQueues.Count > 0)                       
                {
                    SmartBuffer lSmartBuffer = mReturnBufferQueues.Dequeue();
                    mStream.QueueBuffer((PvBuffer)lSmartBuffer);
                }
            }
        }        

        /// <summary>
        /// To response to the OnLinkDisconnected event from PvDevice.
        /// </summary>
        /// <param name="aDevice">PvDevice object.</param>
        public void OnLinkDisconnected(PvDevice aDevice)
        {
            // Notify the MainForm to call method OnLinkDisconnectedMessage to display message when the device is disconnected.
            OnLinkDisconnectedEvent(mRow, mColumn);
        }

        /// <summary>
        /// Initialize the stream memory class. Must be called once at the beginning.
        /// </summary>
        /// <param name="aRow">Row number.</param>
        /// <param name="aColumn">Column number.</param>
        /// <param name="aCurrentBuffersTable">Current Buffers Table used as shared data destination.</param>
        /// <returns>true - Initialized the stream memory class successfully, otherwise false.</returns>
        public bool Initialize(Int32 aRow, Int32 aColumn, ref CurrentBuffersTable aCurrentBuffersTable)
        {
            if (aCurrentBuffersTable == null)
            {
                return false;
            }

            mRow = aRow;
            mColumn = aColumn;

            mCurrentBuffersTable = aCurrentBuffersTable;

            mReturnBufferQueues = new Queue<SmartBuffer>(cRxPoolSize + 1);
            if (mReturnBufferQueues == null)
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// To hide the GenICam controls form.
        /// </summary>
        /// <param name="aForm">Form to be hided.</param>
        public void CloseGenControlForm(Form aForm)
        {
            aForm.Hide();
        }

        /// <summary>
        /// Connect to the image source.
        /// </summary>
        /// <param name="aDeviceInfo"></param>
        /// <returns>True - Connected to the image source successfully, otherwise false.</returns>
        public bool Connect(PvDeviceInfo aDeviceInfo)
        {
            SetupForm lSetupForm = new SetupForm(ref mSetup, aDeviceInfo.Type);
            if (lSetupForm.ShowDialog() != DialogResult.OK)
            {
                return false;
            }
            
            // Just in case we came here still connected...
            Disconnect();

            // Create concrete device and stream
            switch (aDeviceInfo.Type)
            {
                case PvDeviceInfoType.GEV:
                    mDevice = new PvDeviceGEV();
                    mStream = new PvStreamGEV();
                    break;

                case PvDeviceInfoType.U3V:
                    mDevice = new PvDeviceU3V();
                    mStream = new PvStreamU3V();
                    break;

                default:
                    break;
            }

            mDevice.OnLinkDisconnected += new OnLinkDisconnectedHandler(OnLinkDisconnected);

            // Device connection, packet size negotiation and stream opening is performed in a separate 
            // thread while we display a progress dialog to the user.
            ConnectionThread lConnectionThread = new ConnectionThread(mSetup, aDeviceInfo, mDevice, mStream);
          
            lConnectionThread.Connect();
            if (!lConnectionThread.Result.IsOK)
            {
                Disconnect();
                return false;
            }

            if (aDeviceInfo.Type == PvDeviceInfoType.GEV)
            {
                PvDeviceInfoGEV lDeviceInfoGEV = (PvDeviceInfoGEV) aDeviceInfo;
                mIPAddress = lDeviceInfoGEV.IPAddress;
                mMACAddress = lDeviceInfoGEV.MACAddress;
                mSerialNumber = string.Empty;
            }
            else
            {
                PvDeviceInfoU3V lDeviceInfoU3V = (PvDeviceInfoU3V) aDeviceInfo;
                mIPAddress = string.Empty;
                mMACAddress = string.Empty;
                mSerialNumber = lDeviceInfoU3V.U3VSerialNumber;
            }
            mModel = aDeviceInfo.ModelName;
            mUserDefinedName = aDeviceInfo.UserDefinedName;

            if (mDevice.IsConnected == true)
            {
                mConnected = true;
            }

            return true;
        }

        /// <summary>
        /// Disconnect to the image source.
        /// </summary>
        public void Disconnect()
        {
            Stop();

            if (mDevice != null && mDevice.IsConnected == true)
            {
                mDevice.Disconnect();
            }

            if (mDevice != null && mStream.IsOpen == true)
            {
                mStream.Close();
            }
           
            // Clear the used buffers memory
            FreeBuffers();
            
            mConnected = false;
        }

        /// <summary>
        /// Start the stream engine and process the incoming packets.
        /// </summary>
        /// <returns>True - Start stream engine successfully, otherwise false.</returns>
        public bool Start()
        {
            if (mSetup.Role != Setup.cRoleData)
            {
                mDevice.StreamEnable();
                
                try
                {                    
                    // Start the internal thread to retrieve the data.
                    mThread = new Thread(DoStreamThreadWork);
                    mStopStreaming = false;
                    mThread.Start();
                }
                catch (Exception)
                {
                    return false;
                }
                
                // Start the stream.
                if (mStart != null)
                {
                    mStart.Execute();
                }    
            }
            else
            {
                try
                {
                    // Start the internal thread to retrieve the data.
                    mThread = new Thread(DoStreamThreadWork);
                    mStopStreaming = false;
                    mThread.Start();
                }
                catch (Exception)
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Stop data acquisition.
        /// </summary>
        /// <returns>True - Stop data acquisition successfully, otherwise false.</returns>
        public bool Stop()
        {
            PvResult lOperationResult = new PvResult(PvResultCode.OK);
            if (mSetup.Role != Setup.cRoleData)
            {
                if (mDevice == null || !mDevice.IsConnected)
                {
                    return true;
                }

                // Stop the streaming on the device
                if (mStop != null)
                {
                    try
                    {
                        mStop.Execute();
                    }
                    catch (PvException)
                    {
                    }
                }

                mDevice.StreamDisable();
            }

            // Stop the thread retrieving the data.
            mStopStreaming = true;
            if (mThread != null)
            {
                // Blocks the calling thread until the thread terminated.
                mThread.Join();
                mThread = null;
            }

            try
            {
                // Abort the stream.
                if (mStream != null)
                {
                    mStream.AbortQueuedBuffers();

                    // Dequeue all the buffers.
                    PvBuffer lBuffer = null;
                    while (mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult).IsOK)
                    {
                        lBuffer = null;
                    }
                }
            }
            catch (PvException)
            {
                return false;
            }

            // We want to recover all the buffers, including the one
            // in the display snapshot...
            mCurrentBuffersTable.Reset(mRow, mColumn);

            while (mReturnBufferQueues.Count > 0)
            {
                // This is emptying the ready queue...
                mReturnBufferQueues.Dequeue();
            }

            return true;
        }

        public bool Configure(ref UInt32 aWidth, ref UInt32 aHeight)
        {
            UInt32 lPayloadSize;
            PvGenCommand lResetTimestamp;
            PvGenCommand lResetStats;

            // Get access to the parameters.
            mStart = mDevice.Parameters.GetCommand("AcquisitionStart");
            mStop = mDevice.Parameters.GetCommand("AcquisitionStop");
            lResetTimestamp = mDevice.Parameters.GetCommand("GevTimestampControlReset");
            lResetStats = mStream.Parameters.GetCommand("Reset");

            aHeight = 0;
            aWidth = 0;

            // Reset the statistic.
            lResetStats.Execute();

            // Reset timestamp counter.
            if (lResetTimestamp != null)
            {
                lResetTimestamp.Execute();
            }

            if (mSetup.Role == Setup.cRoleData)
            {
                if (mLastPayloadSize == 0)
                {
                    // In data receiver only mode, we do not know what will be the image size 
                    // For this reason, we will re-allocate the image size on the fly. This is
                    // bad but the only way we can make it work.
                    if (CreateNewBuffers(cDefaultRxBufferAllocationSize) != true)
                    {
                        return false;
                    }

                    // We need to cache the payload size for future starts...
                    mLastPayloadSize = cDefaultRxBufferAllocationSize;                    
                }
            }
            else
            {
                aWidth = (UInt32)mDevice.Parameters.GetIntegerValue("Width");
                aHeight = (UInt32)mDevice.Parameters.GetIntegerValue("Height");

                // Get the current configured payload size.
                lPayloadSize = mDevice.PayloadSize;
                if (lPayloadSize == 0)
                {
                    // We cannot get the size, the buffer cannot be pre-allocated.
                    return false;
                }

                // Manage the size of the buffer allocated to receive the images.
                if (mLastPayloadSize != lPayloadSize)
                {
                    if (CreateNewBuffers(lPayloadSize) != true)
                    {
                        return false;
                    }
                    // We need to cache the payload size for future starts...
                    mLastPayloadSize = lPayloadSize;
                }
            }

            // Queue the buffers into the stream.
            for (int i = 0; i < mBuffers.Length; i++)
            {
                mStream.QueueBuffer((PvBuffer) mBuffers[ i ]); 
            }

            return true;
        }

        public bool Store(ref PvConfigurationWriter aWriter)
        {
            string lTemp;

            if (mConnected)
            {
                // Store the device.
                if (mSetup.Role == Setup.cRoleCtrlData)
                {
                    lTemp = string.Format("Device( {0}, {1} )", mRow, mColumn);
                    aWriter.Store(mDevice, lTemp);
                }

                // Store the stream.
                lTemp = string.Format("Stream( {0}, {1} )", mRow, mColumn);
                aWriter.Store(mStream, lTemp);

                // Store the setup information.
                mSetup.Store(ref aWriter, mRow, mColumn);      
            }

            return true;
        }

        public bool Restore(PvConfigurationReader aReader)
        {
            string lTemp;

            if (mConnected)
            {
                Disconnect();
            }

            // Read the setup information first to understand how to deal with the 
            // stream and the device.
            try
            {
                mSetup.Restore(ref aReader, mRow, mColumn);
            }
            catch (PvException)
            {
                return false;
            }

            switch(mSetup.Role)
            {
                case Setup.cRoleCtrlData:
                    lTemp = string.Format("Device( {0}, {1} )", mRow, mColumn);
                    try
                    {
                        aReader.Restore(lTemp, mDevice);
                    }
                    catch (PvException)
                    {
                        return false;
                    }
                    goto case Setup.cRoleData;

                case Setup.cRoleData:
                    lTemp = string.Format("Stream( {0}, {1} )", mRow, mColumn);
                    try
                    {
                        aReader.Restore(lTemp, mStream);
                    }
                    catch (PvException)
                    {
                        mDevice.Disconnect();
                        return false;
                    }
                    break;

                default:
                    return false;
            }

            if (mDevice.Type == PvDeviceType.DeviceGEV)
            {
                // Now we need to make the link between the thread and the strea
                PvDeviceGEV lDeviceGEV = mDevice as PvDeviceGEV;
                PvStreamGEV lStreamGEV = mStream as PvStreamGEV;
                if (mSetup.Role == Setup.cRoleCtrlData)
                {
                    switch (mSetup.Destination)
                    {
                        case Setup.cDestinationUnicastAuto:
                        case Setup.cDestinationUnicastSpecific:
                            lDeviceGEV.SetStreamDestination(lStreamGEV.LocalIPAddress, lStreamGEV.LocalPort);
                            break;
                        case Setup.cDestinationMulticast:
                            lDeviceGEV.SetStreamDestination(mSetup.IPAddress, mSetup.Port);
                            break;
                        default:
                            return false;
                    }
                }
            }

            // Now we will retrieve the device information to be able to display it
            PvGenParameterArray lGenDevice = mDevice.Parameters;

            if (mDevice.Type == PvDeviceType.DeviceGEV)
            {
                // IP
                PvGenInteger lIPAddressParam = lGenDevice.GetInteger("GevCurrentIPAddress");
                byte[] lIPAddressBytes = BitConverter.GetBytes(lIPAddressParam.Value);
                mIPAddress = String.Format("{0}.{1}.{2}.{3}",
                    lIPAddressBytes[3], lIPAddressBytes[2], lIPAddressBytes[1], lIPAddressBytes[0]);

                // MAC address
                PvGenInteger lMACAddressParam = lGenDevice.GetInteger("GevMACAddress");
                byte[] lMacAddressBytes = BitConverter.GetBytes(lMACAddressParam.Value);
                mMACAddress = String.Format("{0}:{1}:{2}:{3}:{4}:{5}",
                    lMacAddressBytes[5].ToString("x2"), lMacAddressBytes[4].ToString("x2"), lMacAddressBytes[3].ToString("x2"),
                    lMacAddressBytes[2].ToString("x2"), lMacAddressBytes[1].ToString("x2"), lMacAddressBytes[0].ToString("x2"));
            }
            else if (mDevice.Type == PvDeviceType.DeviceU3V)
            {
                mSerialNumber = lGenDevice.GetInteger("DeviceSerialNumber").ToString();
            }

            // Model name.
            PvGenString lModelName = lGenDevice.GetString("DeviceModelName");
            mModel = lModelName.Value;
            
            // Device name ( User ID ).
            try
            {
                PvGenString lNameParam = lGenDevice.GetString("GevDeviceUserID");
                mUserDefinedName = lNameParam.Value;
            }
            catch
            {
            }

            mConnected = true;
            return true;
        }

    }
}
