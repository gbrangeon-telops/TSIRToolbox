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

namespace PvTransmitTiledImageSample
{
    /// <summary>
    /// Thread to manage the interation with a streaming device source.
    /// </summary>
    class StreamThread
    {
#region Delegate
        public delegate void OnLinkDisconnectedDelegate(int aRow, int aColumn);
        public event OnLinkDisconnectedDelegate OnLinkDisconnectedEvent;
#endregion

#region constants

        // Pool size definition
        private const UInt16 cRxPoolSize = 4;
        private const UInt16 cTxPoolSize = 4;

        private const UInt32 cWidthResolution = 640;
        private const UInt32 cHeightResolution = 480;
        private const UInt32 cPacketSize = 1476;
        private const UInt32 cDefaultRxBufferAllocationSize = cWidthResolution * cHeightResolution * cRxPoolSize;

#endregion

#region Constructor
        /// <summary>
        /// Constructor
        /// </summary>
        public StreamThread(PvDisplayControl aPvDisplay) 
        {
            mDisplay = aPvDisplay;
            mRow = -1;
            mColumn = -1;
            mStart = null;
            mStop = null;
            mTLLocked = null;
            mConnected = false;
            mLastPayloadSize = 0;
            mDevice.OnLinkDisconnected += new OnLinkDisconnectedHandler(OnLinkDisconnected);
        }
#endregion

#region Data members

        // Device to stream from
        PvDevice mDevice = new PvDevice();

        // Stream recording the data
        private PvStream mStream = new PvStream();

        private bool mConnected = false;

        // Cache the last payload size to avoid resizing the buffer
        private UInt32 mLastPayloadSize;

        // Current information on the stream connection
        private Setup mSetup = new Setup();
        private string mMACAddress;
        private string mIPAddress;
        private string mModel;
        private string mUserDefinedName;

        // Control command for the device streaming
        private PvGenCommand mStart;
        private PvGenCommand mStop;
        private PvGenEnum mMode;
        private PvGenInteger mTLLocked;

        // Return queue for the buffers 
        Queue<SmartBuffer> mReturnBufferQueues = null;

        // Storage for the allocated buffer for this application
        SmartBuffer[] mBuffers = new SmartBuffer[cRxPoolSize];

        // Object that keep track of the last buffer for each stream
        CurrentBuffersTable mCurrentBuffersTable = null;

        // Signal to stop streaming
        bool mStopStreaming = false;

        // This is the unique index of the thread
        // Use to map to the current image table
        private Int32 mRow;
        private Int32 mColumn;

        private Thread mThread = null;

