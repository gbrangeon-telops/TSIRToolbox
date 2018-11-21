Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

===================
TransmitTestPattern
===================

Project name: TransmitTestPattern.csproj

1. Purpose
	
	* The purpose of this application is to transmit test pattern Mono8 pixel type to the multicast IP address 239.192.1.1 at port 1024 then displays the data on the control and status display.
	* This sample application runs on the PC as a transmitter.

2. Pre-conditions
	
	This samples assumes that:
	* Ethernet network adapter installed on the PC, using either the eBUS Universal Pro or manufacturer driver.
	* You have the sample TransmitTestPattern application runs on a PC as a transmitter to send block data to a Multicast group IP address 239.192.1.1 at port 1024.

3. Description
	
	* When sample application is started, it shows the Options group of parameters, after the parameters have been set and the Play button is clicked the blocks of data will be transmitted and displayed on the display control and transmit status label.
	* File/Exit menu item button: To close the application.
	* Options group to change the parameters setting.
	* Play button to transmit data.
	* Stop button to stop transmit data.