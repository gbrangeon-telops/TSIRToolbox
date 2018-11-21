using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace DirectShowDisplay.DirectShow
{
    public enum OABool { False = 0, True = -1 }

    [ComImport, System.Security.SuppressUnmanagedCodeSecurity, Guid("56a868b4-0ad4-11ce-b03a-0020af0ba770"), InterfaceType(ComInterfaceType.InterfaceIsDual)]
    public interface IVideoWindow
    {
        void put_Caption([In, MarshalAs(UnmanagedType.BStr)] string caption);
        void get_Caption([Out, MarshalAs(UnmanagedType.BStr)] out string caption);
        void put_WindowStyle([In] int windowStyle);
        void get_WindowStyle([Out] out int windowStyle);
        void put_WindowStyleEx([In] int windowStyleEx);
        void get_WindowStyleEx([Out] out int windowStyleEx);
        void put_AutoShow([In] OABool autoShow);
        void get_AutoShow([Out] out OABool autoShow);
        void put_WindowState([In] int windowState);
        void get_WindowState([Out] out int windowState);
        void put_BackgroundPalette([In] OABool backgroundPalette);
        void get_BackgroundPalette([Out] out OABool backgroundPalette);
        void put_Visible([In] OABool visible); 
        void get_Visible([Out] out OABool visible);
        void put_Left([In] int left);
        void get_Left([Out] out int left);
        void put_Width([In] int width);
        void get_Width([Out] out int width);
        void put_Top([In] int top);
        void get_Top([Out] out int top);
        void put_Height([In] int height);
        void get_Height([Out] out int height);
        void put_Owner([In] IntPtr owner);
        void get_Owner([Out] out IntPtr owner);
        void put_MessageDrain([In] IntPtr drain);
        void get_MessageDrain([Out] out IntPtr drain);
        void get_BorderColor([Out] out int color);
        void put_BorderColor([In] int color);
        void get_FullScreenMode([Out] out OABool fullScreenMode);
        void put_FullScreenMode([In] OABool fullScreenMode);
        void SetWindowForeground([In] OABool focus);
        void NotifyOwnerMessage([In] IntPtr hwnd, [In] int msg, [In] IntPtr wParam, [In] IntPtr lParam);
        void SetWindowPosition([In] int left, [In] int top, [In] int width, [In] int height );
        void GetWindowPosition([Out] out int left, [Out] out int top, [Out] out int width, [Out] out int height);
        void GetMinIdealImageSize([Out] out int width, [Out] out int height);
        void GetMaxIdealImageSize([Out] out int width, [Out] out int height);
        void GetRestorePosition([Out] out int left, [Out] out int top, [Out] out int width, [Out] out int height);
        void HideCursor([In] OABool hideCursor);
        void IsCursorHidden([Out] out OABool hideCursor);
    }

}

