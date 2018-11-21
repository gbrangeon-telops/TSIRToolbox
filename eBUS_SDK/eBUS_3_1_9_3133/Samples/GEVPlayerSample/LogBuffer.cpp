// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "logbuffer.h"

#ifdef WIN32
#include <shlobj.h>
#endif
#include <vector>

#define LOGBUFFER_VERSION ( "1.0.0.0" )

#define TAG_VERSION ( "logbufferversion" )
#define TAG_GENICAMENABLED ( "loggenicamenabled" )
#define TAG_EVENTSENABLED ( "logeventsenabled" )
#define TAG_BUFFERALLENABLED ( "logbufferallenabled" )
#define TAG_BUFFERERRORENABLED ( "logbuffererrorenabled" )
#define TAG_LOGFILENAME ( "logfilename" )
#define TAG_WRITETOFILEENABLED ( "logwritetofileenabled" )
#define TAG_SERIALCOMLOGENABLED ( "logserialcomenabled" )

#define VAL_TRUE ( "true" )
#define VAL_FALSE ( "false" )


#define MAX_LOG_SIZE ( 256 * 1024 )


// =============================================================================
LogBuffer::LogBuffer()
	: mGenICamEnabled( true )
    , mEventsEnabled( false )
	, mBufferAllEnabled( false )
	, mBufferErrorEnabled( true )
	, mWriteToFileEnabled( false )
    , mSerialComLogEnabled( false )
{
    ResetConfig();
    Reset();
}


// =============================================================================
LogBuffer::~LogBuffer()
{
}


// =============================================================================
void LogBuffer::ResetConfig()
{
	mGenICamEnabled = true;
    mEventsEnabled = false;
	mBufferAllEnabled = false;
	mBufferErrorEnabled = true;
	mWriteToFileEnabled = false;
    mSerialComLogEnabled = false;

#ifdef WIN32
    wchar_t lDesktop[ MAX_PATH ];
	SHGetSpecialFolderPath( NULL, lDesktop, CSIDL_DESKTOP, true );

    mFilename = CString( lDesktop ) + _T( "\\EventMonitor.txt" );
#endif //WIN32

#ifdef _LINUX_
    //TODO: Find API call to get the proper default folder
    mFilename = "EventMonitor.txt";
#endif //_LINUX_
}


// =============================================================================
void LogBuffer::Reset()
{
	mMutex.Lock();
	//////////////////////////////////

    mStartTimestamp = ::GetTickCount();
	mBuffer = "";

    //////////////////////////////////
    mMutex.Unlock();
}


// =============================================================================
void LogBuffer::Log( const CString &aItem )
{
    DWORD lElapsed = ::GetTickCount() - mStartTimestamp;
    
    CString lLast = _T( "0" );
	if ( lElapsed >= 0 )
	{
		lLast.Format( _T( "%04i.%03i" ),
			lElapsed / 1000, lElapsed % 1000 );
	}

	CString lStr = lLast + _T( "    " ) + aItem;

	mMutex.Lock();
	//////////////////////////////////

    FILE *lFile = NULL;
	if ( mWriteToFileEnabled )
	{
		// Open file
		_wfopen_s( &lFile, (LPCTSTR)mFilename, _T( "at" ) );
	}

	if ( mBuffer.GetLength() < MAX_LOG_SIZE )
	{
		mBuffer += lStr + _T( "\r\n" );
	}

	if ( lFile != NULL )
	{
		fwprintf( lFile, _T( "%s\n" ), (LPCTSTR)lStr );
        fclose( lFile );
	}

	//////////////////////////////////
	mMutex.Unlock();
}


// =============================================================================
CString LogBuffer::Empty()
{
	mMutex.Lock();
	//////////////////////////////////

	CString lTemp = mBuffer;
	mBuffer = "";

	//////////////////////////////////
	mMutex.Unlock();

	return lTemp;
}


// =============================================================================
void LogBuffer::SetFilename( const CString &aFilename ) 
{ 
	mMutex.Lock();
	//////////////////////////////////

	mFilename = aFilename; 

	//////////////////////////////////
	mMutex.Unlock();
}


// =============================================================================
void LogBuffer::SetWriteToFileEnabled( bool aEnabled ) 
{ 
	mMutex.Lock();
	//////////////////////////////////

	mWriteToFileEnabled = aEnabled; 

	//////////////////////////////////
	mMutex.Unlock();
}


