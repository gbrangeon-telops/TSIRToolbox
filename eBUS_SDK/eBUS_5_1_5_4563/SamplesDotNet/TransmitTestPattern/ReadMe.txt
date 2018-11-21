Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

===================
TransmitTestPattern
===================

This sample code illustrates how to use the PvVirtualDeviceGEV and PvTransmitterGEV classes to transmit a test pattern using the eBUS SDK.

1. Introduction

The user selects the interface to transmit on using a combo box populated by all network interfaces of a PvSystem.
Using the PvVirtualDeviceGEV class, it listens for device discovery requests so that it can be detected by receiving applications.
Using the PvBuffer class, it shows how to allocate a set of buffers with a given width, height, and pixel format for use by the transmitter API.
Using the PvTransmitterGEV class, a test pattern is transmitted to the specified destination.

By default, this sample transmits a test pattern from the first valid interface to multicast address 239.192.1.1:1042. However, it can be used in many different which can be configured from the user interface.

By default, this sample transmits as fast as the computer permits. This can cause the CPU to seem high. The fps can be configured on the user interface to limit bandwidth.

Procedure for multicasting a test pattern to a GigE Vision receiver:
 * Ensure that the GigE Vision receiver you wish to transmit to is reachable from the PC that will be running this sample. Their IP addresses should be set such that they are on the same subnet.
 * Start the application
 * Select the network interface to transmit on.
 * Configure the multicast group (IP and port) or use the defaults.
 * Press the Play button to start transmitting.
 * By default, a vDisplay should be able to receive the stream "out of the box" since it subscribes to the same multicast address and port by default.
 * To receive the test pattern with eBUSPlayer:
    * Start eBUSPlayer
    * Click Tools->Setup
    * Under "eBUSPlayer Role", select "Data receiver"
    * Under "Stream Destination", select "Multicast"
	* Provide your multicast group (IP and port) or accept the defaults
    * Click OK
    * Click Select/Connect.
    * Select the device in the list that matches the IP and MAC of the interface you are transmitting from. By default it will be labelled "eBUS Transmitter".
    * You should begin seeing images as soon as the sample starts transmitting. 


Procedure for unicasting a test pattern to a GigE Vision receiver:
 * Ensure that the GigE Vision receiver you wish to transmit to is reachable from the PC that will be running this sample. Their IP addresses should be set such that they are on the same subnet.
 * Start the application.
 * Connect to the sample as a GigE Vision receiver and obtain the port number it is listening on. To receive the test pattern with eBUSPlayer:
    * Start eBUSPlayer
    * Click Tools->Setup
    * Under "eBUSPlayer Role", select "Data receiver"
    * Under "Stream Destination", select "Unicast, automatic"
    * Click OK
    * Click Select/Connect.
    * Select the device in the list that matches the IP and MAC of the interface you are transmitting from. By default it will be labelled "eBUS Transmitter".
    * At this point, you will be able to see what port eBUSPlayer is listening on.
       * Click the "Image stream control" button.
       * Under the Connection category, observe the DataPort property.
 * Configure the transmitter destination
    * Enter the IP address of the network interface used by eBUSPlayer in the Destination Address text box
    * Enter the DataPort used by eBUSPlayer in the Destination Port text box
 * Press the Play buffer to start transmitting
 * The sample should begin transmitting and you should see an image appear in the receiver.


2. Prerequisites

This sample assumes that:
 * You have a network adapter installed on your PC with a valid IP address.
 * You have a GigE Vision receiver that can receive and display the test pattern (such as a vDisplay, eBUSPlayer, NetCommand or any other GigE Vision receiver that supports the GVSP protocol). The receiver should be reachable and on the same subnet as the interface from which it will be receiving.

