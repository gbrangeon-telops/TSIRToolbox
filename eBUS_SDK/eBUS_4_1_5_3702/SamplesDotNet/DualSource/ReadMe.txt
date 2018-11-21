Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.

==========
DualSource
==========

Project name: DualSource.csproj

This C# sample shows how to control and stream from a dual source GigE Vision device.

1. Description

This sample works just like SimpleApplication sample, except that it allows receiving data
from two sources.

To make room for the additional display the button controls have been moved in the
application menu.

Attention should be paid to selecting the right source (using the source selector
of the GenICam interface of the device) before issuing acquisition control commands.

If your only has a single source, the left display is used and the acquisition controls
for the second source are disabled.
