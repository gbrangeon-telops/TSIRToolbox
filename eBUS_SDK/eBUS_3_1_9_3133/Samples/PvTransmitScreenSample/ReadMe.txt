Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.

========================
Using PvTransmitScreenSample
========================

This sample code illustrates how to use the PvVirtualDevice and PvTransmitterRaw classes to transmit the contents of the screen using the eBUS SDK.

1. Introduction
----------------------------------------

Using the PvSystem and PvInterface class, this sample enumerates available interfaces (network adapters) and selects the first interface with a valid IP address.
Using the PvVirtualDevice class, it listens for device discovery requests so that it can be detected by receiving applications.
Using the PvBuffer class, it shows how to allocate a set of buffers with a given width, height, and pixel format for use by the transmitter API.
Using the PvTransmitterRaw class, a test pattern is transmitted to the specified destination.

By default, this sample transmits the contents of the screen from the first valid interface to multicast address 239.192.1.1:1042. However, it can be used in many different ways by providing it with optional command line arguments. For further details on the command line options that are available, run the sample with the argument --help.

Procedure for multicasting the contents of the screen to a GigE Vision receiver:
 * Ensure that the GigE Vision receiver you wish to transmit to is reachable from the PC that will be running this sample. Their IP addresses should be set such that they are on the same subnet.
 * If only one network interface is installed and connected with a valid IP address on your PC, start the sample with default options (no command line arguments). Otherwise, note the IP address of the interface you wish to transmit from and start the sample with command line argument --sourceaddress=<your IP address>.
 * Press any key when you are ready to begin transmitting (once the receiver is ready to receive the image stream).
 * By default, a vDisplay should be able to receive the stream "out of the box" since it subscribes to the same multicast address and port by default.
 * To receive the test pattern with GEVPlayer:
    * Start GEVPlayer
    * Click Tools->Setup
    * Under "GEVPlayer Role", select "Data receiver"
    * Under "Stream Destination", select "Multicast"
    * Click OK
    * Click Select/Connect.
    * Select the device in the list that matches the IP and MAC of the interface you are transmitting from. By default it will be labeled "eBUS Transmitter".
    * You should begin seeing images as soon as the sample starts transmitting. 


Procedure for unicasting the contents of the screen to a GigE Vision receiver:
 * Ensure that the GigE Vision receiver you wish to transmit to is reachable from the PC that will be running this sample. Their IP addresses should be set such that they are on the same subnet.
 * Start the sample with the following command line arguments:
--sourceaddress=<ip address of the interface you're transmitting from> --destinationport=0 --destinationaddress=<ip address of the interface you're transmitting to>
 * Since a destination port of 0 was provided, the sample will begin listening for device discovery requests and prompt for a destination port.
 * Connect to the sample as a GigE Vision receiver and obtain the port number it is listening on. To receive the test pattern with GEVPlayer:
    * Start GEVPlayer
    * Click Tools->Setup
    * Under "GEVPlayer Role", select "Data receiver"
    * Under "Stream Destination", select "Unicast, automatic"
    * Click OK
    * Click Select/Connect.
    * Select the device in the list that matches the IP and MAC of the interface you are transmitting from. By default it will be labeled "eBUS Transmitter".
    * At this point, you will be able to see what port GEVPlayer is listening on.
       * Click the "Image stream control" button.
       * Under the Connection category, observe the DataPort property.
 * Enter the destination port where prompted by the sample.
 * The sample should begin transmitting and you should see an image appear in the receiver.


2. Prerequisites
----------------------------------------

This sample assumes that:
 * You have a network adapter installed on your PC with a valid IP address.
 * You have a GigE Vision receiver that can receive and display the test pattern (such as a vDisplay, GEVPlayer, NetCommand or any other GigE Vision receiver that supports the GVSP protocol). The receiver should be reachable and on the same subnet as the interface from which it will be receiving.
 * You have the DirectX SDK installed (Available at http://www.microsoft.com/downloads/en/details.aspx?FamilyID=529f03be-1339-48c4-bd5a-8506e5acf571).

3. Description
----------------------------------------

3.1 PvTransmitScreenSample.cpp

Sample code as described above. Please refer to the comments in the source code for information on the methods used in this sample.

4 Visual Studio 10 notes
----------------------------------------

If you want to compile this sample for Visual Studio 10, you will need to make sure that your include directories are configured to give a higher priority to those of the platform SDK than the DirectX SDK.
($WindowsSdkDir)include must come before $(DXSDK_DIR)\Include.

If you do not meed this requirement, you will most likely end up with errors simillar to:

1>objidl.h(11280): error C2061: syntax error : identifier '__RPC__out_xcount_part'
1>objidl.h(11281): error C2059: syntax error : ')'
1>objidl.h(11281): fatal error C1903: unable to recover from previous error(s); stopping compilation

