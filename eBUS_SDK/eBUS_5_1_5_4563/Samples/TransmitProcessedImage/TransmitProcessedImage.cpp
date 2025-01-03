// *****************************************************************************
//
// Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// -----------------------------------------------------------------------------
// This sample code illustrates how to:
// * Receive video from a device using PvStream and PvPipeline
// * Print text on it
// * Transmit it using PvTransmitterGEV
//
// Using the PvSystem and PvInterface class, this sample enumerates available 
// interfaces (network adapters) and selects the first interface with a valid IP 
// address to transmit from.
//
// Using the PvVirtualDeviceGEV class, it listens for device discovery requests so 
// that it can be detected by receiving applications.
// Using the PvBuffer class, it shows how to allocate a set of buffers with a 
// given width, height and pixel format for use in receiving and transmitting 
// video.
//
// Using PvDeviceFinderWnd, the user can select a device to receive from.
// Using PvStream and PvPipeline, images are received from a device on 
// channel 0.
//
// Using PvBufferConverter, the incoming video is converted into an RGB format.
// The text is printed onto the video using GDI+.
//
// Using the PvTransmitterGEV class, the transformed image is continuously 
// transmitted to the specified destination.
// 
// By default, this sample prompts for a device to receive from, begins 
// receiving from that device, converts the video to RGB24, renders
// text on it and finally transmits it from the first valid interface 
// to multicast address 239.192.1.1:1042. However, it can be used in many 
// different ways by providing it with optional command line arguments. For 
// further details on the command line options that are available, run the 
// sample with the argument --help.
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvTransmitterGEV.h>
#include <PvVirtualDeviceGEV.h>
#include <PvFPSStabilizer.h>

#include <list>
typedef std::list<PvBuffer *> PvBufferList;

#include "Configuration.h"
#include "VideoSource.h"


PV_INIT_SIGNAL_HANDLER();


