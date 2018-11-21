Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.

========================
Using PvRecoverySample
========================

This sample code illustrates how to create a simple command line
application performing typical image acquisition and fully handles
the interaction required with the PvDevice in order to support
automatic recovery.

1. Introduction

Mostly the PvDevice and PvStream classes, this sample shows how to:
 * Select a device using PvDeviceFinderWnd
 * Connect to a device
 * Open a stream
 * Start acquisition on a device
 * Receive images using a PvStream
 * Receive recovery events/callback from a PvDevice
 * Handle recovery events

2. Prerequisites

This samples assumes that:
 * You have a GigE Vision device is connected to the network

3. Description

3.1 PvRecoverySample.cpp

Shows how to create a simple command line application for image
acquisition with recovery support by using the PureGEV SDK.