// =============================================================================
void LogBuffer::Save( PvConfigurationWriter &aWriter )
{
    // Save a version string, just in case we need it in the future
    aWriter.Store( LOGBUFFER_VERSION, TAG_VERSION );

	// bool mGenICamEnabled;
    aWriter.Store( mGenICamEnabled ? VAL_TRUE : VAL_FALSE, TAG_GENICAMENABLED );

	// bool mEventsEnabled;
    aWriter.Store( mEventsEnabled ? VAL_TRUE : VAL_FALSE, TAG_EVENTSENABLED );

    // bool mBufferAllEnabled;
    aWriter.Store( mBufferAllEnabled ? VAL_TRUE : VAL_FALSE, TAG_BUFFERALLENABLED );

	// bool mBufferErrorEnabled;
    aWriter.Store( mBufferErrorEnabled ? VAL_TRUE : VAL_FALSE, TAG_BUFFERERRORENABLED );

    // CString mLogFilename
    aWriter.Store( (LPCTSTR)mFilename, TAG_LOGFILENAME );

	// bool mWriteToFileEnabled;
    aWriter.Store( mWriteToFileEnabled ? VAL_TRUE : VAL_FALSE, TAG_WRITETOFILEENABLED );

    aWriter.Store( mSerialComLogEnabled ? VAL_TRUE : VAL_FALSE, TAG_SERIALCOMLOGENABLED );
}


// =============================================================================
void LogBuffer::Load( PvConfigurationReader &aReader )
{
    PvResult lResult;
    PvString lPvStr;

	// Always load from a blank setup!
	ResetConfig();

	// bool mGenICamEnabled;
    lResult = aReader.Restore( TAG_GENICAMENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        mGenICamEnabled = ( lPvStr == VAL_TRUE );
    }

	// bool mEventsEnabled;
    lResult = aReader.Restore( TAG_EVENTSENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        mEventsEnabled = ( lPvStr == VAL_TRUE );
    }

	// bool mBufferAllEnabled;
    lResult = aReader.Restore( TAG_BUFFERALLENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        mBufferAllEnabled = ( lPvStr == VAL_TRUE );
    }

	// bool mBufferErrorEnabled;
    lResult = aReader.Restore( TAG_BUFFERERRORENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        mBufferErrorEnabled = ( lPvStr == VAL_TRUE );
    }

	// CString mFilename;

    lResult = aReader.Restore( TAG_LOGFILENAME, lPvStr );
    if ( lResult.IsOK() )
	{
		SetFilename( lPvStr.GetUnicode() );
	}

	// bool mWriteToFileEnabled;
    lResult = aReader.Restore( TAG_WRITETOFILEENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        SetWriteToFileEnabled( lPvStr == VAL_TRUE );
    }

    lResult = aReader.Restore( TAG_SERIALCOMLOGENABLED, lPvStr );
    if ( lResult.IsOK() )
    {
        mSerialComLogEnabled = ( lPvStr == VAL_TRUE );
    }
}

//
// returns true if it is possible to enable serial communications logging between IP engine and camera
// 
bool LogBuffer::SetSerialComLogEnabled( bool aEnabled, PvGenParameterArray* aParameters )
{
// Get required parameters
    PvGenEnum *lEventSelector = aParameters->GetEnum( "EventSelector" );
    PvGenEnum *lEventNotification = aParameters->GetEnum( "EventNotification" );
    if ( ( lEventSelector == NULL ) || ( lEventNotification == NULL ) )
    {
        mSerialComLogEnabled = false;
		return false;
    }

    // Select IPEngineCamHeadSerialComLog
    PvResult lResult = lEventSelector->SetValue( "IPEngineCamHeadSerialComLog" );
    if ( !lResult.IsOK() )
    {
        mSerialComLogEnabled = false;
		return false;
    }

    // Enable event
    lResult = lEventNotification->SetValue( aEnabled ? "GigEVisionEvent" : "Off" );
    if ( !lResult.IsOK() )
    {
        lResult = lEventNotification->SetValue( aEnabled ? "On" : "Off" );
        if ( !lResult.IsOK() )
        {
            mSerialComLogEnabled = false;
		    return false;
        }
    }
    mSerialComLogEnabled = aEnabled;
    return true;
}