        // To display images.
        private PvDisplayControl mDisplay = null;

#endregion


#region Properties
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
            get {  return mMACAddress; }
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

#endregion

#region Utilities methods
        bool CreateNewBuffers( UInt32 aPayloadSize )
        {
            // First allocate all the buffers pool
            for( int i = 0; i < cRxPoolSize; i++ )
            {
                mBuffers[i]= new SmartBuffer( ref mReturnBufferQueues );
                if( mBuffers[i] == null )
                {
                    if (i > 0)
                    {
                        for( int j = 0; j < i; j++ )
                        {
                            mBuffers[ j ] = null;
                        }
                    }
                    return false;
                }
            }

            // Now we can set the size of the buffers
            for( int i = 0; i < cRxPoolSize; i++ )
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
        /// Query the image source to get the payload size
        /// </summary>
        /// <param name="aWidth">Width of image.</param>
        /// <param name="aHeight">Height of image.</param>
        /// <returns>The payload size.</returns>
        private UInt32 GetPayloadSize(ref UInt32 aWidth, ref UInt32 aHeight)
        {
            Int64 lWidthValue = 0; 
            Int64 lHeightValue = 0;
            Int64 lPayloadSizeValue = 0;
            Int64 lPoorMansPayloadSize = 0;
            Int64 lPixelFormatValue = 0;
            Int64 lPixelSizeInBits = 0;

            try
            {
                PvGenInteger lPayloadSize = mDevice.GenParameters.GetInteger("PayloadSize");
                PvGenInteger lWidth = mDevice.GenParameters.GetInteger("Width") ;
                PvGenInteger lHeight = mDevice.GenParameters.GetInteger("Height") ;
                PvGenEnum lPixelFormat = mDevice.GenParameters.GetEnum("PixelFormat");

                aWidth = 0;
                aHeight = 0;

                // Get payload size
                if (lPayloadSize != null)
                {
                    lPayloadSizeValue = lPayloadSize.Value;
                }

                // Get the width and the height
                lWidthValue = lWidth.Value;
                aWidth = (UInt32) lWidthValue;
                lHeightValue = lHeight.Value;
                aHeight = (UInt32) lHeightValue;

                // Compute poor man's payload size - for devices not maintaning PayloadSize properly
                lPixelFormatValue = lPixelFormat.ValueInt;
                lPixelSizeInBits = PvImage.GetPixelBitCount((PvPixelType)lPixelFormatValue);
                lPoorMansPayloadSize = (lWidthValue * lHeightValue * lPixelSizeInBits) / 8;
            }
            catch(PvException lPvExc)
            {
                throw lPvExc;
            }
            Int64 lBestPayloadSize = Math.Max(lPayloadSizeValue, lPoorMansPayloadSize);
            if ((lBestPayloadSize > 0)  && (lBestPayloadSize < UInt32.MaxValue))
            {
                // Round up to make it mod 32 (works around an issue with some devices)
                if ((lBestPayloadSize % 32) != 0)
                {
                    lBestPayloadSize = ((lBestPayloadSize / 32) + 1) * 32;
                }

                return (UInt32)(lBestPayloadSize);
            }

            // Could not compute/retrieve payload size...
            return 0;
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
                // The 50ms timeout is to allow the system to check the stop state
                PvBuffer lBuffer = null;
                lResult = mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult, 50);
                if (lResult.IsOK)
                {
                    if (lOperationResult.IsOK)
                    {
                        // We will keep this image as the last acquired.
                        // The mCurrentBuffersTable will manage the return of the buffer into the stream
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
               
                // Requeue any available buffer
                while (mReturnBufferQueues.Count > 0)                       
                {
                    SmartBuffer lSmartBuffer = mReturnBufferQueues.Dequeue();
                    mStream.QueueBuffer((PvBuffer)lSmartBuffer);
                }
            }
        }        

#endregion

#region Public methods

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
        /// <returns>true - Initialized the stream memory class successfully, otherwise flase.</returns>
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
        /// <param name="aPvDeviceInfo"></param>
        /// <returns>True - Connected to the image source successfullu, otherwise false.</returns>
        public bool Connect(PvDeviceInfo aPvDeviceInfo)
        {
            SetupForm lSetupForm = new SetupForm(ref mSetup);
           
            if (lSetupForm.ShowDialog() != DialogResult.OK)
            {
                return false;
            }
            
            // Just in case we came here still connected...
            Disconnect();

            // Device connection, packet size negociation and stream opening is performed in a separate 
            // thread while we display a progress dialog to the user
            ConnectionThread lConnectionThread = new ConnectionThread(mSetup, aPvDeviceInfo, mDevice, mStream);
          
            lConnectionThread.Connect();

            if (!lConnectionThread.Result.IsOK)
            {
                Disconnect();
                return false;
            }

            mIPAddress = aPvDeviceInfo.IPAddress;
            mMACAddress = aPvDeviceInfo.MACAddress;
            mModel = aPvDeviceInfo.Model;
            mUserDefinedName = aPvDeviceInfo.UserDefinedName;

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

            if (mDevice.IsConnected == true)
            {
                mDevice.Disconnect();
            }

            if (mStream.IsOpened == true)
            {
                mStream.Close();
            }
           
            // Clear the used buffers memory
            if(mLastPayloadSize > 0)
            {
                for(int i = 0; i < cRxPoolSize; i++)
                {
                    if (mBuffers[i] != null)
                    {
                        mBuffers[i] = null;
                    }
                }
                mLastPayloadSize = 0;
            }
            
            mConnected = false;
        }

        /// <summary>
        /// Start the stream engine and process the incoming packets.
        /// </summary>
        /// <returns>True - Start stream engine successfully, otherwise false.</returns>
        public bool Start()
        {
            string lModeStr;
            bool lContinuous;

            if(mSetup.Role != Setup.cRoleData)
            {
                // Get the current acquisition mode used
                lModeStr = mMode.ValueString;
                lContinuous = lModeStr.Contains("Continuous");

                // Lock the device access
                if(mTLLocked != null)
                {
                    mTLLocked.Value = 1;
                }
                
                try
                {                    
                    // Start the internal thread to retrieve the data
                    mThread = new Thread(DoStreamThreadWork);
                    mStopStreaming = false;
                    mThread.Start();
                }
                catch (Exception)
                {
                    return false;
                }
                
                // Start the stream
                if(mStart != null)
                {
                    mStart.Execute();
                }

                if(!lContinuous)
                {
                    if(mTLLocked != null)
                    {
                        mTLLocked.Value = 0;
                    }
                }                
            }
            else
            {
                try
                {
                    // Start the internal thread to retrieve the data
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
                if (!mDevice.IsConnected)
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
                    catch
                    {
                    }
                }
                if (mTLLocked != null)
                {
                    mTLLocked.Value = 0;
                }
            }

            // Stop the thread retrieving the data
            mStopStreaming = true;
            if (mThread != null)
            {
                // Blocks the calling thread untils the thread terminated
                mThread.Join();
                mThread = null;
            }

            try
            {
                // Abort the stream
                mStream.AbortQueuedBuffers();

                // Dequeue all the buffers
                PvBuffer lBuffer = null;
                while (mStream.RetrieveBuffer(ref lBuffer, ref lOperationResult).IsOK)
                {
                    lBuffer = null;
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

            // Get access to the parameters
            mStart = mDevice.GenParameters.GetCommand("AcquisitionStart");
            mStop = mDevice.GenParameters.GetCommand("AcquisitionStop");
            mMode = mDevice.GenParameters.GetEnum("AcquisitionMode");
            mTLLocked = mDevice.GenParameters.GetInteger("TLParamsLocked");
            lResetTimestamp = mDevice.GenParameters.GetCommand("GevTimestampControlReset");
            lResetStats = mStream.Parameters.GetCommand("Reset");

            aHeight = 0;
            aWidth = 0;

            // Reset the statistic
            lResetStats.Execute();

            // Reset timestamp counter 
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
                    // bad but the only way we can make it work
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
                // Get the current configured payload size
                lPayloadSize = GetPayloadSize(ref aWidth, ref aHeight);
                if (lPayloadSize == 0)
                {
                    // We cannot get the size, the buffer cannot be pre-allocated
                    return false;
                }

                // Manage the size of the buffer allocated to receive the images
                if (mLastPayloadSize == 0)
                {
                    // This is a case where no buffers were allocated
                    if (CreateNewBuffers(lPayloadSize) != true)
                    {
                        return false;
                    }
                    // We need to cache the payload size for future starts...
                    mLastPayloadSize = lPayloadSize;                
                }
                else if (mLastPayloadSize != lPayloadSize)
                {
                    // Reallocate the memory of each of the buffer to be able to 
                    // deal with the new payload size
                    for (int i = 0; i < cRxPoolSize; i++)
                    {
                        try
                        {
                            // Recalling Alloc will make a re-allocate in this case
                            mBuffers[i].Alloc(lPayloadSize);
                        }
                        catch (PvException)
                        {
                            for (int j = 0; j <= i; j++)
                            {
                                mBuffers[j] = null;
                            }
                            // Set mLastPayloadSize to 0 to ensure a clean re-allocation of the memory
                            mLastPayloadSize = 0;
                            return false;
                        }
                    }

                    // We need to cache the payload size for future starts...
                    mLastPayloadSize = lPayloadSize;
                }
            }

            // Queue the buffers into the stream
            for (int i = 0; i < cRxPoolSize; i++)
            {
                mStream.QueueBuffer((PvBuffer) mBuffers[ i ]); 
            }

            return true;
        }

        public bool Store(ref PvConfigurationWriter aWriter)
        {
            string lTemp;

            if(mConnected)
            {
                // Store the device
                if(mSetup.Role == Setup.cRoleCtrlData)
                {
                    lTemp = string.Format("Device( {0}, {1} )", mRow, mColumn);
                    aWriter.Store(mDevice, lTemp);
                }

                // Store the stream
                lTemp = string.Format("Stream( {0}, {1} )", mRow, mColumn);
                aWriter.Store(mStream, lTemp);

                // Store the setup information
                mSetup.Store(ref aWriter, mRow, mColumn);      
            }

            return true;
        }

        public bool Restore(PvConfigurationReader aReader)
        {
            string lTemp;

            if(mConnected)
            {
                Disconnect();
            }

            // Read the setup information first to understand how to deal with the 
            // stream and the device
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

            // Now we need to make the link between the thread and the stream
            if (mSetup.Role == Setup.cRoleCtrlData)
            {
                switch(mSetup.Destination)
                {
                    case Setup.cDestinationUnicastAuto:
                    case Setup.cDestinationUnicastSpecific:
                        mDevice.SetStreamDestination(mStream.LocalIPAddress,  mStream.LocalPort);
                        break;
                    case Setup.cDestinationMulticast:
                        mDevice.SetStreamDestination(mSetup.IPAddress, mSetup.Port);
                        break;
                    default:
                        return false;
                }
            }

            // Now we will retrieve the device information to be able to display it
            PvGenParameterArray lGenDevice = mDevice.GenParameters;

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

            // Model name
            PvGenString lModelName = lGenDevice.GetString("DeviceModelName");
            mModel = lModelName.Value;
            
            // Device name ( User ID )
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


#endregion
    }
}
