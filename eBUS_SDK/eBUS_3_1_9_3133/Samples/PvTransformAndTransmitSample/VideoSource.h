// *****************************************************************************
//
// Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __VIDEO_SOURCE_H__
#define __VIDEO_SOURCE_H__

#ifdef WIN32 

#pragma comment( lib, "gdiplus" )

#include <PvSampleUtils.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvBufferConverter.h>

#include <map>

#include <windows.h>
#include <gdiplus.h>

typedef map<PvUInt32, Gdiplus::Bitmap *> gBMap;
class PvTransmitterRaw;


// Video source
class VideoSource
{
public:

    VideoSource( const char *aDeviceAddress );
    ~VideoSource();

    PvUInt32 GetWidth();
    PvUInt32 GetHeight();
    PvPixelType GetPixelFormat();

    bool Connect();
    bool StartAcquisition();
    bool StopAcquisition();
    bool Disconnect();

    bool FillBuffer( PvBuffer *aBuffer, PvTransmitterRaw *aTransmitter );

private:

    Gdiplus::Bitmap *GetImageForBuffer( PvBuffer *aBuffer );
    void Draw( PvTransmitterRaw *aTransmitter, Gdiplus::Bitmap *aImage );

    std::string mDeviceAddress;

    PvDevice mDevice;
    PvStream mStream;
    PvPipeline *mPipeline;

    PvBufferConverter mConverter;

    gBMap mMap;
	ULONG_PTR mGdiplusToken;
};

#endif // WIN32

#endif // __VIDEO_SOURCE_H__

