// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Threading;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using PvDotNet;
using PvGUIDotNet;

namespace PvTransmitTestPatternSample
{
    public partial class MainForm : Form
    {
        const int WIDTH = 640;
        const int HEIGHT = 480;
        const int BUFFER_COUNT = 4;
        const int FPS = 0;
        const int PACKET_SIZE = 1440;

        private PvTransmitterRaw mTransmitterRaw = new PvTransmitterRaw();
        private PvVirtualDevice mDevice = new PvVirtualDevice();
        private Thread mThread = null;
        private bool mStopTransmit = false;

        public MainForm()
        {
            InitializeComponent();
            InitializeParameters();
            timer1.Tick +=new EventHandler(timer1_Tick); // Event timer1_Tick is called every time timer ticks.   
            timer1.Interval = 1 * 10000; // Timer tick every 1 second
            timer1.Enabled = true;
            timer1.Start();
            transmitStatusLabel.BorderStyle = BorderStyle.FixedSingle;
        }

        private void InitializeParameters()
        {
            PvSystem lPvSystem = new PvSystem(); 

            // Find NIC in the system
            lPvSystem.Find();

            sourceAddressComboBox.Items.Clear();
            for (uint i = 0; i < lPvSystem.InterfaceCount; i++)
            {
                if ( lPvSystem.GetInterface( i ).IPAddress.CompareTo( "0.0.0.0" ) != 0 )
                {
                    // Add found IP address of the NIC to sourceAddressComboBox
                    sourceAddressComboBox.Items.Add( lPvSystem.GetInterface(i).IPAddress );
                }
            }

            sourceAddressComboBox.SelectedIndex = 0;
            destinationPortTextBox.Text = "1042";
            destinationAddressTextBox.Text = "239.192.1.1";
            sourcePortTextBox.Text = "0";
            widthTextBox.Text = WIDTH.ToString();
            heightTextBox.Text = HEIGHT.ToString();
            fpsTextBox.Text = FPS.ToString();
            packetSizeTextBox.Text = PACKET_SIZE.ToString();
            poolSizeTextBox.Text = BUFFER_COUNT.ToString();
            pixelTypeTexBox.ReadOnly = true;
        }

