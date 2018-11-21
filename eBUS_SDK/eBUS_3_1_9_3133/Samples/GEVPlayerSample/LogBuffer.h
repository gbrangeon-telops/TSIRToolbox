// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>
#include <PvGenParameterArray.h>


class LogBuffer
{
public:

	LogBuffer();
	~LogBuffer();

	void Log( const CString &aItem );
	CString Empty();

	void Reset();

	void SetGenICamEnabled( bool aEnabled ) { mGenICamEnabled = aEnabled; }
	void SetEventsEnabled( bool aEnabled ) { mEventsEnabled = aEnabled; }
	void SetBufferErrorEnabled( bool aEnabled ) { mBufferErrorEnabled = aEnabled; }
	void SetBufferAllEnabled( bool aEnabled ) { mBufferAllEnabled = aEnabled; }
	void SetFilename( const CString &aFilename );
	void SetWriteToFileEnabled( bool aEnabled );
    bool SetSerialComLogEnabled( bool aEnabled, PvGenParameterArray* aParameters );

	bool IsGenICamEnabled() const { return mGenICamEnabled; }
	bool IsEventsEnabled() const { return mEventsEnabled; }
	bool IsBufferErrorEnabled() const { return mBufferErrorEnabled; }
	bool IsBufferAllEnabled() const { return mBufferAllEnabled; }
	const CString &GetFilename() const { return mFilename; }
	bool IsWriteToFileEnabled() const { return mWriteToFileEnabled; }
    bool IsSerialComLogEnabled() const { return mSerialComLogEnabled; }

    // Persistence
    void Save( PvConfigurationWriter &aWriter );
    void Load( PvConfigurationReader &aReader );

    void CamHeadSerialComLog( const unsigned char* aData, PvUInt32 aDataLength, PvUInt64 aTimestamp );

    static CString Unpack( const char *aPattern, const unsigned char *aData, PvUInt32 aDataLength, int aGap );

protected:

	void ResetConfig();
    PvUInt32 FindNext( const unsigned char *aData, PvUInt32 aDataLength, 
    const unsigned char *aPattern, PvUInt32 aPatternLength );

private:

	CMutex mMutex;

	DWORD mStartTimestamp;
	CString mBuffer;

	bool mGenICamEnabled;
    bool mEventsEnabled;
	bool mBufferErrorEnabled;
	bool mBufferAllEnabled;
    bool mSerialComLogEnabled;

	CString mFilename;
	bool mWriteToFileEnabled;
};

