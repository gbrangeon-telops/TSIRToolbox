// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDisplayWnd.h>
#include <PvBuffer.h>
#include <PvDevice.h>

class PvCamHeadSerialComLogSampleDlg;

class Thread
{
public:

	Thread( PvCamHeadSerialComLogSampleDlg *aDlg, PvDisplayWnd *aDisplayWnd, PvDevice *aDevice, PvPipeline *aPipeline );
	~Thread();

	void Start();
	void Stop();

	void SetPriority( int aPriority );
	int GetPriority() const;

	bool IsDone();
	DWORD GetReturnValue();

protected:

	static unsigned long WINAPI Link( void *aParam );
	virtual DWORD Function();

	bool IsStopping() const;

private:

	HANDLE mHandle;
	DWORD mID;

	bool mStop;

	DWORD mReturnValue;

	// We do not own these objects
	PvCamHeadSerialComLogSampleDlg *mMainDlg;
	PvDisplayWnd *mDisplayWnd;
    PvDevice *mDevice;
    PvPipeline * mPipeline;
};