        private bool GatherParameters()
        {
            bool lResult = true;
            UInt32 lValue = 0;
            try
            {
                lValue = Convert.ToUInt32( destinationPortTextBox.Text );
                if (lValue < 0)
                {
                    MessageBox.Show( "Source port should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
                
            }
            catch ( Exception lExc)
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32( sourcePortTextBox.Text );
                if (lValue < 0)
                {
                    MessageBox.Show( "Source port should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch ( Exception lExc )
            {
                MessageBox.Show( lExc.Message );
                return false;
            }
            try
            {
                lValue = Convert.ToUInt32(widthTextBox.Text);
                if ( lValue < 0 )
                {
                    MessageBox.Show( "Width should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch ( Exception lExc )
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32( heightTextBox.Text );
                if ( lValue < 0 )
                {
                    MessageBox.Show( "Height should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch ( Exception lExc )
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32( fpsTextBox.Text );
                if (lValue < 0)
                {
                    MessageBox.Show( "Frame per second should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32( packetSizeTextBox.Text );
                if ( lValue < 0 )
                {
                    MessageBox.Show( "Packet size should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch ( Exception lExc )
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32( packetSizeTextBox.Text );
                if (lValue < 0)
                {
                    MessageBox.Show("Packet size should be a number and greater than 0!", "PvTransmitTestPatternSample" );
                    return false;
                }
            }
            catch ( Exception lExc )
            {
                MessageBox.Show( lExc.Message );
                return false;
            }

            try
            {
                PvNetworkUtils.IPStrToDWORD(sourceAddressComboBox.Text);
            }
            catch ( Exception )
            {
                MessageBox.Show( "Invalid source IP address!", "PvTransmitTestPatternSample" );
                return false;
            }

            try
            {
                PvNetworkUtils.IPStrToDWORD( destinationAddressTextBox.Text );
            }
            catch ( Exception )
            {
                MessageBox.Show( "Invalid destination IP address!", "PvTransmitTestPatternSample" );
                return false;
            }

            return lResult;
        }

        private bool TransmittData()
        {
            bool lResult = true;
            PvPixelType lPvPixelType = PvPixelType.Mono8; 
            Int32 lPoolSize = Convert.ToInt32( poolSizeTextBox.Text );
            UInt32 lWidth = Convert.ToUInt32( widthTextBox.Text );
            UInt32 lHeight = Convert.ToUInt32( heightTextBox.Text );
            UInt32 lPacketSize = Convert.ToUInt32(packetSizeTextBox.Text);
            UInt32 lFPS = Convert.ToUInt32(fpsTextBox.Text);
            UInt32 lSize = lWidth * lHeight * PvImage.GetPixelBitCount(lPvPixelType) / 8;

            List<PvBuffer> lPvBufferList = new List<PvBuffer>();

            if (lFPS > 0)
            {
                float lPayLoadSize = lWidth * lHeight * PvImage.GetPixelBitCount(lPvPixelType) * lFPS;
                mTransmitterRaw.MaxPayloadThroughput = lPayLoadSize;
            }

            // Initializes list of PvBuffer
            for ( Int32 i = 0; i < lPoolSize; i++ )
            {
                lPvBufferList.Add( new PvBuffer() );
                lPvBufferList[i].Image.Alloc(lWidth, lHeight, lPvPixelType);
            }

            try
            {
                // Start responding to GVCP traffic on the specified NIC.
                mDevice.StartListening(sourceAddressComboBox.Text);
                // Sets packet size
                mTransmitterRaw.PacketSize = lPacketSize;
                // Loads free buffers into the pool
                mTransmitterRaw.LoadBufferPool( lPvBufferList );
                lPvBufferList.Clear();
                // Initializes a socket to begin transmitting data.
                mTransmitterRaw.Open( destinationAddressTextBox.Text, Convert.ToUInt16( destinationPortTextBox.Text ), sourceAddressComboBox.Text, Convert.ToUInt16( sourcePortTextBox.Text ) );

                mThread = new Thread(DoThreadWork);

                //Signal to start transmit data.
                mStopTransmit = false;
                mThread.Start();
                
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show( lPvExc.Message );
                lResult = false;
            }
            return lResult;
        }

        private bool StopTransmitData()
        {
            bool lResult = true;
            if (mTransmitterRaw.IsOpened == false)
            {
                return lResult;
            }
            mStopTransmit = true;
            if (mThread != null)
            {
                // Blocks the calling thread untils the thread terminated
                mThread.Join();
            }
            mThread = null;
            PvBuffer lPvBuffer = null;
            try
            {
                // Aborts all pending buffers.
                mTransmitterRaw.AbortQueuedBuffers(); 

                // Retrieve the free buffer and release it.
                while (mTransmitterRaw.RetrieveFreeBuffer(ref lPvBuffer, 0).IsOK)
                {
                    lPvBuffer = null;
                }
                // Close the transmitter
                mTransmitterRaw.Close();
                // Stop responding to GVCP traffic and close the connection..
                mDevice.StopListening();
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message);
                lResult = false;
            }

            return lResult;
        }
        // Method to retrieve and  queue a buffer into mTransmitterRaw. It is called when mThread is started in method TransmittData().
        unsafe private void DoThreadWork()
        {
            PvBuffer lPvBuffer = null;
            byte lPixel = 0;
            byte lSeed = 0;
            while ( !mStopTransmit )
            {
                try
                {
                    // Retrieve a buffer from a list of available buffers
                    if (mTransmitterRaw.RetrieveFreeBuffer( ref lPvBuffer).IsOK )
                    {
                        // Changes the test pattern.
                        byte lFirst = lSeed;
                        byte* lDataPtr = lPvBuffer.DataPointer;
                        
                        PvImage lImage = lPvBuffer.Image;
                        for (UInt32 y = 0; y < lImage.Height; y++)
                        {
                            for (UInt32 x = 0; x < lImage.Width; x++)
                            {
                                *lDataPtr = lPixel++;
                                lDataPtr++;
                            }

                            lPixel = ++lFirst;
                        }
                        lSeed++;
                        // Process the image in the PvBuffer.  
                        // ....
                        // Displays the image.
                        pvDisplayControl1.Display(lPvBuffer);

                        // Queue a buffer in to be transmitted.
                        mTransmitterRaw.QueueBuffer(lPvBuffer);
                    }
                }
                catch (PvException lPvExc)
                {
                    MessageBox.Show(lPvExc.Message);
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult lDR = MessageBox.Show( "Are you sure you want to exit the application?",
                Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question );
            if ( lDR == DialogResult.Yes )
            {
                StopTransmitData();
                Close();
            }
        }

        private void resetOptionsButton_Click(object sender, EventArgs e)
        {
            // Initializes parameters for transmitting data.
            InitializeParameters();            
        }

        private void playButton_Click(object sender, EventArgs e)
        {
            if ( GatherParameters() == true )
            {
                if (TransmittData() == true)
                {
                    playButton.Enabled = false;
                    stopButton.Enabled = true;
                    optionsGroupBox.Enabled = false;
                }
            }
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            if (StopTransmitData() == true)
            {
                playButton.Enabled = true;
                stopButton.Enabled = false;
                optionsGroupBox.Enabled = true;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            StringBuilder lTransmitStatus = new StringBuilder();
            lTransmitStatus.AppendFormat("Transmitted {0, 0:F1} blocks at average rate of {1, 0:F1} FPS. Instantaneous rate of {2, 0:F1} FPS.{3}Average bandwidth is {4, 0:F1} Mb/s. Instantaneous bandwidth is {4, 0:F1} Mb/s.", mTransmitterRaw.BlocksTransmitted, mTransmitterRaw.AverageTransmissionRate, mTransmitterRaw.InstantaneousTransmissionRate, Environment.NewLine, mTransmitterRaw.AveragePayloadThroughput / 1000000.0, mTransmitterRaw.InstantaneousPayloadThroughput / 1000000.0, Environment.NewLine);
            if (mTransmitterRaw.IsOpened == true)
            {
                transmitStatusLabel.Text = lTransmitStatus.ToString();
            }
        }
    }
}   