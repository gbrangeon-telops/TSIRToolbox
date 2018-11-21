using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DsCAUUID
    {
        public int cElems;
        public IntPtr pElems;
    }

    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("B196B28B-BAB4-101A-B69C-00AA00341D07"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ISpecifyPropertyPages
    {
        int GetPages(out DsCAUUID pPages);
    }

}
