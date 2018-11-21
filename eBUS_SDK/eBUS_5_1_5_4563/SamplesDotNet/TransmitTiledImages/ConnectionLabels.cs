// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PvDotNet;


namespace TransmitTiledImages
{
    public class ConnectionLabels
    {
        public ConnectionLabels(Label aIPAddress, Label aMACAddress, Label aSerialNumber, Label aModel)
        {
            mIPAddressLabel = aIPAddress;
            mMACAddressLabel = aMACAddress;
            mSerialNumberLabel = aSerialNumber;
            mModelLabel = aModel;

            Reset();
        }

        Label mIPAddressLabel = null;
        Label mMACAddressLabel = null;
        Label mSerialNumberLabel = null;
        Label mModelLabel = null;

        private string mMACAddress;
        private string mIPAddress;
        private string mSerialNumber;
        private string mModel;
        private string mUserDefinedName;

        public string IPAddress
        {
            set
            {
                mIPAddress = value;
            }
        }

        public string MACAddress
        {
            set
            {
                mMACAddress = value;
            }
        }

        public string SerialNumber
        {
            set
            {
                mSerialNumber = value;
            }
        }

        public string Model
        {
            set
            {
                mModel = value;
            }
        }

        public string UserDefinedName
        {
            set
            {
                mUserDefinedName = value;
            }
        }

        public void Reset()
        {
            mIPAddress = "Not connected";
            mMACAddress = "";
            mModel = "";
            mUserDefinedName = "";

            mIPAddressLabel.Text = "";
            mIPAddressLabel.Enabled = false;

            mMACAddressLabel.Text = "";
            mMACAddressLabel.Enabled = false;

            mSerialNumberLabel.Text = "";
            mSerialNumberLabel.Enabled = false;

            mModelLabel.Text = "";
            mModelLabel.Enabled = false;
        }

        public void Update()
        {
            if ((mIPAddressLabel == null) || (mMACAddressLabel == null)
                || (mSerialNumberLabel == null) || (mModelLabel == null))
            {
                return;
            }
          
            if (!string.IsNullOrEmpty(mIPAddress))
            {
                // An IP address is available; display GEV device information
                mIPAddressLabel.Text = "IP: " + mIPAddress;
                mIPAddressLabel.Enabled = true;

                mMACAddressLabel.Text = "MAC: " + mMACAddress;
                mMACAddressLabel.Enabled = true;
            }
            else
            {
                // Display U3V device information
                mSerialNumberLabel.Text = "Serial number: " + mSerialNumber;
                mSerialNumberLabel.Enabled = true;
            }

            if (mModel.Length != 0)
            {
                mModelLabel.Text = mModel;
                mModelLabel.Enabled = true;
            }
            else if (mUserDefinedName.Length != 0)
            {
                mModelLabel.Text = mUserDefinedName;
                mModelLabel.Enabled = true;
            }
        }
    }
}

