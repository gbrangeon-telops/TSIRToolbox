using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct FilterInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)] public string achName;
        [MarshalAs(UnmanagedType.Interface)] public IFilterGraph pGraph;
    }

    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a86895-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IBaseFilter
    {
        void GetClassID([Out] out Guid pClassID);
        void Stop();
        void Pause();
        void Run(long tStart);
        void GetState([In] int dwMilliSecsTimeout, [Out] out int filtState);
        void SetSyncSource([In] Object pClock);
        void GetSyncSource([Out] out Object pClock);
        void EnumPins([Out] out IEnumPins ppEnum);
        void FindPin([In] string Id, [Out] out IPin ppPin);
        void QueryFilterInfo([Out] out FilterInfo pInfo);
        void JoinFilterGraph([In] IFilterGraph pGraph, [In] string pName);
        void QueryVendorInfo([Out] out string pInfo);
    }
}

