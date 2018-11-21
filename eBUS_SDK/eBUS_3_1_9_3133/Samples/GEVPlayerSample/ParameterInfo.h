// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvGenParameterArray.h>

#include "LogBuffer.h"


class ParameterInfo : public PvGenEventSink
{
public:

	ParameterInfo();
	~ParameterInfo();

	void Initialize( PvGenParameter *aParameter, LogBuffer *aLogBuffer );
	void Reset();

	// Used to temporarely disabling node map access
	bool IsEnabled() const { return mEnabled; }
	void Enable( bool aEnabled ) { mEnabled = aEnabled; }

private:

	void OnParameterUpdate( PvGenParameter *aParameter );

	void SetLastValue();
	void LogToBuffer();

	PvGenParameter *mParameter;
	CString mLastValue;

	LogBuffer *mLogBuffer;

	bool mEnabled;
};

