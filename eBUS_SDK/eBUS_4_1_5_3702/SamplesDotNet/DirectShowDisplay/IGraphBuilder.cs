using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a868a9-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IGraphBuilder : IFilterGraph
    {
        new void AddFilter([In] IBaseFilter pFilter, [In] string pName);
        new void RemoveFilter([In] IBaseFilter pFilter);
        new void EnumFilters([Out] out Object ppEnum);
        new void FindFilterByName([In] string pName, [Out] out IBaseFilter ppFilter);
        new void ConnectDirect([In] IPin ppinOut, [In] IPin ppinIn, [In] Object pmt);
        new void Reconnect([In] IPin ppin);
        new void Disconnect([In] IPin ppin);
        new void SetDefaultSyncSource();
        void Connect([In] IPin ppinOut, [In] IPin ppinIn);
        void Render([In] IPin ppinOut);
        void RenderFile([In] string lpcwstrFile, [In] string lpcwstrPlayList);
        void AddSourceFilter( [In] string lpcwstrFileName, [In] string lpcwstrFilterName, [Out] out IBaseFilter ppFilter);
        void SetLogFile(IntPtr hFile);
        void Abort();
        void ShouldOperationContinue();
    }

}

