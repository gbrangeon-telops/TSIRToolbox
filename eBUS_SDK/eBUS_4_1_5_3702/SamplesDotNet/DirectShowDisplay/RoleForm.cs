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
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PvGUIDotNet;

namespace DirectShowDisplay
{
    public partial class RoleForm : Form
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public RoleForm()
        {
            InitializeComponent();
        }

        public int Role { get; private set; }
        public UInt16 UnicastPort { get; private set; }
        public string MulticastAddress { get; private set; }
        public UInt16 MulticastPort { get; private set; }

        /// <summary>
        /// Form closing
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RoleForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult != DialogResult.OK)
            {
                return;
            }

            Role = 0;
            if (controllerRadioButton.Checked)
            {
                Role = 0;
            }
            else if (unicastReceiverRadioButton.Checked)
            {
                Role = 1;
            }
            else if (multicastReceiverRadioButton.Checked)
            {
                Role = 2;
            }

            UnicastPort = (UInt16)localPortSpinEdit.Value;
            MulticastAddress = multicastIPAddressEdit.AddressText;
            MulticastPort = (UInt16)multicastPortSpinEdit.Value;
        }
    }
}
