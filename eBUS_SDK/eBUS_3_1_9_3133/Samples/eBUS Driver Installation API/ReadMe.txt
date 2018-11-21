Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.

=========================
Using the eBUS Installation API.
=========================

This sample shows how to display information about network configuration and apply eBUS drivers to network adapters on a given PC.

1. Introduction

Using the eBUS installation API, this sample shows how to:
 * Display information about network adapters and drivers installed on the PC.
 * Install and uninstall drivers on a given adapter.
 * Know when to reboot when necessary.

Important notes:
 * You must use the 64 bit version of the tool to install 64 bit drivers on a 64 bit system.
 * When installing drivers on Windows Vista and Windows 7, elevated UAC privileges are required. Note the additional manifest files that have been associated with this project to ensure that UAC privileges are elevated when running this sample (i.e. the user is always prompted to run the application as an administrator).

2. Pre-conditions

This samples assumes that:
 * You intend to use the eBUS installation API to distribute Pleora drivers with your software.
 * You have installed the drivers on your development system using the Driver Installation Tool that comes with the SDK and you understand the recommended driver configuration for the system being deployed.

3. Description

3.1 main.cpp

Shows how to use the EbNetworkAdapter, EbDriver and EbInstaller classes.

4. Notes

In Microsoft Visual Studio 2010, the manifest file cannot be used to change the user level anymore. 

The following error will happen if you try to compile the imported project: 
	manifest authoring error c1010001: Values of attribute "level" not equal in different manifest snippets.	

To work around this situation, change the property of the project:
	Configuration Properties\Linker\Manifest File\UAC Execution Level to the value "requireAdministrator (level='requireAdministrator')"