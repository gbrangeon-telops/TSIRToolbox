// *****************************************************************************
//
// Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <PvSampleTransmitterConfig.h>


// Application config
class Config : public PvSampleTransmitterConfig
{
public:

    Config()
    {
        SetDefaults();
    }

    ~Config()
    {
    }

    void ParseCommandLine( int aCount, const char **aArgs )
    {
        if ( ParseOptionFlag( aCount, aArgs, "--help" ) )
        {
            PrintHelp();
            exit( 0 );
        }

        PvSampleTransmitterConfig::ParseCommandLine( aCount, aArgs );
    }
    
    void PrintHelp()
    {
        cout << "This utility transmits screen captures to a given destination using the GigEVision Streaming Protocol (GVSP)." << endl << endl << endl;

        PvSampleTransmitterConfig::PrintHelp();
    }

private:

};


#endif // __CONFIGURATION_H__

