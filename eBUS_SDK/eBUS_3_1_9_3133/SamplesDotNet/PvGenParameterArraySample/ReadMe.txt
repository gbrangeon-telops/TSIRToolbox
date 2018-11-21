Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.

====================================
Using PvGenParameterArraySample
====================================

This C# sample shows how to use the PvGenParameter of PvGenParameterArray.

1. Description

On startup, this sample shows a PvDeviceFinderWnd form prompting you to select a 
device. A PvDevice is then connected to the GigE Vision Device.

Parameters are grouped in categories in the left of the application. On the right
information about the currently selected parameter is displayed.

The GenICam parameters of the device are all displayed in a custom GUI. The intent
of building this GUI is to show how to access the parameters and their attributes
directly. To control parameters you should use the PvGenBrowserWnd class.


