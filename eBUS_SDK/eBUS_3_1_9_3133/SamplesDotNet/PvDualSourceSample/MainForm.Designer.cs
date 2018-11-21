namespace PvDualSourceSample
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
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.modeComboBox1 = new System.Windows.Forms.ComboBox();
            this.display1 = new PvGUIDotNet.PvDisplayControl();
            this.playButton1 = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.statusControl1 = new PvGUIDotNet.PvStatusControl();
            this.stopButton1 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.modeComboBox2 = new System.Windows.Forms.ComboBox();
            this.display2 = new PvGUIDotNet.PvDisplayControl();
            this.playButton2 = new System.Windows.Forms.Button();
            this.statusControl2 = new PvGUIDotNet.PvStatusControl();
            this.stopButton2 = new System.Windows.Forms.Button();
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selectConnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.disconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.parametersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.communicationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deviceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.source1StreamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.source2StreamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tableLayoutPanel.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.modeComboBox1);
            this.groupBox4.Controls.Add(this.display1);
            this.groupBox4.Controls.Add(this.playButton1);
            this.groupBox4.Controls.Add(this.statusControl1);
            this.groupBox4.Controls.Add(this.stopButton1);
            this.groupBox4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox4.Location = new System.Drawing.Point(3, 3);
            this.groupBox4.MinimumSize = new System.Drawing.Size(378, 338);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(437, 428);
            this.groupBox4.TabIndex = 27;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Source 1";
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label7.AutoSize = true;
            this.label7.Enabled = false;
            this.label7.Location = new System.Drawing.Point(6, 371);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(88, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Acquisition Mode";
            // 
            // modeComboBox1
            // 
            this.modeComboBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.modeComboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modeComboBox1.Enabled = false;
            this.modeComboBox1.FormattingEnabled = true;
            this.modeComboBox1.Location = new System.Drawing.Point(9, 387);
            this.modeComboBox1.Name = "modeComboBox1";
            this.modeComboBox1.Size = new System.Drawing.Size(147, 21);
            this.modeComboBox1.TabIndex = 11;
            this.modeComboBox1.SelectedIndexChanged += new System.EventHandler(this.modeComboBox1_SelectedIndexChanged);
            // 
            // display1
            // 
            this.display1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.display1.BackgroundColor = System.Drawing.Color.DarkGray;
            this.display1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.display1.Location = new System.Drawing.Point(6, 19);
            this.display1.Name = "display1";
            this.display1.Size = new System.Drawing.Size(425, 292);
            this.display1.TabIndex = 10;
            // 
            // playButton1
            // 
            this.playButton1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.playButton1.Enabled = false;
            this.playButton1.ImageIndex = 0;
            this.playButton1.ImageList = this.imageList;
            this.playButton1.Location = new System.Drawing.Point(227, 367);
            this.playButton1.Name = "playButton1";
            this.playButton1.Size = new System.Drawing.Size(102, 53);
            this.playButton1.TabIndex = 7;
            this.playButton1.Text = "Play";
            this.playButton1.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.playButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.playButton1.UseVisualStyleBackColor = true;
            this.playButton1.Click += new System.EventHandler(this.playButton1_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.White;
            this.imageList.Images.SetKeyName(0, "play.bmp");
            this.imageList.Images.SetKeyName(1, "stop.bmp");
            // 
            // statusControl1
            // 
            this.statusControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusControl1.BuffersReallocated = false;
            this.statusControl1.DisplayThread = null;
            this.statusControl1.Location = new System.Drawing.Point(6, 317);
            this.statusControl1.MinimumSize = new System.Drawing.Size(400, 44);
            this.statusControl1.Name = "statusControl1";
            this.statusControl1.Size = new System.Drawing.Size(425, 44);
            this.statusControl1.Stream = null;
            this.statusControl1.TabIndex = 11;
            this.statusControl1.Warning = "";
            // 
            // stopButton1
            // 
            this.stopButton1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.stopButton1.Enabled = false;
            this.stopButton1.ImageIndex = 1;
            this.stopButton1.ImageList = this.imageList;
            this.stopButton1.Location = new System.Drawing.Point(335, 367);
            this.stopButton1.Name = "stopButton1";
            this.stopButton1.Size = new System.Drawing.Size(96, 53);
            this.stopButton1.TabIndex = 8;
            this.stopButton1.Text = "Stop";
            this.stopButton1.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton1.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton1.UseVisualStyleBackColor = true;
            this.stopButton1.Click += new System.EventHandler(this.stopButton1_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.modeComboBox2);
            this.groupBox3.Controls.Add(this.display2);
            this.groupBox3.Controls.Add(this.playButton2);
            this.groupBox3.Controls.Add(this.statusControl2);
            this.groupBox3.Controls.Add(this.stopButton2);
            this.groupBox3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox3.Location = new System.Drawing.Point(446, 3);
            this.groupBox3.MinimumSize = new System.Drawing.Size(378, 338);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(438, 428);
            this.groupBox3.TabIndex = 28;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Source 2";
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label6.AutoSize = true;
            this.label6.Enabled = false;
            this.label6.Location = new System.Drawing.Point(6, 371);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(88, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Acquisition Mode";
            // 
            // modeComboBox2
            // 
            this.modeComboBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.modeComboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modeComboBox2.Enabled = false;
            this.modeComboBox2.FormattingEnabled = true;
            this.modeComboBox2.Location = new System.Drawing.Point(6, 387);
            this.modeComboBox2.Name = "modeComboBox2";
            this.modeComboBox2.Size = new System.Drawing.Size(147, 21);
            this.modeComboBox2.TabIndex = 11;
            this.modeComboBox2.SelectedIndexChanged += new System.EventHandler(this.modeComboBox2_SelectedIndexChanged);
            // 
            // display2
            // 
            this.display2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.display2.BackgroundColor = System.Drawing.Color.DarkGray;
            this.display2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.display2.Location = new System.Drawing.Point(6, 19);
            this.display2.Name = "display2";
            this.display2.Size = new System.Drawing.Size(426, 292);
            this.display2.TabIndex = 10;
            // 
            // playButton2
            // 
            this.playButton2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.playButton2.Enabled = false;
            this.playButton2.ImageIndex = 0;
            this.playButton2.ImageList = this.imageList;
            this.playButton2.Location = new System.Drawing.Point(228, 367);
            this.playButton2.Name = "playButton2";
            this.playButton2.Size = new System.Drawing.Size(102, 53);
            this.playButton2.TabIndex = 7;
            this.playButton2.Text = "Play";
            this.playButton2.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.playButton2.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.playButton2.UseVisualStyleBackColor = true;
            this.playButton2.Click += new System.EventHandler(this.playButton2_Click);
            // 
            // statusControl2
            // 
            this.statusControl2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusControl2.BuffersReallocated = false;
            this.statusControl2.DisplayThread = null;
            this.statusControl2.Location = new System.Drawing.Point(6, 317);
            this.statusControl2.MinimumSize = new System.Drawing.Size(400, 32);
            this.statusControl2.Name = "statusControl2";
            this.statusControl2.Size = new System.Drawing.Size(426, 44);
            this.statusControl2.Stream = null;
            this.statusControl2.TabIndex = 11;
            this.statusControl2.Warning = "";
            // 
            // stopButton2
            // 
            this.stopButton2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.stopButton2.Enabled = false;
            this.stopButton2.ImageIndex = 1;
            this.stopButton2.ImageList = this.imageList;
            this.stopButton2.Location = new System.Drawing.Point(336, 367);
            this.stopButton2.Name = "stopButton2";
            this.stopButton2.Size = new System.Drawing.Size(96, 53);
            this.stopButton2.TabIndex = 8;
            this.stopButton2.Text = "Stop";
            this.stopButton2.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton2.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton2.UseVisualStyleBackColor = true;
            this.stopButton2.Click += new System.EventHandler(this.stopButton2_Click);
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.ColumnCount = 2;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.Controls.Add(this.groupBox4, 0, 0);
            this.tableLayoutPanel.Controls.Add(this.groupBox3, 1, 0);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 24);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.RowCount = 1;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel.Size = new System.Drawing.Size(887, 434);
            this.tableLayoutPanel.TabIndex = 29;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.connectionToolStripMenuItem,
            this.parametersToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(887, 24);
            this.menuStrip1.TabIndex = 30;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.exitToolStripMenuItem.Text = "E&xit...";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // connectionToolStripMenuItem
            // 
            this.connectionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.selectConnectToolStripMenuItem,
            this.disconnectToolStripMenuItem});
            this.connectionToolStripMenuItem.Name = "connectionToolStripMenuItem";
            this.connectionToolStripMenuItem.Size = new System.Drawing.Size(81, 20);
            this.connectionToolStripMenuItem.Text = "Connection";
            // 
            // selectConnectToolStripMenuItem
            // 
            this.selectConnectToolStripMenuItem.Name = "selectConnectToolStripMenuItem";
            this.selectConnectToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.selectConnectToolStripMenuItem.Text = "Select/Connect";
            this.selectConnectToolStripMenuItem.Click += new System.EventHandler(this.selectConnectToolStripMenuItem_Click);
            // 
            // disconnectToolStripMenuItem
            // 
            this.disconnectToolStripMenuItem.Name = "disconnectToolStripMenuItem";
            this.disconnectToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.disconnectToolStripMenuItem.Text = "Disconnect";
            this.disconnectToolStripMenuItem.Click += new System.EventHandler(this.disconnectToolStripMenuItem_Click);
            // 
            // parametersToolStripMenuItem
            // 
            this.parametersToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.communicationToolStripMenuItem,
            this.deviceToolStripMenuItem,
            this.source1StreamToolStripMenuItem,
            this.source2StreamToolStripMenuItem});
            this.parametersToolStripMenuItem.Name = "parametersToolStripMenuItem";
            this.parametersToolStripMenuItem.Size = new System.Drawing.Size(78, 20);
            this.parametersToolStripMenuItem.Text = "Parameters";
            // 
            // communicationToolStripMenuItem
            // 
            this.communicationToolStripMenuItem.Name = "communicationToolStripMenuItem";
            this.communicationToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.communicationToolStripMenuItem.Text = "Communication";
            this.communicationToolStripMenuItem.Click += new System.EventHandler(this.communicationToolStripMenuItem_Click);
            // 
            // deviceToolStripMenuItem
            // 
            this.deviceToolStripMenuItem.Name = "deviceToolStripMenuItem";
            this.deviceToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.deviceToolStripMenuItem.Text = "Device";
            this.deviceToolStripMenuItem.Click += new System.EventHandler(this.deviceToolStripMenuItem_Click);
            // 
            // source1StreamToolStripMenuItem
            // 
            this.source1StreamToolStripMenuItem.Name = "source1StreamToolStripMenuItem";
            this.source1StreamToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.source1StreamToolStripMenuItem.Text = "Source 1 Stream";
            this.source1StreamToolStripMenuItem.Click += new System.EventHandler(this.source1StreamToolStripMenuItem_Click);
            // 
            // source2StreamToolStripMenuItem
            // 
            this.source2StreamToolStripMenuItem.Name = "source2StreamToolStripMenuItem";
            this.source2StreamToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.source2StreamToolStripMenuItem.Text = "Source 2 Stream";
            this.source2StreamToolStripMenuItem.Click += new System.EventHandler(this.source2StreamToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(887, 458);
            this.Controls.Add(this.tableLayoutPanel);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(784, 406);
            this.Name = "MainForm";
            this.Text = "PvDualSourceSample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tableLayoutPanel.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox modeComboBox1;
        private PvGUIDotNet.PvDisplayControl display1;
        private System.Windows.Forms.Button playButton1;
        private PvGUIDotNet.PvStatusControl statusControl1;
        private System.Windows.Forms.Button stopButton1;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox modeComboBox2;
        private PvGUIDotNet.PvDisplayControl display2;
        private System.Windows.Forms.Button playButton2;
        private PvGUIDotNet.PvStatusControl statusControl2;
        private System.Windows.Forms.Button stopButton2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem selectConnectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem disconnectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem parametersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem communicationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deviceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem source1StreamToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem source2StreamToolStripMenuItem;
    }
}

