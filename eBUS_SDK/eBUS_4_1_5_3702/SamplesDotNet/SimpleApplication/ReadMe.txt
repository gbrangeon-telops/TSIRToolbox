Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

=================
SimpleApplication
=================

Project name: SimpleApplication.csproj

This C# sample shows a typical simple eBUS SDK application.

1. Description

This simple UI sample represents a simpler version of the eBUS Player application.
It allows the user to connect to a device and control a stream of images coming
from the device.

You can connect to the device using the Select/Connect button. A PvDeviceFinderWnd
dialog is displayed prompting the user to select a GigE Vision device available on
the network.

A PvDevice is then connected to the device and a PvStream is opened. The PvDevice
is configured to stream image data to the PvStream.

Acquisition controls can be used to control streaming. Received images are displayed
using the PvDisplayWnd on the right hand side of the application.

