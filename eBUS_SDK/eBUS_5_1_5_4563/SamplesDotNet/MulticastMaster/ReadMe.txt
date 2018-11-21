Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

===============
MulticastMaster
===============

Project name: MulticastMaster.csproj

1. Purpose
	
	* The purpose of this multicasting master sample is to allow the PC that runs this application to act as a PC Master to control the device from the stream of the group of multicast IP address 239.192.1.1 at port 1024.
	* This sample application runs on the PC Master as a transmitter.

2. Pre-conditions
	
	This samples assumes that:
	* You have a GigE Vision Device and Ethernet network adapter connected to a IGMP Ethernet network switch, using either the eBUS Universal Pro or manufacturer driver.
	* You have the sample MulticastMaster application runs on a PC Master as a transmitter to send block data to a Multicast group controlled by an IGMP Ethernet switch.
	* The PC Master has a Ethernet network adapter connected to a IGMP Ethernet network switch, using either the eBUS Universal Pro or manufacturer driver. 

3. Description

	* Play button: To select and connect to the GigE Vision Device then set the packet sizes, lock device parameters, and start acquisition of the selected device.
	* Stop button: To stop acquisition of the selected device.
	* File/Exit menu item button: To close the application.
	* Parameters/Device menu item button: To shows the Device Control form.
	* Parameters/Communication menu item button: To shows the Communication Control form.


