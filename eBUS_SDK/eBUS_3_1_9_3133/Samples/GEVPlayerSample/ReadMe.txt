Copyright (c) 2010, Pleora Technologies Inc., All rights reserved. 

                               =============
                                  PureGEV
                               =============

This sample is contains the source code for the GEVPlayer application, used to detect, 
connect and configure GEV devices.

1. Introduction

This code illustrates how to use the PureGEV SDK in 
depth.

2. Pre-conditions

This sample assumes that:

 * You have used the GEVPlayer application before
 * You have a very good understanding of C++ and Microsoft's MFC foundation classes

3. Description

3.1. AboutBox.cpp

This is the source code the About dialog in GEVPlayer. It is accessible from the 
"Help > About GEVPlayer..." menu option.

3.2. BitmapButton.cpp

Class used for the play and stop buttons in the GEVPlayer UI.

3.3. BufferOptionsDlg.cpp

This is the source code for the Buffer Options dialog in GEVPlayer. It is accessible from the 
"Tools > Buffer Options..." menu option.

3.4. ConnectionThread.cpp

Thread that is created to asynchronously perform the connection procedure and pass messages to ProgressDlg (See ProgressDlg.cpp).

3.5. EventMonitorDlg.cpp
This is the source code for the Event Monitor dialog in GEVPlayer. It is accessible from the 
"Tools > Event Monitor..." menu option.

3.6. FilteringDlg.cpp

This is the source code for the Image Filtering dialog in GEVPlayer. It is accessible from the 
"Tools > Image Filtering..." menu option.

3.7. GEVPlayer.cpp

This is the CWinApp class that brings up the GEVPlayer dialog on InitInstance.

3.8. GEVPlayerDlg.cpp

The GEVPlayer dialog class. This is where the majority of the code resides as it contains all the UI elements that are visible when you open GEVPlayer.

3.9. HTMLDialog.cpp

Parent class of the about box and splash page that uses an HTML window to display UI elements for those dialogs.

3.10. ImageSaveDlg.cpp

This is the source code for the Image Save dialog in GEVPlayer. It is accessible from the 
"Tools > Save Images..." menu option.

3.11. LoadingThread.cpp

Thread that is created to asynchronously load from a pvcfg file and perform the connection procedure while passing messages to ProgressDlg (See ProgressDlg.cpp).

3.12. LogBuffer.cpp

Used to log events to the Event Monitor dialog (see EventMonitorDlg.cpp).

3.13. ParameterInfo.cpp

Used by the event monitor dialog to log GenICam events.

3.14. ProgressDlg.cpp

Dialog that appears when connecting to a device or while loading or saving a configuration file.

3.15. RegisterInterfaceDlg.cpp

Register interface dialog that appears when one presses Ctrl+Alt+R.

3.16. SavingThread.cpp

Thread that is created to asynchronously save to a pvcfg file while passing messages to ProgressDlg (See ProgressDlg.cpp).

3.17. SetupDlg.cpp

This is the source code for the Setup dialog in GEVPlayer. It is accessible from the 
"Tools > Setup..." menu option.

3.18. SplashPage.cpp

Dialog that appears for a moment while GEVPlayer is starting.

3.19. Thread.cpp

Class that wraps the win32 thread functionality to be used by ThreadDisplay.

3.20. ThreadDisplay.cpp

Thread that pulls images out of the PvPipeline and displays (and/or saves) them.

3.21. stdafx.cpp

This a standard MFC application generated source file.

