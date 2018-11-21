// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDeviceFinderWnd.h>

///
/// \class StreamDeviceFinderWnd
///
/// \brief Overload of PvDeviceFinderWnd to filter out what cannot send data
///
class StreamDeviceFinderWnd
    : public PvDeviceFinderWnd
{
public :

    ///
    /// \brief Constructor
    ///

    StreamDeviceFinderWnd( const CString &aIfAddress )
        : PvDeviceFinderWnd()
		, mIfAddress( aIfAddress )
    {
    }

    ///
    /// \brief Destructor
    ///

    virtual ~StreamDeviceFinderWnd()
    {
    }

    ///
    /// \brief Filter the device found to only enumerate the one that can stream and is not our virtual device.
    ///
    /// \param aDI Devide discovered.
    /// \param true to keep the device.
	///

	virtual bool OnFound( PvDeviceInfo *aDI )
    {
		if ( ( aDI->GetIPAddress() != (LPCTSTR)mIfAddress ) &&
			 ( aDI->GetClass() == PvDeviceClassTransmitter ) )
		{
			return true;
		}

		return false;
    }

private:

	CString mIfAddress;
};