int main( int aCount, const char ** aArgs )
{
    PV_SAMPLE_INIT();

    // Creates default configuration, parse command line parameters.
    Configuration lConfig;
    bool lRetValOk = true;

    lConfig.ParseCommandLine( aCount, aArgs );

    // Create video source.
    VideoSource lSource( lConfig.GetConnectionID() );

    // Connect to the device.
    lRetValOk = lSource.Connect();
    if ( !lRetValOk )
    {
        cout << "Press any key to exit." << endl;
        PvWaitForKeyPress();
        return 1;
    }

    // Start acquisition.
    lRetValOk = lSource.StartAcquisition();
    if ( !lRetValOk )
    {
        cout << "Press any key to exit." << endl;
        PvWaitForKeyPress();
        return 1;
    }

    // Get video source properties.
    uint32_t lWidth = lSource.GetWidth();
    uint32_t lHeight = lSource.GetHeight();
    PvPixelType lPixelFormat = lSource.GetPixelFormat();
    uint32_t lSize = lWidth * lHeight;

    // Allocate transmit buffers.
    PvBufferList lBuffers;
    PvBufferList lFreeBuffers;
    for ( uint32_t i = 0; i < lConfig.GetBufferCount(); i++ )
    {
        // Allocates new buffer.
        PvBuffer *lBuffer = new PvBuffer();
        lBuffer->SetID( i );

        // Add to both buffer list and free buffer list.
        lBuffers.push_back( lBuffer );
        lFreeBuffers.push_back( lBuffer );
    }

    // Create transmitter, set packet size.
    PvTransmitterGEV lTransmitter;
    lTransmitter.SetPacketSize( lConfig.GetPacketSize() );

    // Create virtual device (used for discovery).
    PvVirtualDeviceGEV lDevice;
    lDevice.StartListening( lConfig.GetSourceAddress() );

    cout << "Listening for device discovery requests on " << lConfig.GetSourceAddress() << endl;

    // Open transmitter - sets destination and source.
    PvResult lResult = lTransmitter.Open( 
        lConfig.GetDestinationAddress(), lConfig.GetDestinationPort(), 
        lConfig.GetSourceAddress(), lConfig.GetSourcePort() );
    if ( !lResult.IsOK() )
    {
        cout << "Failed to open a connection to the transmitter." << endl;
        return 1;
    }

    cout << "Transmission stream opened:" << endl;
    cout << "Source: " << lTransmitter.GetSourceIPAddress().GetAscii() << " port " << lTransmitter.GetSourcePort() << endl;
    cout << "Destination: " << lConfig.GetDestinationAddress() << " port " << lConfig.GetDestinationPort() << endl; 

    if ( !lConfig.GetSilent() )
    {
        cout << "Press any key to begin transmitting.\r";
        PvWaitForKeyPress();
    }

    cout << "Press any key to stop transmitting." << endl;

    // Set maximum throughput (just to even out traffic, as we control throughput at the source).
    if ( lConfig.GetFPS() != 0 )
    {
        // Multiply image size (in bits) by FPS
        float lMax = static_cast<float>( lSize ) * 8;
        lMax *= lConfig.GetFPS();

        // Since we control throughput at the source, make sure maximum throughput is slightly
        // higher than what we need. We want to even out packet traffic, not slow down source frame rate.
        lMax *= 1.1f;

        // Set max throughput.
        lTransmitter.SetMaxPayloadThroughput( lMax );
    }

    char lDoodle[] = "|\\-|-/";
    int lDoodleIndex = 0;

    // Reset transmitter stats.
    lTransmitter.ResetStats();

    // Used to transmit at a steady frame rate.
    PvFPSStabilizer lStabilizer;

    // Seed used to generate the test pattern.
    unsigned char lSeed = 0;

    // Acquisition/transmission loop.
    while ( !PvKbHit() )
    {
        // Step 1: If timing is right to meet desired FPS, generate pattern, transmit.
        if ( ( lConfig.GetFPS() == 0 ) || lStabilizer.IsTimeToDisplay( (uint32_t)lConfig.GetFPS() ) )
        {
            // Are there buffers available for transmission?
            if ( lFreeBuffers.size() > 0 )
            {
                // Retrieve buffer from list
                PvBuffer *lBuffer = lFreeBuffers.front();
                lFreeBuffers.pop_front();

                // Get, transform and copy image into buffer.
                if ( lSource.FillBuffer( lBuffer, &lTransmitter ) )
                {
                    // Queue the buffer for transmission.
                    lTransmitter.QueueBuffer( lBuffer );
                }
                else
                {
                    // No source image, put buffer back in free list.
                    lFreeBuffers.push_front( lBuffer );
                }
            }
        }

        // Step 2: Retrieve free buffer(s), display stats and requeue.
        PvBuffer *lBuffer = NULL;
        while ( lTransmitter.RetrieveFreeBuffer( &lBuffer, 0 ).IsOK() )
        {
            // Queue buffers back in available buffer list.
            lFreeBuffers.push_back( lBuffer );

            // Buffer transmission complete, display stats.
            cout << fixed << setprecision( 1 );
            cout << lDoodle[ lDoodleIndex ] << " ";
            cout << "Transmitted " << lTransmitter.GetBlocksTransmitted() << " blocks ";
            cout << "at " << lTransmitter.GetAverageTransmissionRate() << " ";
            cout << "(" << lTransmitter.GetInstantaneousTransmissionRate() << ") FPS ";
            cout << lTransmitter.GetAveragePayloadThroughput() / 1000000.0f << " ";
            cout << "(" << lTransmitter.GetInstantaneousPayloadThroughput() / 1000000.0f << ") Mb/s  \r";
            ++lDoodleIndex %= 6;
        }
    }

    // Close transmitter (will also abort buffers).
    lTransmitter.Close();

    // Free buffers.
    PvBufferList::iterator lIt = lBuffers.begin();
    while ( lIt != lBuffers.end() )
    {
        delete ( *lIt );
        lIt++;
    }

    // Stop video source.
    lRetValOk = lSource.StopAcquisition();
    if ( lRetValOk )
    {
        // Disconnect the device.
        lRetValOk = lSource.Disconnect();
        if ( lRetValOk )
        {
            // Stop virtual device.
            lDevice.StopListening();
        }
    }

    if ( !lRetValOk )
    {
        cout << "Press any key to exit." << endl;
        PvWaitForKeyPress();
        return 1;
    }

    PV_SAMPLE_TERMINATE();

    return 0;
}


