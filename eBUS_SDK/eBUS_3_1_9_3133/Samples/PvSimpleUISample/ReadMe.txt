Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.

========================
Using PvSimpleUISample
========================

This sample code illustrates how to create a simple UI application
using PureGEV to connect to a device, receive and display a stream.

1. Introduction

Using the PvDevice and PvStream classes, this sample shows how to:
 * Select a device using PvDeviceFinderWnd
 * Connect to a device (PvDevice)
 * Open a stream (PvStream)
 * Manage acquisition on a device (start/stop)
 * Receive images using a PvStream object
 * Display images using PvDisplayWnd object
 * Use PvGenBrowserWnd objects to browse/edit PvGenParameterArray objects

2. Prerequisites

This samples assumes that:
 * You have a GigE Vision device is connected to the network

3. Files description

3.1 PvSimpleUISample.cpp, PvSimpleUISample.h

Application, inherits from MFC CWinApp class. This is where the application starts. Mostly intantiates and opens a PvSimpleUISampleDlg dialog.

3.2 PvSimpleUISampleDlg.cpp, PvSimpleUISample.h

Main dialog of the application. Inherits from the MFC CDialog class. This is where most of the sample application lives. Owns the PvDevice, PvStream and manages most of the UI.

3.3 Thread.cpp, Thread.h

Display/acquisition thread taking buffers out of PvPipeline and into PvDisplayWnd.

3.4 Resource.h, PvSimpleUISample.ico, PvSimpleUISample.rc, PvSimpleUISample.rc2

Resource files required for a MFC GUI-based project.

3.5 stdafx.cpp, stdafx.h

Pre-compiled header files.



