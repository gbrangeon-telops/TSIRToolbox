using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a86892-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IEnumPins
    {
        void Next([In] int cPins, [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] IPin[] ppPins, [In] IntPtr pcFetched);
        void Skip([In] int cPins);
        void Reset();
        void Clone([Out] out IEnumPins ppEnum);
    }

}
