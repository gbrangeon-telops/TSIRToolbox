Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

==============
MulticastSlave
==============

Project name: MulticastSlave.csproj

1. Purpose
	
	* The purpose of this application is to receive data from the stream of the group of multicast IP address 239.192.1.1 at port 1024 then displays the data on the control and status display.
	* This sample application runs on the PC slave as a receiver.

2. Pre-conditions
	
	This samples assumes that:
	* You have a GigE Vision Device and Ethernet network adapter connected to a IGMP Ethernet network switch, using either the eBUS Universal Pro or manufacturer driver.
	* You have the sample PvMultiCastMasterSample application runs on a PC Master as a transmitter to send block data to a Multicast group controlled by an IGMP Ethernet switch.
	* The PC Master has a Ethernet network adapter connected to a IGMP Ethernet network switch, using either the eBUS Universal Pro or manufacturer driver. 

3. Description
	
	* When sample application is started, it shows the GigE Device Selection, after the device is selected and connected to the group of multicast IP address 239.192.1.1 at port 1024, the
	  block of data will be retrieved and displayed on the control and status display.
	* File/Exit menu item button: To close the application.
	* Parameters/Stream menu item button: To open the Stream control.