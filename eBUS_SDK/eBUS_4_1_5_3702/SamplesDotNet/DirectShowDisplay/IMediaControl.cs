using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;

namespace DirectShowDisplay.DirectShow
{
    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a868b1-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsDual)]
    public interface IMediaControl
    {
        void Run();
        void Pause();
        void Stop();
        void GetState([In] int msTimeout, [Out] out int pfs);
        void RenderFile([In] string strFilename);
        void AddSourceFilter([In, MarshalAs(UnmanagedType.BStr)] string strFilename, [Out, MarshalAs(UnmanagedType.IDispatch)] out object ppUnk);
        void get_FilterCollection([Out, MarshalAs(UnmanagedType.IDispatch)] out object ppUnk);
        void get_RegFilterCollection([Out, MarshalAs(UnmanagedType.IDispatch)] out object ppUnk);
        void StopWhenReady();
    }

}
