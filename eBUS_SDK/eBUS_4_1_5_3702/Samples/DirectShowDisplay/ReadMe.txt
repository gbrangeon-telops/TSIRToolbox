Copyright (c) 2014, Pleora Technologies Inc., All rights reserved.

================
DirectShowDsplay
================

Shows how to use the eBUS SDK DirectShow source filter to receive images
from a GigE Vision or USB3 Vision device which is also being controlled.

The images are simply sent to the Microsoft Video Mixing Renderer 9.

It is possible to control the eBUS SDK DirectShow source filter using 
its IPvDSSource COM interface.

This sample focuses mainly on using the eBUS SDK DirectShow Source filter and
assumes good knowledge of DirectShow and COM.


1. Introduction

Using the eBUS SDK DirectShow Source filter, this sample shows how to:
 * Instantiate the filter
 * Configure the filter
 * Add the filter to a graph
 * Connect the filter to a DirectShow display filter
 * Start streaming
 * Stop streaming


2. Pre-conditions

This sample assumes that:
 * you have a GigE Vision Device connected to a network adapter or a USB3 Vision device connected to a USB 3.0 interface.


3. Description

3.1 DirectShowDisplay.cpp.

Shows how to use the eBUS SDK DirectShow filter for image acquisition and display. Please refer to the comments in the source 
code for more information regarding the methods used in this sample.