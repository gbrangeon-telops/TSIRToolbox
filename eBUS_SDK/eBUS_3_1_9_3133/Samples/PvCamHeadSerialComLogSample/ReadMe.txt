Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.

========================
Using PvCamHeadSerialComLogSample
========================

This sample code illustrates how to create a simple UI application
providing basic device control and streaming capabilities. 

It also allows monitoring the serial communication between a Pleora IP
Engine and its camera head.

It can be used in the process of GigE-enabling your device with the AutoGEV
application. Serial commands provided through your AutoGEV project can
be debugged/monitored using this sample.

1. Introduction

This application is based on the PvSimpleUISample application. It was 
customized/enhanced with the ability of monitoring the serial communication
between your Pleora IP Engine and the camera head.

It was developed as a simple MFC single dialog application. All the code is in 
the main dialog class, except for the streaming/display thread code.

To use the application, connect to a device, open the device controls and change
the value of any parameter your configured with serial commands in AutoGEV. You should
see the exchanges triggered between the IP Engine and the camera head in the
text box below the display. Tx stands for transmitted data, Rx stands for received data.

2. Prerequisites

This sample assumes that:
 * You have a GigE Vision device is connected to the network
 * You have already customized the IP Engine to interact
   with the camera head using the AutoGEV application.

3. Description

3.1 PvCamHeadSerialComLogSampleDlg.cpp

Main application dialog. This is where pretty much everything happens.

3.1 PvCamHeadSerialComLogSample.cpp

Main application. Its only real task is to instantiate the main dialog.

3.2 Thread.cpp

Display thread. Responsible of getting buffers out of the PvPipeline and
displaying them on a the PvDisplay.

