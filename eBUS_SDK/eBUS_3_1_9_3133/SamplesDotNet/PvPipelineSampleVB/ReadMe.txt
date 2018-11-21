Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.

====================================
Using PvGenParameterArraySampleVB
====================================

This VB sample shows how to use the PvPipeline class for image acquisition.

1. Description

This sample uses a simple state machine driven a timer. It moves from one state
to the other when the previous operation is completed (or when the user clicks the
Stop button when streaming).

Each step tries to clearly show what is required in order to setup and maintain
a stream of images using a PvPipeline:

Step 1: connecting
Step 2: configuring
Step 3: start streaming
Step 4: streaming
Step 5: stop streaming
Step 6: disconnnecting

Step 1 is initiating when starting the application - the user is prompted with a 
PvDeviceFinderWnd in order to select which device to use.

When step 6 is completed, the application terminates.
