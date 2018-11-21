// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using PvDotNet;
using PvGUIDotNet;
using System.Runtime.InteropServices;

namespace DirectShowDisplay
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
        }

        DirectShow.IGraphBuilder mGraph = null;
        DirectShow.IBaseFilter mSourceFilter = null;
        DirectShow.IBaseFilter mVideoFilter = null;
        
        RoleForm mRoleForm = null;

        /// <summary>
        /// Form load event handler. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            mRoleForm = new RoleForm();
            DialogResult lResult = mRoleForm.ShowDialog();
            if (lResult != DialogResult.OK)
            {
                Close();
                return;
            }

            PvDeviceFinderForm lForm = new PvDeviceFinderForm();
            lForm.PleoraProtocolVisible = false;
            lForm.U3VVisible = (mRoleForm.Role == 0);
            lForm.ShowDialog();
            if (lForm.Selected == null)
            {
                Close();
                return;
            }

            CreateSourceFilter(lForm.Selected.ConnectionID);
            CreateVideoFilter();
            CreateGraph();
            ConnectFilters();
            BindDisplay();

            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;
            lControl.Run();
        }

        /// <summary>
        /// Creates the eBUS SDK PvDSSource filter
        /// </summary>
        /// <param name="aDeviceID"></param>
        private void CreateSourceFilter(string aDeviceID)
        {
            // Create the filter
            mSourceFilter = new DirectShow.PvDSSource() as DirectShow.IBaseFilter;
            
            // Get IPvDSSource interface from the filter
            DirectShow.IPvDSSource lDSSource = mSourceFilter as DirectShow.IPvDSSource;

            // Configure filter
            lDSSource.put_DropThreshold(4); // The filter will drop frames if more than 4 frames are available to limit latency
            lDSSource.put_Role(mRoleForm.Role);
            lDSSource.put_UnicastPort(mRoleForm.UnicastPort);
            lDSSource.put_MulticastIP(mRoleForm.MulticastAddress);
            lDSSource.put_MulticastPort(mRoleForm.MulticastPort);
            lDSSource.put_DeviceID(aDeviceID);
            lDSSource.ConnectIfNeeded();

            // Get sources, fill sources menu
            int lSourceCount = 0;
            lDSSource.get_SourceCount(out lSourceCount);
            sourcesToolStripMenuItem.Visible = lSourceCount > 0;
            sourcesToolStripMenuItem.DropDownItems.Clear();
            for (int i = 0; i < lSourceCount; i++)
            {
                string lSourceName = "";
                lDSSource.get_SourceName(i, out lSourceName);

                ToolStripItem lItem = sourcesToolStripMenuItem.DropDownItems.Add(lSourceName);
                lItem.Click += new EventHandler(sourceMenuItem_Click);
            }
        }

        /// <summary>
        /// Creates the video filter (display)
        /// </summary>
        private void CreateVideoFilter()
        {
            // Create display
            mVideoFilter = new DirectShow.VideoMixingRenderer9() as DirectShow.IBaseFilter;
        }

        /// <summary>
        /// Creates the DirectShow graph
        /// </summary>
        private void CreateGraph()
        {
            // Create graph
            mGraph = new DirectShow.FilterGraph() as DirectShow.IGraphBuilder;

            // Add both filters
            mGraph.AddFilter(mSourceFilter, "Source");
            mGraph.AddFilter(mVideoFilter, "Display");
        }

        /// <summary>
        /// Connects the filters
        /// </summary>
        private void ConnectFilters()
        {
            // Get pins
            DirectShow.IPin lPin1 = GetFilterPin(mSourceFilter);
            DirectShow.IPin lPin2 = GetFilterPin(mVideoFilter);

            // In case the filters are already connected, disconnect first
            mGraph.Disconnect(lPin1);
            mGraph.Disconnect(lPin2);

            // Connect pins
            mGraph.Connect(lPin1, lPin2);
        }

        /// <summary>
        /// Returns the 1st pin of a filter
        /// </summary>
        /// <param name="aFilter"></param>
        /// <returns></returns>
        private DirectShow.IPin GetFilterPin(DirectShow.IBaseFilter aFilter)
        {
            DirectShow.IEnumPins lEnumPins = null;
            aFilter.EnumPins(out lEnumPins);

            DirectShow.IPin[] lPins = new DirectShow.IPin[1];
            IntPtr lFetched = (IntPtr)0;
            lEnumPins.Next(1, lPins, lFetched);

            return lPins[0];
        }

        /// <summary>
        /// Propagate window position to the display
        /// </summary>
        private void UpdateDisplayPosition()
        {
            DirectShow.IVideoWindow lVideoWindow = mVideoFilter as DirectShow.IVideoWindow;
            if (lVideoWindow == null)
            {
                return;
            }

            lVideoWindow.SetWindowPosition(0, 0, panel.Width, panel.Height);
        }

        /// <summary>
        /// Binds the display to our window
        /// </summary>
        private void BindDisplay()
        {
            // Get IVideoWindow interface
            DirectShow.IVideoWindow lVideoWindow = mGraph as DirectShow.IVideoWindow;

            // Configure display
            lVideoWindow.put_Owner(panel.Handle);
            lVideoWindow.put_MessageDrain(panel.Handle);
            lVideoWindow.put_WindowStyle(0x44000000); // Child | ClipSiblings

            AdjustSize();
            UpdateDisplayPosition();
        }

        /// <summary>
        /// Adjusts the window size to match the display
        /// </summary>
        private void AdjustSize()
        {
            // Set form size to match source filter
            DirectShow.IPvDSSource lSource = mSourceFilter as DirectShow.IPvDSSource;
            int lWidth, lHeight;
            lSource.get_Width(out lWidth);
            lSource.get_Height(out lHeight);
            ClientSize = new System.Drawing.Size(lWidth, lHeight + menuStrip.Height);
        }

        /// <summary>
        /// Form activated: update display bounds
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Activated(object sender, EventArgs e)
        {
            UpdateDisplayPosition();
        }

        /// <summary>
        /// Form resized: update display bounds
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Resize(object sender, EventArgs e)
        {
            UpdateDisplayPosition();
        }

        /// <summary>
        /// Form moved: update display bounds
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Move(object sender, EventArgs e)
        {
            UpdateDisplayPosition();
        }

        /// <summary>
        /// Exit menu handler. Quit the application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// Controls drop down opening. Set items state.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void controlsToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            int lState = 0;
            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;

            lControl.GetState(100, out lState);

            runToolStripMenuItem.Enabled = (lState == 0) || (lState == 1);
            pauseToolStripMenuItem.Enabled = (lState == 2);
            stopToolStripMenuItem.Enabled = (lState == 1) || (lState == 2);
        }

        /// <summary>
        /// Run menu item handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void runToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;
            lControl.Run();
        }

        /// <summary>
        /// Pause menu item handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;
            lControl.Pause();
        }

        /// <summary>
        /// Stop menu item handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void stopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;
            lControl.Stop();
        }

        /// <summary>
        /// Display menu opening. Enable items.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void displayToolStripMenuItem1_DropDownOpening(object sender, EventArgs e)
        {
            DirectShow.IPvDSSource lDSSource = mSourceFilter as DirectShow.IPvDSSource;

            bool lDiagnostics = false;
            lDSSource.get_DiagnosticEnabled(out lDiagnostics);

            diagnosticsMenuItem.Checked = lDiagnostics;
        }

        /// <summary>
        /// Flip diagnostics mode
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void diagnosticsMenuItem_Click(object sender, EventArgs e)
        {
            DirectShow.IPvDSSource lDSSource = mSourceFilter as DirectShow.IPvDSSource;

            bool lDiagnostics = false;
            lDSSource.get_DiagnosticEnabled(out lDiagnostics);
            lDSSource.put_DiagnosticEnabled(!lDiagnostics);
        }

        /// <summary>
        /// Full screen display menu handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void fullScreenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DirectShow.IVideoWindow lVideoWindow = mGraph as DirectShow.IVideoWindow;
            lVideoWindow.put_FullScreenMode(DirectShow.OABool.True);
        }

        /// <summary>
        /// Source change handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void sourceMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripItem lItem = sender as ToolStripItem;
            if (lItem == null)
            {
                return;
            }

            DirectShow.IPvDSSource lDSSource = mSourceFilter as DirectShow.IPvDSSource;
            DirectShow.IMediaControl lControl = mGraph as DirectShow.IMediaControl;

            // New source
            string lNewSource = lItem.Text;

            // Current source
            string lCurrentSource = "";
            lDSSource.get_Source(out lCurrentSource);

            // Don't change for the same
            if (lNewSource == lCurrentSource)
            {
                return;
            }

            // Stop graph, disconnect filters
            lControl.Stop();

            // Set new source, reconnect source filter
            lDSSource.put_Source(lNewSource);
            lDSSource.ConnectIfNeeded();

            // Reconnect filters
            ConnectFilters();

            // Make sure our window fits the newly selected source
            AdjustSize();

            // Resume
            lControl.Run();
        }

        private void sourceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DisplayPropertyPages(mSourceFilter);
        }

        private void displayToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DisplayPropertyPages(mVideoFilter);
        }

        [DllImport("oleaut32.dll", PreserveSig = false)]
        static extern void OleCreatePropertyFrame(IntPtr hwndOwner, int x, int y,
            [MarshalAs(UnmanagedType.LPWStr)] string lpszCaption,
            int cObjects, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4, ArraySubType = UnmanagedType.IUnknown)] object[] lplpUnk,
            int cPages, IntPtr lpPageClsID,
            int lcid, int dwReserved, int lpvReserved);

        /// <summary>
        /// Displays filter property pages
        /// </summary>
        /// <param name="aForm"></param>
        /// <param name="aFilter"></param>
        public void DisplayPropertyPages(DirectShow.IBaseFilter aFilter)
        {
            DirectShow.ISpecifyPropertyPages lPropertyPages = aFilter as DirectShow.ISpecifyPropertyPages;
            DirectShow.DsCAUUID lPages;
            DirectShow.FilterInfo lInfo;
            aFilter.QueryFilterInfo(out lInfo);
            lPropertyPages.GetPages(out lPages);
            DirectShow.IBaseFilter []lFilters = new DirectShow.IBaseFilter[1];
            lFilters[0] = aFilter;
            OleCreatePropertyFrame(Handle, 0, 0, lInfo.achName, 1, lFilters, lPages.cElems, lPages.pElems, 0, 0, 0);
            Marshal.FreeCoTaskMem(lPages.pElems);
        }
    }
}

