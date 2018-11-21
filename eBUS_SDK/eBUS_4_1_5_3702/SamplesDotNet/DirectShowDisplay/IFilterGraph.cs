using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a8689f-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IFilterGraph
    {
        void AddFilter([In] IBaseFilter pFilter, string pName);
        void RemoveFilter([In] IBaseFilter pFilter);
        void EnumFilters([Out] out Object ppEnum);
        void FindFilterByName([In] string pName, [Out] out IBaseFilter ppFilter);
        void ConnectDirect([In] IPin ppinOut, [In] IPin ppinIn, Object pmt);
        void Reconnect([In] IPin ppin);
        void Disconnect([In] IPin ppin);
        void SetDefaultSyncSource();
    }

}