void LogBuffer::CamHeadSerialComLog( const unsigned char* aData, PvUInt32 aDataLength, PvUInt64 aTimestamp )
{
    const char *cPatterns[] = { "Tx: ", "Rx: " };

    CString lBuffer;
    lBuffer.Format( _T( "IP engine <-> camera head\r\nTimestamp: %016llX\r\n" ), aTimestamp );

    PvUInt32 lStart = 0;
    PvUInt32 lEnd = 0;

    for ( int i = 0; ; i++ )
    {
        // Alternate between patterns
        const char *lPattern = cPatterns[ i % 2 ];
        size_t lLength = strlen( lPattern );

        // Find Tx/Rx
        PvUInt32 lIndex = FindNext( aData + lStart, aDataLength - lStart, 
            reinterpret_cast<const unsigned char *>( lPattern ), static_cast<PvUInt32>( lLength ) );

        // Set end to end of data or new found tag
        lEnd = ( lIndex <= 0 ) ? aDataLength : ( lIndex + lStart ) - 1;

        // If we have a start, unpack
        if ( lStart > 0 )
        {
            // Unpack Rx
            const char *lPattern = cPatterns[ ( i + 1 ) % 2 ]; // Compensate, we're always late by one
            lBuffer += Unpack( lPattern, aData + lStart, lEnd - lStart, 1 );
        }

        // If nothing found, end of data
        if ( lIndex <= 0 )
        {
            break;
        }

        // Update start before going for another round
        lStart = ( lIndex + lStart ) + static_cast<PvUInt32>( lLength );
        lEnd = 0;
    }

    CString lLogBuffer = lBuffer;
    Log( lLogBuffer );
}



// =============================================================================
PvUInt32 LogBuffer::FindNext( const unsigned char *aData, PvUInt32 aDataLength, 
    const unsigned char *aPattern, PvUInt32 aPatternLength )
{
    PvUInt32 j = 0;
    const unsigned char *lData = aData;
    for ( PvUInt32 i = 0; i < aDataLength; i++ )
    {
        if ( lData[ i ] == aPattern[ j ] )
        {
            j++;
            if ( j == aPatternLength )
            {
                return ( i - j + 1 );
            }
        }
        else
        {
            j = 0;
        }
    }

    return 0;
}

#define TERMINAL_COLUMNS ( 8 )
// =============================================================================
CString LogBuffer::Unpack( const char *aPattern, const unsigned char *aData, PvUInt32 aDataLength, int aGap )
{
    CString lBuffer;

    size_t lLength = strlen( aPattern );
    for ( size_t j = 0; j < lLength; j++ )
    {
        lBuffer += aPattern[ j ];
    }

    std::vector<unsigned char> lLine;
    PvUInt32 i = 0;
    for ( ;; ) // Skip NULLs
    {
        unsigned char lChar = aData[ i ];

        // Format, append hex value
        CString lHex;
        lHex.Format( _T( "%02X " ), lChar );
        lBuffer += lHex;

        // 0x20: chars below or equal to space, cannot be displayed
        // 0x7F: delete
        // 0xAF: extended ASCII, cannot be displayed
        if ( ( lChar <= 0x20 ) || ( lChar > 0x7F ) || ( lChar == 127 ) )
        {
            lChar = '.';
        }

        // Save character for latter
        lLine.push_back( lChar );

        // Next character, skipping possible inter-char
        i += 1 + aGap;

        // If end of terminal line reached - or end of data output readable chars
        if ( ( ( ( i / ( 1 + aGap ) ) % TERMINAL_COLUMNS ) == 0 ) || ( i >= aDataLength ) )
        {
            int lMissing = TERMINAL_COLUMNS - ( ( ( i - ( 1 + aGap ) ) / ( 1 + aGap ) ) % TERMINAL_COLUMNS );
            for ( int k = 0; k < lMissing + 1; k++ )
            {
                lBuffer += _T( "   " );
            }
            
            // Flush characters buffer
            std::vector<unsigned char>::iterator lIt = lLine.begin();
            while ( lIt != lLine.end() )
            {
                lBuffer += *lIt;
                lIt++;
            }
            lLine.clear();

            // Still data? If so, start new line
            if ( i < aDataLength )
            {
                // New line
                lBuffer += _T( "\r\n" );

                // Align data
                for ( size_t k = 0; k < lLength; k++ )
                {
                    lBuffer += _T( " " );
                }
            }
        }

        // Reached end of data?
        if ( i >= aDataLength )
        {
            break;
        }
    }

    lBuffer += _T( "\r\n" );

    return lBuffer;
}


