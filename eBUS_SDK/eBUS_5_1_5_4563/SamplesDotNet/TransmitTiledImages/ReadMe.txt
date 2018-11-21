Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

===================
TransmitTiledImages
===================

Project name: TransmitTiledImages.csproj

1. Purpose
	
	* The purpose of this application is to transmit the tiled image from 1-4 source(s) of images. 
	* The source image is processed, tiled according to the display resolution, tiling mode, number of connections, then the data is transmitted to the default multicast IP address 239.192.1.1 at port 1024 and displayed with pixel type BGR8Packed on the display control and status label.

2. Pre-conditions
	
	This samples assumes that:
	 * You have at least one Ethernet network adapter installed on the PC with a valid IP address, using either the eBUS Universal Pro or manufacturer driver.
     * You have at least 1 GigE Vision or USB3 Vision device to use as a transmitter.
	 * You have the sample TransmitTiledImages application runs on a PC to send block data to a default multicast group IP address 239.192.1.1 at port 1024 by the first found NIC IP Address.
	 * You have a GigE Vision receiver that can receive and display the tiled image (such as a vDisplay, eBUS Player, NetCommand, or any GigE Vision receiver that supports the GVSP protocol). 
	   The receiver should be reachable and on the same subnet as the interface from which it will be receiving.
	 * You are aware of the amount of bandwidth that is available for transmitting and receiving video data. For best results, use a different network adapter for transmitting than the one that is receiving and ensure that your desired throughput is less than your theoretical bandwidth (typically less than the max interface speed upstream or downstream).
	 * Each NIC you use should be on a different subnet.
	 * You must have .NET framework version 4.0 installed.

3. Description
	
	* When the sample application is started, it shows a form with a "File" menu, 4 video sources group, the acquisition controls group, the video output group, the transmission group, the display control and the status label.
	* The file menu has the following menu items:
		- Open: To open the pvt configuration file.
		- Save: To save the opened pvt configuration file.
		- Save As: To save the pvt configuration file as the specified file name.
		- Exit: To close application.
	* 4 video sources group:
		- Each video source group has an IP address and MAC address (or serial number for USB3 Vision devices), a model, "Select" button to select a device, "Connect/Disconnect" button to interact with the device, setup communication information, and a "..." button to set the communication, device and stream parameters.
	* Acquisition controls group to start and stop the data acquisition.
	* Video output group has the display resolution and the tiling mode to select between "Maintain Aspect Ratio", "Maintain Resolution" and "Stretch to Fit". 
	* Transmission group has the destination address (default is multicast address 239.192.1.1), destination port (default port is 1042), interface address (default is the first found NIC IP address), desired frame rate (default is 30), maximum packet size (default is 1476).
	* Display control to display tiled image.
	* Status label to display transmitter status.