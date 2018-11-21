// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// PvSimpleUISampleApp:
// See PvSimpleUISample.cpp for the implementation of this class
//

class PvCamHeadSerialComLogSampleApp : public CWinApp
{
public:
	PvCamHeadSerialComLogSampleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern PvCamHeadSerialComLogSampleApp theApp;

