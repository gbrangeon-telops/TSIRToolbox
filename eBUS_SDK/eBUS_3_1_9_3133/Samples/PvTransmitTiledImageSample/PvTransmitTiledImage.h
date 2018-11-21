// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPvTransmitTiledImageApp:
// See PvTransmitTiledImage.cpp for the implementation of this class
//
class CPvTransmitTiledImageApp : public CWinApp
{
public:
	CPvTransmitTiledImageApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPvTransmitTiledImageApp theApp;