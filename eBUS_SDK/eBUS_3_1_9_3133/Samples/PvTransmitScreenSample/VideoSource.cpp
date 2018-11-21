// *****************************************************************************
//
// Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "VideoSource.h"


VideoSource::VideoSource()
{

    D3DDISPLAYMODE lDisplayMode;

    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( mD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
    {
           
    }

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS lD3Dpp; 
    ZeroMemory( &lD3Dpp, sizeof(lD3Dpp) );
    lD3Dpp.Windowed = TRUE;
    lD3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    lD3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( mD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, ::GetDesktopWindow(),
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &lD3Dpp, &mD3DDevice ) ) )
    {
       
    }

    mD3DDevice->GetDisplayMode( 0, &lDisplayMode );
    mD3DDevice->CreateOffscreenPlainSurface( lDisplayMode.Width, lDisplayMode.Height,
    D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &mSurface, NULL );

    mWidth = lDisplayMode.Width;
    mHeight = lDisplayMode.Height;
    mPixelFormat = PvPixelBGRa8;
}


VideoSource::~VideoSource()
{
    if ( mSurface != NULL )
    {
        mSurface->Release();
    }
    if( mD3DDevice != NULL )
    {
        mD3DDevice->Release();
    }
    if( mD3D != NULL)
    {
        mD3D->Release();
    }
}


void VideoSource::CaptureScreen( PvBuffer* aBuffer )
{
    if( SUCCEEDED( mD3DDevice->GetFrontBufferData(0, mSurface) ) )
    {
        D3DLOCKED_RECT lLockedRect;
        PvBuffer lBufferSrc;
        PvBuffer lBufferDest;
        PvImage* lImageSrc;
        PvImage* lImageDest;
        PvImage* lImageOut;

        lImageDest = lBufferDest.GetImage();
        lImageOut = aBuffer->GetImage();
        lImageSrc = lBufferSrc.GetImage();
        lImageDest->Attach( aBuffer->GetDataPointer(), lImageOut->GetWidth(), lImageOut->GetHeight(), PV_PIXEL_WIN_RGB32 );

        mSurface->LockRect(&lLockedRect,NULL,D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_NOSYSLOCK|D3DLOCK_READONLY);

        lImageSrc->Attach( lLockedRect.pBits, mWidth, mHeight, mPixelFormat );
        mConverter.Convert( &lBufferSrc, &lBufferDest, false );
        lImageSrc->Detach();

        mSurface->UnlockRect();

        lImageDest->Detach();
    }
}

PvUInt32 VideoSource::GetWidth()
{
    return mWidth;
}

PvUInt32 VideoSource::GetHeight()
{
    return mHeight;
}

PvPixelType VideoSource::GetPixelFormat()
{
    return PvPixelRGBa8;
}


