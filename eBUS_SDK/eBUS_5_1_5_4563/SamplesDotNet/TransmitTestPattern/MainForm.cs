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
using System.Threading;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using PvDotNet;
using PvGUIDotNet;

namespace TransmitTestPattern
{
    public partial class MainForm : Form
    {
        const int WIDTH = 640;
        const int HEIGHT = 480;
        const int BUFFER_COUNT = 8;
        const int FPS = 0;
        const int PACKET_SIZE = 1440;

        private PvTransmitterGEV mTransmitter = new PvTransmitterGEV();
        private PvVirtualDeviceGEV mDevice = new PvVirtualDeviceGEV();
        private Thread mThread = null;
        private bool mStopTransmit = false;
        private bool mFirstTime = true;
        PvPixelType mPvPixelType = PvPixelType.Mono8;

        public MainForm()
        {
            InitializeComponent();
            InitializeParameters();
            timer1.Tick +=new EventHandler(timer1_Tick); // Event timer1_Tick is called every time timer ticks.   
            timer1.Interval = 1 * 1000; // Timer tick every 1 second.
            timer1.Enabled = true;
            timer1.Start();
            transmitStatusLabel.BorderStyle = BorderStyle.FixedSingle;
        }

        private void InitializeParameters()
        {
            PvSystem lSystem = new PvSystem(); 

            // Find NIC in the system.
            lSystem.Find();

            sourceAddressComboBox.Items.Clear();
            foreach (PvInterface lInterface in lSystem)
            {
                PvNetworkAdapter lNetworkAdapter = lInterface as PvNetworkAdapter;
                if (lNetworkAdapter == null)
                {
                    continue;
                }

                // Add found IP addresses of the NIC to sourceAddressComboBox.
                PvIPConfiguration[] lConfigs = lNetworkAdapter.IPConfigurations;
                foreach (PvIPConfiguration lConfig in lConfigs)
                {
                    if (lConfig.IPAddress != "0.0.0.0")
                    {
                        sourceAddressComboBox.Items.Add(lConfig.IPAddress);
                    }
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

            // Set device version
            mDevice.DeviceVersion = "1.0";
            // Set serial number
            mDevice.SerialNumber = "00000001";
            // Set manufacturer name
            mDevice.ManufacturerName = "Pleora Technologies Inc.";
            // Set model name
            mDevice.ModelName = "eBUS SDK Transmitter Sample";
            // Set resend command enabled
            mDevice.SetGVCPCapabilityPacketResendCommandSupported(true);
        }

        private bool GatherParameters()
        {
            bool lResult = true;
            UInt32 lValue = 0;
            try
            {
                lValue = Convert.ToUInt32(destinationPortTextBox.Text);
                if (lValue < 0)
                {
                    MessageBox.Show("Destination port should be a number and greater than 0!", Text);
                    return false;
                }                
            }
            catch ( Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Destination port");
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32(sourcePortTextBox.Text);
                if (lValue < 0)
                {
                    MessageBox.Show("Source port should be a number and greater than 0!", Text);
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Source port");
                return false;
            }
            try
            {
                lValue = Convert.ToUInt32(widthTextBox.Text);
                if (lValue <= 0)
                {
                    MessageBox.Show("Width should be a number and greater than 0!", Text);
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Width");
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32(heightTextBox.Text);
                if (lValue <= 0)
                {
                    MessageBox.Show("Height should be a number and greater than 0!", Text);
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Height");
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32(fpsTextBox.Text);
                if (lValue < 0)
                {
                    MessageBox.Show("Frame per second should be a number and greater than 0!", Text);
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Frame per second");
                return false;
            }

            try
            {
                lValue = Convert.ToUInt32(packetSizeTextBox.Text);
                if (lValue <= 0)
                {
                    MessageBox.Show("Packet size should be a number and greater than 0!", Text);
                    return false;
                }
            }
            catch (Exception lExc)
            {
                MessageBox.Show(lExc.Message, "Packet size");
                return false;
            }

            try
            {
                PvNetworkUtils.IPStrToDWORD(sourceAddressComboBox.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid source IP address!", Text);
                return false;
            }

            try
            {
                PvNetworkUtils.IPStrToDWORD(destinationAddressTextBox.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid destination IP address!", Text);
                return false;
            }

            return lResult;
        }

        private bool TransmittData()
        {
            bool lResult = true;

            try
            {
                Int32 lPoolSize = Convert.ToInt32(poolSizeTextBox.Text);
                UInt32 lWidth = Convert.ToUInt32(widthTextBox.Text);
                UInt32 lHeight = Convert.ToUInt32(heightTextBox.Text);
                UInt32 lPacketSize = Convert.ToUInt32(packetSizeTextBox.Text);
                UInt32 lFPS = Convert.ToUInt32(fpsTextBox.Text);
                UInt32 lSize = lWidth * lHeight * PvImage.GetPixelBitCount(mPvPixelType) / 8;

                List<PvBuffer> lBufferList = new List<PvBuffer>();

                transmitStatusLabel.Text = "";

                // Set the frame rate
                float lMaxPayloadThroughput = 0;
                if (lFPS > 0)
                {
                    lMaxPayloadThroughput = lWidth * lHeight * PvImage.GetPixelBitCount(mPvPixelType) * lFPS;
                }
                mTransmitter.MaxPayloadThroughput = lMaxPayloadThroughput;

                // Initializes list of PvBuffer.
                for (Int32 i = 0; i < lPoolSize; i++)
                {
                    lBufferList.Add(new PvBuffer());
                    lBufferList[i].Image.Alloc(lWidth, lHeight, mPvPixelType);
                }
               
                // Sets packet size
                mTransmitter.PacketSize = lPacketSize;
                
                // Loads free buffers into the pool
                mTransmitter.LoadBufferPool(lBufferList);
                lBufferList.Clear();
                
                // Sets packet size
                mTransmitter.PacketSize = lPacketSize;

                // Initializes a socket to begin transmitting data.
                mTransmitter.Open(destinationAddressTextBox.Text, Convert.ToUInt16(destinationPortTextBox.Text),
                    sourceAddressComboBox.Text, Convert.ToUInt16(sourcePortTextBox.Text), true, false,
                    (UInt32)lPoolSize, true, 64, (UInt32)lPoolSize / 4);

                // Add the transmitter to the device
                if( mFirstTime )
                {
                    mDevice.AddTransmitterGEV(mTransmitter);
                    mFirstTime = false;
                }
                
                // Start responding to GVCP traffic on the specified NIC.
                mDevice.StartListening(sourceAddressComboBox.Text);

                mThread = new Thread(DoThreadWork);

                //Signal to start transmit data.
                mStopTransmit = false;
                mThread.Start();
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show( lPvExc.Message, Text );
                lResult = false;
            }
            return lResult;
        }

        private bool StopTransmitData()
        {
            bool lResult = true;
            if (mTransmitter.IsOpened == false)
            {
                return lResult;
            }
            mStopTransmit = true;
            if (mThread != null)
            {
                // Blocks the calling thread until the thread terminated.
                mThread.Join();
            }
            mThread = null;
            PvBuffer lBuffer = null;
            try
            {
                // Stop responding to GVCP traffic and close the connection.
                mDevice.StopListening();

                // Aborts all pending buffers.
                mTransmitter.AbortQueuedBuffers(); 

                // Retrieve the free buffer and release it.
                while (mTransmitter.RetrieveFreeBuffer(ref lBuffer, 0).IsOK)
                {
                    lBuffer = null;
                }

                // Close the transmitter
                mTransmitter.Close();
            }
            catch (PvException lPvExc)
            {
                MessageBox.Show(lPvExc.Message, Text);
                lResult = false;
            }

            return lResult;
        }

        // Method to retrieve and  queue a buffer into mTransmitter. It is called when mThread is started in method TransmittData().
        unsafe private void DoThreadWork()
        {
            PvBuffer lBuffer = null;
            byte lPixel = 0;
            byte lSeed = 0;
            while (!mStopTransmit)
            {
                try
                {
                    // Retrieve a buffer from a list of available buffers.
                    if (mTransmitter.RetrieveFreeBuffer( ref lBuffer).IsOK )
                    {
                        // Changes the test pattern.
                        byte lFirst = lSeed;
                        byte* lDataPtr = lBuffer.DataPointer;
                        
                        PvImage lImage = lBuffer.Image;
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
                        pvDisplayControl1.Display(lBuffer);

                        // Queue a buffer in to be transmitted.
                        mTransmitter.QueueBuffer(lBuffer);
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
            if (GatherParameters())
            {
                if (TransmittData())
                {
                    playButton.Enabled = false;
                    stopButton.Enabled = true;
                    optionsGroupBox.Enabled = false;
                }
            }
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            if (StopTransmitData())
            {
                playButton.Enabled = true;
                stopButton.Enabled = false;
                optionsGroupBox.Enabled = true;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (mTransmitter.IsOpened)
            {
                StringBuilder lTransmitStatus = new StringBuilder();
                lTransmitStatus.AppendFormat("Transmitted {0, 0:F1} blocks at average rate of {1, 0:F1} FPS. Instantaneous rate of {2, 0:F1} FPS.{3}Average bandwidth is {4, 0:F1} Mb/s. Instantaneous bandwidth is {4, 0:F1} Mb/s.", mTransmitter.BlocksTransmitted, mTransmitter.AverageTransmissionRate, mTransmitter.InstantaneousTransmissionRate, Environment.NewLine, mTransmitter.AveragePayloadThroughput / 1000000.0, mTransmitter.InstantaneousPayloadThroughput / 1000000.0, Environment.NewLine);
                transmitStatusLabel.Text = lTransmitStatus.ToString();
            }
            else
            {
                transmitStatusLabel.Text = "";
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            stopButton.PerformClick();
        }
    }
}   
