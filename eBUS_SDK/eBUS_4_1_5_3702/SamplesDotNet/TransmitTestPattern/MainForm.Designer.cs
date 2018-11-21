namespace TransmitTestPattern
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.widthLabel = new System.Windows.Forms.Label();
            this.widthTextBox = new System.Windows.Forms.TextBox();
            this.heightTextBox = new System.Windows.Forms.TextBox();
            this.heightLabel = new System.Windows.Forms.Label();
            this.fpsTextBox = new System.Windows.Forms.TextBox();
            this.fpsLabel = new System.Windows.Forms.Label();
            this.poolSizeTextBox = new System.Windows.Forms.TextBox();
            this.poolSizeLabel = new System.Windows.Forms.Label();
            this.sourceAddressLabel = new System.Windows.Forms.Label();
            this.destinationAddressTextBox = new System.Windows.Forms.TextBox();
            this.destinationAddressLabel = new System.Windows.Forms.Label();
            this.destinationPortTextBox = new System.Windows.Forms.TextBox();
            this.destinationPortLabel = new System.Windows.Forms.Label();
            this.sourcePortTextBox = new System.Windows.Forms.TextBox();
            this.sourcePortLabel = new System.Windows.Forms.Label();
            this.packetSizeTextBox = new System.Windows.Forms.TextBox();
            this.packetSizeLabel = new System.Windows.Forms.Label();
            this.pixelTypeLabel = new System.Windows.Forms.Label();
            this.stopButton = new System.Windows.Forms.Button();
            this.playButton = new System.Windows.Forms.Button();
            this.resetOptionsButton = new System.Windows.Forms.Button();
            this.optionsGroupBox = new System.Windows.Forms.GroupBox();
            this.pixelTypeTexBox = new System.Windows.Forms.TextBox();
            this.sourceAddressComboBox = new System.Windows.Forms.ComboBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.transmitStatusLabel = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.pvDisplayControl1 = new PvGUIDotNet.PvDisplayControl();
            this.menuStrip1.SuspendLayout();
            this.optionsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(787, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(92, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // widthLabel
            // 
            this.widthLabel.AutoSize = true;
            this.widthLabel.Location = new System.Drawing.Point(6, 16);
            this.widthLabel.Name = "widthLabel";
            this.widthLabel.Size = new System.Drawing.Size(35, 13);
            this.widthLabel.TabIndex = 1;
            this.widthLabel.Text = "Width";
            this.toolTip1.SetToolTip(this.widthLabel, "Width in pixels - default 640.");
            // 
            // widthTextBox
            // 
            this.widthTextBox.Location = new System.Drawing.Point(112, 16);
            this.widthTextBox.Name = "widthTextBox";
            this.widthTextBox.Size = new System.Drawing.Size(124, 20);
            this.widthTextBox.TabIndex = 2;
            this.toolTip1.SetToolTip(this.widthTextBox, "Width in pixels - default 640.");
            // 
            // heightTextBox
            // 
            this.heightTextBox.Location = new System.Drawing.Point(112, 42);
            this.heightTextBox.Name = "heightTextBox";
            this.heightTextBox.Size = new System.Drawing.Size(124, 20);
            this.heightTextBox.TabIndex = 4;
            this.toolTip1.SetToolTip(this.heightTextBox, "Height in pixels - default 480.");
            // 
            // heightLabel
            // 
            this.heightLabel.AutoSize = true;
            this.heightLabel.Location = new System.Drawing.Point(6, 42);
            this.heightLabel.Name = "heightLabel";
            this.heightLabel.Size = new System.Drawing.Size(38, 13);
            this.heightLabel.TabIndex = 3;
            this.heightLabel.Text = "Height";
            this.toolTip1.SetToolTip(this.heightLabel, "Height in pixels - default 480.");
            // 
            // fpsTextBox
            // 
            this.fpsTextBox.Location = new System.Drawing.Point(112, 68);
            this.fpsTextBox.Name = "fpsTextBox";
            this.fpsTextBox.Size = new System.Drawing.Size(124, 20);
            this.fpsTextBox.TabIndex = 6;
            this.toolTip1.SetToolTip(this.fpsTextBox, "Desired frame rate - default UINT_MAX - no limitation on frame rate.");
            // 
            // fpsLabel
            // 
            this.fpsLabel.AutoSize = true;
            this.fpsLabel.Location = new System.Drawing.Point(6, 68);
            this.fpsLabel.Name = "fpsLabel";
            this.fpsLabel.Size = new System.Drawing.Size(27, 13);
            this.fpsLabel.TabIndex = 5;
            this.fpsLabel.Text = "FPS";
            this.toolTip1.SetToolTip(this.fpsLabel, "Desired frame rate - default UINT_MAX - no limitation on frame rate.");
            // 
            // poolSizeTextBox
            // 
            this.poolSizeTextBox.Location = new System.Drawing.Point(112, 120);
            this.poolSizeTextBox.Name = "poolSizeTextBox";
            this.poolSizeTextBox.Size = new System.Drawing.Size(124, 20);
            this.poolSizeTextBox.TabIndex = 8;
            this.toolTip1.SetToolTip(this.poolSizeTextBox, "Number of buffers to pre-allocate - default 4. Increase this number when sending " +
        "smaller images at high frame rates.");
            // 
            // poolSizeLabel
            // 
            this.poolSizeLabel.AutoSize = true;
            this.poolSizeLabel.Location = new System.Drawing.Point(6, 120);
            this.poolSizeLabel.Name = "poolSizeLabel";
            this.poolSizeLabel.Size = new System.Drawing.Size(82, 13);
            this.poolSizeLabel.TabIndex = 7;
            this.poolSizeLabel.Text = "Buffer Pool Size";
            this.toolTip1.SetToolTip(this.poolSizeLabel, "Number of buffers to pre-allocate - default 4. Increase this number when sending " +
        "smaller images at high frame rates.");
            // 
            // sourceAddressLabel
            // 
            this.sourceAddressLabel.AutoSize = true;
            this.sourceAddressLabel.Location = new System.Drawing.Point(6, 146);
            this.sourceAddressLabel.Name = "sourceAddressLabel";
            this.sourceAddressLabel.Size = new System.Drawing.Size(82, 13);
            this.sourceAddressLabel.TabIndex = 9;
            this.sourceAddressLabel.Text = "Source Address";
            this.toolTip1.SetToolTip(this.sourceAddressLabel, "Source address in the form 123.215.168.101 - default - first valid address encoun" +
        "tered while enumerating interfaces.");
            // 
            // destinationAddressTextBox
            // 
            this.destinationAddressTextBox.Location = new System.Drawing.Point(112, 172);
            this.destinationAddressTextBox.Name = "destinationAddressTextBox";
            this.destinationAddressTextBox.Size = new System.Drawing.Size(124, 20);
            this.destinationAddressTextBox.TabIndex = 12;
            this.toolTip1.SetToolTip(this.destinationAddressTextBox, "Destination address in the form 123.215.168.101 - default 239.192.1.1");
            // 
            // destinationAddressLabel
            // 
            this.destinationAddressLabel.AutoSize = true;
            this.destinationAddressLabel.Location = new System.Drawing.Point(6, 172);
            this.destinationAddressLabel.Name = "destinationAddressLabel";
            this.destinationAddressLabel.Size = new System.Drawing.Size(101, 13);
            this.destinationAddressLabel.TabIndex = 11;
            this.destinationAddressLabel.Text = "Destination Address";
            this.toolTip1.SetToolTip(this.destinationAddressLabel, "Destination address in the form 123.215.168.101 - default 239.192.1.1");
            // 
            // destinationPortTextBox
            // 
            this.destinationPortTextBox.Location = new System.Drawing.Point(112, 224);
            this.destinationPortTextBox.Name = "destinationPortTextBox";
            this.destinationPortTextBox.Size = new System.Drawing.Size(124, 20);
            this.destinationPortTextBox.TabIndex = 16;
            this.toolTip1.SetToolTip(this.destinationPortTextBox, "destination port - default 1042.");
            // 
            // destinationPortLabel
            // 
            this.destinationPortLabel.AutoSize = true;
            this.destinationPortLabel.Location = new System.Drawing.Point(6, 224);
            this.destinationPortLabel.Name = "destinationPortLabel";
            this.destinationPortLabel.Size = new System.Drawing.Size(82, 13);
            this.destinationPortLabel.TabIndex = 15;
            this.destinationPortLabel.Text = "Destination Port";
            this.toolTip1.SetToolTip(this.destinationPortLabel, "destination port - default 1042.");
            // 
            // sourcePortTextBox
            // 
            this.sourcePortTextBox.Location = new System.Drawing.Point(112, 198);
            this.sourcePortTextBox.Name = "sourcePortTextBox";
            this.sourcePortTextBox.Size = new System.Drawing.Size(124, 20);
            this.sourcePortTextBox.TabIndex = 14;
            this.toolTip1.SetToolTip(this.sourcePortTextBox, "source port - default 0. A port is automatically assigned when the socket is open" +
        "ed.");
            // 
            // sourcePortLabel
            // 
            this.sourcePortLabel.AutoSize = true;
            this.sourcePortLabel.Location = new System.Drawing.Point(6, 198);
            this.sourcePortLabel.Name = "sourcePortLabel";
            this.sourcePortLabel.Size = new System.Drawing.Size(63, 13);
            this.sourcePortLabel.TabIndex = 13;
            this.sourcePortLabel.Text = "Source Port";
            this.toolTip1.SetToolTip(this.sourcePortLabel, "source port - default 0. A port is automatically assigned when the socket is open" +
        "ed.");
            // 
            // packetSizeTextBox
            // 
            this.packetSizeTextBox.Location = new System.Drawing.Point(112, 94);
            this.packetSizeTextBox.Name = "packetSizeTextBox";
            this.packetSizeTextBox.Size = new System.Drawing.Size(124, 20);
            this.packetSizeTextBox.TabIndex = 22;
            this.toolTip1.SetToolTip(this.packetSizeTextBox, "Maximimum size of streaming packets - default 1440. For best results, set \"Jumbo " +
        "Frames\" property on your NIC and increase this value accordingly.");
            // 
            // packetSizeLabel
            // 
            this.packetSizeLabel.AutoSize = true;
            this.packetSizeLabel.Location = new System.Drawing.Point(6, 94);
            this.packetSizeLabel.Name = "packetSizeLabel";
            this.packetSizeLabel.Size = new System.Drawing.Size(64, 13);
            this.packetSizeLabel.TabIndex = 21;
            this.packetSizeLabel.Text = "Packet Size";
            this.toolTip1.SetToolTip(this.packetSizeLabel, "Maximimum size of streaming packets - default 1440. For best results, set \"Jumbo " +
        "Frames\" property on your NIC and increase this value accordingly.");
            // 
            // pixelTypeLabel
            // 
            this.pixelTypeLabel.AutoSize = true;
            this.pixelTypeLabel.Location = new System.Drawing.Point(6, 253);
            this.pixelTypeLabel.Name = "pixelTypeLabel";
            this.pixelTypeLabel.Size = new System.Drawing.Size(56, 13);
            this.pixelTypeLabel.TabIndex = 23;
            this.pixelTypeLabel.Text = "Pixel Type";
            this.toolTip1.SetToolTip(this.pixelTypeLabel, "GEV pixel type - default Mono8.");
            // 
            // stopButton
            // 
            this.stopButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.stopButton.Enabled = false;
            this.stopButton.Image = ((System.Drawing.Image)(resources.GetObject("stopButton.Image")));
            this.stopButton.Location = new System.Drawing.Point(688, 375);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(82, 41);
            this.stopButton.TabIndex = 31;
            this.stopButton.Text = "Stop";
            this.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // playButton
            // 
            this.playButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.playButton.Image = ((System.Drawing.Image)(resources.GetObject("playButton.Image")));
            this.playButton.Location = new System.Drawing.Point(600, 375);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(82, 41);
            this.playButton.TabIndex = 30;
            this.playButton.Text = "Play";
            this.playButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.playButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.playButton_Click);
            // 
            // resetOptionsButton
            // 
            this.resetOptionsButton.Location = new System.Drawing.Point(143, 295);
            this.resetOptionsButton.Name = "resetOptionsButton";
            this.resetOptionsButton.Size = new System.Drawing.Size(93, 23);
            this.resetOptionsButton.TabIndex = 32;
            this.resetOptionsButton.Text = "Reset Options";
            this.resetOptionsButton.UseVisualStyleBackColor = true;
            this.resetOptionsButton.Click += new System.EventHandler(this.resetOptionsButton_Click);
            // 
            // optionsGroupBox
            // 
            this.optionsGroupBox.Controls.Add(this.pixelTypeTexBox);
            this.optionsGroupBox.Controls.Add(this.sourceAddressComboBox);
            this.optionsGroupBox.Controls.Add(this.poolSizeTextBox);
            this.optionsGroupBox.Controls.Add(this.resetOptionsButton);
            this.optionsGroupBox.Controls.Add(this.widthLabel);
            this.optionsGroupBox.Controls.Add(this.widthTextBox);
            this.optionsGroupBox.Controls.Add(this.heightLabel);
            this.optionsGroupBox.Controls.Add(this.heightTextBox);
            this.optionsGroupBox.Controls.Add(this.fpsLabel);
            this.optionsGroupBox.Controls.Add(this.fpsTextBox);
            this.optionsGroupBox.Controls.Add(this.pixelTypeLabel);
            this.optionsGroupBox.Controls.Add(this.poolSizeLabel);
            this.optionsGroupBox.Controls.Add(this.packetSizeTextBox);
            this.optionsGroupBox.Controls.Add(this.sourceAddressLabel);
            this.optionsGroupBox.Controls.Add(this.packetSizeLabel);
            this.optionsGroupBox.Controls.Add(this.destinationAddressLabel);
            this.optionsGroupBox.Controls.Add(this.destinationAddressTextBox);
            this.optionsGroupBox.Controls.Add(this.sourcePortLabel);
            this.optionsGroupBox.Controls.Add(this.sourcePortTextBox);
            this.optionsGroupBox.Controls.Add(this.destinationPortTextBox);
            this.optionsGroupBox.Controls.Add(this.destinationPortLabel);
            this.optionsGroupBox.Location = new System.Drawing.Point(12, 27);
            this.optionsGroupBox.Name = "optionsGroupBox";
            this.optionsGroupBox.Size = new System.Drawing.Size(251, 335);
            this.optionsGroupBox.TabIndex = 33;
            this.optionsGroupBox.TabStop = false;
            this.optionsGroupBox.Text = "Options";
            // 
            // pixelTypeTexBox
            // 
            this.pixelTypeTexBox.Location = new System.Drawing.Point(112, 250);
            this.pixelTypeTexBox.Name = "pixelTypeTexBox";
            this.pixelTypeTexBox.Size = new System.Drawing.Size(124, 20);
            this.pixelTypeTexBox.TabIndex = 35;
            this.pixelTypeTexBox.Text = "Mono8";
            this.toolTip1.SetToolTip(this.pixelTypeTexBox, "GEV pixel type - default Mono8.");
            // 
            // sourceAddressComboBox
            // 
            this.sourceAddressComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.sourceAddressComboBox.FormattingEnabled = true;
            this.sourceAddressComboBox.Items.AddRange(new object[] {
            "true",
            "false"});
            this.sourceAddressComboBox.Location = new System.Drawing.Point(112, 145);
            this.sourceAddressComboBox.Name = "sourceAddressComboBox";
            this.sourceAddressComboBox.Size = new System.Drawing.Size(124, 21);
            this.sourceAddressComboBox.TabIndex = 34;
            this.toolTip1.SetToolTip(this.sourceAddressComboBox, "Source address in the form 123.215.168.101 - default - first valid address encoun" +
        "tered while enumerating interfaces.");
            // 
            // transmitStatusLabel
            // 
            this.transmitStatusLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.transmitStatusLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.transmitStatusLabel.Location = new System.Drawing.Point(269, 322);
            this.transmitStatusLabel.MinimumSize = new System.Drawing.Size(502, 40);
            this.transmitStatusLabel.Name = "transmitStatusLabel";
            this.transmitStatusLabel.Size = new System.Drawing.Size(502, 40);
            this.transmitStatusLabel.TabIndex = 34;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // pvDisplayControl1
            // 
            this.pvDisplayControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pvDisplayControl1.BackColor = System.Drawing.Color.Transparent;
            this.pvDisplayControl1.BackgroundColor = System.Drawing.Color.DarkGray;
            this.pvDisplayControl1.Location = new System.Drawing.Point(269, 27);
            this.pvDisplayControl1.Name = "pvDisplayControl1";
            this.pvDisplayControl1.Size = new System.Drawing.Size(502, 292);
            this.pvDisplayControl1.TabIndex = 25;
            this.pvDisplayControl1.TextOverlay = "";
            this.pvDisplayControl1.TextOverlayColor = System.Drawing.Color.Lime;
            this.pvDisplayControl1.TextOverlayOffsetX = 10;
            this.pvDisplayControl1.TextOverlayOffsetY = 10;
            this.pvDisplayControl1.TextOverlaySize = 18;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(787, 432);
            this.Controls.Add(this.transmitStatusLabel);
            this.Controls.Add(this.optionsGroupBox);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.playButton);
            this.Controls.Add(this.pvDisplayControl1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "TransmitTestPattern";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.optionsGroupBox.ResumeLayout(false);
            this.optionsGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Label widthLabel;
        private System.Windows.Forms.TextBox widthTextBox;
        private System.Windows.Forms.TextBox heightTextBox;
        private System.Windows.Forms.Label heightLabel;
        private System.Windows.Forms.TextBox fpsTextBox;
        private System.Windows.Forms.Label fpsLabel;
        private System.Windows.Forms.TextBox poolSizeTextBox;
        private System.Windows.Forms.Label poolSizeLabel;
        private System.Windows.Forms.Label sourceAddressLabel;
        private System.Windows.Forms.TextBox destinationAddressTextBox;
        private System.Windows.Forms.Label destinationAddressLabel;
        private System.Windows.Forms.TextBox destinationPortTextBox;
        private System.Windows.Forms.Label destinationPortLabel;
        private System.Windows.Forms.TextBox sourcePortTextBox;
        private System.Windows.Forms.Label sourcePortLabel;
        private System.Windows.Forms.TextBox packetSizeTextBox;
        private System.Windows.Forms.Label packetSizeLabel;
        private System.Windows.Forms.Label pixelTypeLabel;
        private PvGUIDotNet.PvDisplayControl pvDisplayControl1;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.Button resetOptionsButton;
        private System.Windows.Forms.GroupBox optionsGroupBox;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ComboBox sourceAddressComboBox;
        private System.Windows.Forms.Label transmitStatusLabel;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox pixelTypeTexBox;
    }
}

