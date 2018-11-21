// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvPipeline.h>


class GEVPlayerDlg;


class BufferOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(BufferOptionsDlg)

public:

	BufferOptionsDlg( GEVPlayerDlg *aGEVPlayerDlg, PvPipeline *aPipeline );   // standard constructor
	virtual ~BufferOptionsDlg();

    // Persistence
    static void Save( PvConfigurationWriter &aWriter, PvPipeline *aPipeline );
    static void Load( PvConfigurationReader &aReader, PvPipeline *aPipeline );

	enum { IDD = IDD_BUFFER_OPTIONS };

protected:

	static bool StrToUInt32( const CString &aString, PvUInt32 &aValue );
	static bool EditToUInt32( CEdit &aEdit, PvUInt32 &aValue );

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

	GEVPlayerDlg *mGEVPlayerDlg;
	PvPipeline* mPipeline;

	CEdit mBufferCountEdit;
	CEdit mBufferSizeEdit;
    CButton mAutoResizeCheck;

	PvUInt32 mOriginalBufferCount;
	PvUInt32 mOriginalBufferSize;
    bool mOriginalAutoResize;
};



