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
using System.Text;
using System.Windows.Forms;

namespace PvTransmitTiledImageSample
{
    public partial class BrowserForm : Form
    {
        public BrowserForm()
        {
            InitializeComponent();
        }

        private void BrowserForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!Modal && (e.CloseReason == CloseReason.UserClosing))
            {
                e.Cancel = true;
                Hide();
            }
        }
    }
}