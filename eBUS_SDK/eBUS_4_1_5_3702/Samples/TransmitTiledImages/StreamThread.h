// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>
#include <PvGenBrowserTabbedWnd.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>

#include "Thread.h"
#include "Constants.h"
#include "CurrentBuffersTable.h"
#include "SmartBuffer.h"
#include "ProtectedQueue.h"
#include "Setup.h"

///
/// \class StreamThread
///
/// \brief Thread managing the integration with a streaming device source.
///
class StreamThread 
    : public Thread
    , public PvDeviceEventSink
{
public:
    ///
    /// \brief Constructor
    ///
    StreamThread();

    ///
    /// \brief Destructor
    ///
    virtual ~StreamThread();

    ///
    /// \brief Initialize the stream memory class.
    /// Must be called once at the beginning
    ///
    /// \param aMainWnd Parent GUI window
    /// \param aRow Row tile position 
    /// \param aColumn Column tile position
    /// \param aCurrentBuffersTable Current Buffers Table used as shared data destination
    ///
    /// \return false on error
    ///
    bool Initialize( CWnd* aMainWnd, int32_t aRow, int32_t aColumn, CurrentBuffersTable* aCurrentBuffersTable );

    ///
    /// \brief Connect to the image source
    ///
    /// \return false on error
    ///
    PvResult Connect( const CString &aIfAddress, CWnd *aParent );

    ///
    /// \brief Disconnect to the image source
    ///
    void Disconnect();

    ///
    /// \brief Configure the internal thread memory and buffers
    /// 
    /// \param aWidth The current configured width of the image
    /// \param aHeight The current configured height of the image
    /// 
    /// \return false on error
    ///
    bool Configure( unsigned int& aWidth, unsigned int& aHeight );
    
    ///
    /// \brief Start the stream engine and process the incoming packets
    ///
    /// \return false on error
    ///
    virtual bool Start();
    
    ///
    /// \brief Stop the stream engine
    ///
    /// \return false on error
    ///
    virtual bool Stop();

    ///
    /// \bried Display the configuration page to set the option of the 
    /// connected image source
    ///
    void ShowOptions();

    ///
    /// \brief Get connection state
    ///
    /// \return true when connected
    inline bool IsConnected()
    {
        return mConnected;
    }

    ///
    /// \brief MAC address of the connected image source
    ///
    /// \return MAC address
    ///
    inline CString& GetMACAddress()
    {
        return mMACAddress;
    }

    ///
    /// \brief IP address of the connected image source
    ///
    /// \return IP address
    ///
    inline CString& GetIPAddress()
    {
        return mIPAddress;
    }

    ///
    /// \brief Model of the connected image source
    ///
    /// \return Model
    ///
    inline CString& GetModel()
    {
        return mModel;
    }

    ///
    /// \brief User defined name of the connected image source
    ///
    /// \return User DefinedName
    ///
    inline CString& GetUserDefinedName()
    {
        return mUserDefinedName;
    }

    ///
    /// \brief Store the current connection states in the writer
    ///
    /// \param aWriter The destination writer
    ///
    /// \return false on error
    ///
    bool Store( PvConfigurationWriter& aWriter );

    /// 
    /// \brief Restore the current connection state from a reader
    ///
    /// \param aReader The information source
    ///
    /// \return false on error
    ///
    bool Restore( PvConfigurationReader& aReader );

protected:

    ///
    /// \brief Notify the main application that the connection is lost
    ///
    /// \param aDevice Device with the lost connection
    ///
    void OnLinkDisconnected( PvDevice *aDevice );

private:

    // Thread main acquisition loop
    virtual DWORD Function();

    // Device to stream from
    PvDeviceGEV mDeviceGEV;
    
    // Stream recording the data
    PvStreamGEV mStreamGEV;

    // Control command for the device streaming
    PvGenCommand* mStart;
    PvGenCommand* mStop;
    PvGenEnum* mMode;
    PvGenInteger* mTLLocked;

    // Current information on the stream connection
    Setup mSetup;
    CString mMACAddress;
    CString mIPAddress;
    CString mModel;
    CString mUserDefinedName;

    // Current state of the connection
    bool mConnected;

    // This is the unique index of the thread
    // Use to map to the current image table
    int mRow;
    int mColumn;

    // Object that keep track of the last buffer for each stream
    CurrentBuffersTable* mCurrentBuffersTable;

    // GUI for the configuration of the stream
    PvGenBrowserTabbedWnd mGenBrowserTabbedWnd;

    // Cache the last payload size to avoid resizing the buffer
    uint32_t mLastPayloadSize;

    // Storage for the allocated buffer for this application
    SmartBuffer* mBuffers[ RX_POOL_SIZE ];

    // Parent window
    CWnd* mMainWnd;

    // Return queue for the buffers 
    ProtectedQueue<SmartBuffer> mReturnBufferQueues;

    ///
    /// Create the smart buffers with the given payload size
    ///
    /// \param aPayloadSize The payload size used as information for the buffer creation
    ///
    /// \return false on error
    ///
    bool CreateNewBuffers( unsigned int aPayloadSize );
};