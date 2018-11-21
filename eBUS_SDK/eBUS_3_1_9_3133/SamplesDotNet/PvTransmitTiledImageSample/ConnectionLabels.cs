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


namespace PvTransmitTiledImageSample
{
    public class ConnectionLabels
    {
        public ConnectionLabels(Label aIPAddress, Label aMACAddress, Label aModel)
        {
            mIPAddressLabel = aIPAddress;
            mMACAddressLabel = aMACAddress;
            mModelLabel = aModel;

            Reset();
        }

        Label mIPAddressLabel = null;
        Label mMACAddressLabel = null;
        Label mModelLabel = null;

        private string mMACAddress;
        private string mIPAddress;
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

            mModelLabel.Text = "";
            mModelLabel.Enabled = false;
        }

        public void Update()
        {
            if ((mIPAddressLabel == null) || (mMACAddressLabel == null) || (mModelLabel == null))
            {
                return;
            }
          
            mIPAddressLabel.Text = "IP: " + mIPAddress;
            mIPAddressLabel.Enabled = true;

            mMACAddressLabel.Text = "MAC: " +  mMACAddress;
            mMACAddressLabel.Enabled = true;

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

