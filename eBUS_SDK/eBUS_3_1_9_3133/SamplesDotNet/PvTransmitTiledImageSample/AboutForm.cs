using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PvTransmitTiledImageSample
{
    public partial class AboutForm : Form
    {
        public AboutForm()
        {
            InitializeComponent();
        }

        private void AboutForm_Load(object sender, EventArgs e)
        {
            CompanyNameLabel.Text = PvDotNet.PvVersion.COMPANY_NAME;
            VersionLabel.Text = PvDotNet.PvVersion.VERSION;
            CopyrightLabel.Text = PvDotNet.PvVersion.COPYRIGHT;
        }
    }
}
