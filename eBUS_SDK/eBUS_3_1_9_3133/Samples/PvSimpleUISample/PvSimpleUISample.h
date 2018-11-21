// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
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

class PvSimpleUISampleApp : public CWinApp
{
public:
	PvSimpleUISampleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

extern PvSimpleUISampleApp theApp;