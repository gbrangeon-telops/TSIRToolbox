// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    /// <summary>
    /// You can use this wrapper in your own application in order to interact with the eBUS SDK PvDSSource 
    /// source filter.
    /// </summary>
    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("7d692bc5-05d4-4032-a581-b7fa1c202825"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IPvDSSource
    {
        void get_Role([Out] out int role);
        void put_Role([In] int role);
        void get_UnicastPort([Out] out int port);
        void put_UnicastPort([In] int port);
        void get_MulticastIP([Out] out string ip);
        void put_MulticastIP([In] string ip);
        void get_MulticastPort([Out] out int port);
        void put_MulticastPort([In] int port);
        void get_DiagnosticEnabled([Out] out bool enabled);
        void put_DiagnosticEnabled([In] bool enabled);
        void get_DeviceID([Out] out string deviceid);
        void put_DeviceID([In] string deviceid);
        void DisconnectDevice();
        void ConnectIfNeeded();
        void get_SourceCount([Out] out int sourcecount);
        void get_SourceName(int aIndex, [Out] out string sourcename);
        void get_Source([Out] out string source);
        void put_Source(string source);
        void get_Channel([Out] out int channel);
        void put_Channel(int channel);
        void get_BufferCount([Out] out int count);
        void put_BufferCount(int count);
        void get_DefaultBufferSize([Out] out int size);
        void put_DefaultBufferSize(int size);
        void get_DropThreshold([Out] out int threshold);
        void put_DropThreshold(int threshold);
        void get_Width([Out] out int width);
        void put_Width(int width);
        void get_Height([Out] out int height);
        void put_Height(int height);
        void get_Interlaced([Out] out bool interlaced);
        void put_Interlaced([In] bool interlaced);
        void get_ParametersSelector([Out] out int selector);
        void put_ParametersSelector(int selector);
        void get_ParametersAvailable([Out] out bool available);
        void get_ParameterCount([Out] out int count);
        void get_ParameterName(int index, [Out] out string deviceid);
        void get_ParameterCategory(string name, [Out] out string category);
        void get_ParameterValue(string name, [Out] out string value);
        void put_ParameterValue(string name, string value, [Out] out string message);
        void get_ParameterType(string name, [Out] out string type);
        void get_ParameterAccess(string name, [Out] out bool available, [Out] out bool readable, [Out] out bool writable);
        void ParameterExecute(string name, [Out] out string message);
        void get_ParameterPossibleValueCount(string name, [Out] out int count);
        void get_ParameterPossibleValue(string name, int index, [Out] out string value);
        void get_ParameterVisibility(string name, [Out] out int visibility);
    }
}


