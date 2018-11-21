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


// CGEVPlayerApp:
// See GEVPlayer.cpp for the implementation of this class
//

class CGEVPlayerApp : public CWinApp
{
public:
    CGEVPlayerApp();

// Overrides
    public:
    virtual BOOL InitInstance();

// Implementation

//	BOOL PreTranslateMessage(MSG* pMsg);
//	BOOL OnIdle(LONG lCount);
    DECLARE_MESSAGE_MAP()
};

extern CGEVPlayerApp theApp;