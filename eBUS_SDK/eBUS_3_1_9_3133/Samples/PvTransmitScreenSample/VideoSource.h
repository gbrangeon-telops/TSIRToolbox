// *****************************************************************************
//
// Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __VIDEO_SOURCE_H__
#define __VIDEO_SOURCE_H__

#include <PvBuffer.h>
#include <PvBufferConverter.h>

#include <d3d9.h>
#include <D3dx9tex.h>

class VideoSource
{
public:
    VideoSource();
    virtual ~VideoSource();

    virtual void CaptureScreen( PvBuffer* aBuffer );
    virtual PvUInt32 GetWidth();
    virtual PvUInt32 GetHeight();
    virtual PvPixelType GetPixelFormat();

private:

    PvPixelType mPixelFormat;
    PvUInt32 mWidth;
    PvUInt32 mHeight;
    LPDIRECT3D9             mD3D; // Used to create the D3DDevice
    LPDIRECT3DDEVICE9       mD3DDevice; // Our rendering device
    IDirect3DSurface9 * mSurface;
    PvBufferConverter mConverter;
};

#endif //__VIDEO_SOURCE_H__

