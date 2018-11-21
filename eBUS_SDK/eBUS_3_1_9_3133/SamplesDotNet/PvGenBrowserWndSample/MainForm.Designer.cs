namespace PvGenBrowserWndSample
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.connectButton = new System.Windows.Forms.Button();
            this.disconnectButton = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.communicationBrowser = new PvGUIDotNet.PvGenBrowserControl();
            this.deviceBrowser = new PvGUIDotNet.PvGenBrowserControl();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(12, 12);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(110, 26);
            this.connectButton.TabIndex = 0;
            this.connectButton.Text = "Select/Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // disconnectButton
            // 
            this.disconnectButton.Location = new System.Drawing.Point(128, 12);
            this.disconnectButton.Name = "disconnectButton";
            this.disconnectButton.Size = new System.Drawing.Size(110, 26);
            this.disconnectButton.TabIndex = 1;
            this.disconnectButton.Text = "Disconnect";
            this.disconnectButton.UseVisualStyleBackColor = true;
            this.disconnectButton.Click += new System.EventHandler(this.disconnectButton_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.communicationBrowser, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.deviceBrowser, 1, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 44);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(643, 525);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // communicationBrowser
            // 
            this.communicationBrowser.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.communicationBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.communicationBrowser.GenParameterArray = null;
            this.communicationBrowser.Location = new System.Drawing.Point(3, 3);
            this.communicationBrowser.Name = "communicationBrowser";
            this.communicationBrowser.Size = new System.Drawing.Size(315, 519);
            this.communicationBrowser.TabIndex = 2;
            this.communicationBrowser.Visibility = PvDotNet.PvGenVisibility.Beginner;
            // 
            // deviceBrowser
            // 
            this.deviceBrowser.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.deviceBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.deviceBrowser.GenParameterArray = null;
            this.deviceBrowser.Location = new System.Drawing.Point(324, 3);
            this.deviceBrowser.Name = "deviceBrowser";
            this.deviceBrowser.Size = new System.Drawing.Size(316, 519);
            this.deviceBrowser.TabIndex = 1;
            this.deviceBrowser.Visibility = PvDotNet.PvGenVisibility.Beginner;
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(667, 581);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.disconnectButton);
            this.Controls.Add(this.connectButton);
            this.MinimumSize = new System.Drawing.Size(638, 518);
            this.Name = "MainForm";
            this.Text = "PvGenBrowserWndSample";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button disconnectButton;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        public PvGUIDotNet.PvGenBrowserControl communicationBrowser;
        public PvGUIDotNet.PvGenBrowserControl deviceBrowser;
    }
}

