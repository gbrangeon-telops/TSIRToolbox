namespace MulticastMaster
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.stopButton = new System.Windows.Forms.Button();
            this.playButton = new System.Windows.Forms.Button();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.parametersSettingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deviceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.communicationCOntrolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nameTextBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.modelTextBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.manufacturerTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.macAddressTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.ipAddressTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.packetSizeTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.multicastPortTextBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.multicastIPTextBox = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.menuStrip.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // stopButton
            // 
            this.stopButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.stopButton.Enabled = false;
            this.stopButton.Image = ((System.Drawing.Image)(resources.GetObject("stopButton.Image")));
            this.stopButton.Location = new System.Drawing.Point(207, 346);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(82, 41);
            this.stopButton.TabIndex = 29;
            this.stopButton.Text = "Stop";
            this.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.btnStop_Click_1);
            // 
            // playButton
            // 
            this.playButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.playButton.Image = ((System.Drawing.Image)(resources.GetObject("playButton.Image")));
            this.playButton.Location = new System.Drawing.Point(119, 346);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(82, 41);
            this.playButton.TabIndex = 28;
            this.playButton.Text = "Play";
            this.playButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.playButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.btnPlay_Click);
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.parametersSettingToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(301, 24);
            this.menuStrip.TabIndex = 31;
            this.menuStrip.Text = "menuStrip2";
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
            // parametersSettingToolStripMenuItem
            // 
            this.parametersSettingToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deviceToolStripMenuItem,
            this.communicationCOntrolToolStripMenuItem});
            this.parametersSettingToolStripMenuItem.Name = "parametersSettingToolStripMenuItem";
            this.parametersSettingToolStripMenuItem.Size = new System.Drawing.Size(78, 20);
            this.parametersSettingToolStripMenuItem.Text = "Parameters";
            this.parametersSettingToolStripMenuItem.DropDownOpening += new System.EventHandler(this.parametersSettingToolStripMenuItem_DropDownOpening);
            // 
            // deviceToolStripMenuItem
            // 
            this.deviceToolStripMenuItem.Enabled = false;
            this.deviceToolStripMenuItem.Name = "deviceToolStripMenuItem";
            this.deviceToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.deviceToolStripMenuItem.Text = "Device";
            this.deviceToolStripMenuItem.Click += new System.EventHandler(this.deviceToolStripMenuItem_Click);
            // 
            // communicationCOntrolToolStripMenuItem
            // 
            this.communicationCOntrolToolStripMenuItem.Name = "communicationCOntrolToolStripMenuItem";
            this.communicationCOntrolToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.communicationCOntrolToolStripMenuItem.Text = "Communication";
            this.communicationCOntrolToolStripMenuItem.Click += new System.EventHandler(this.communicationCOntrolToolStripMenuItem_Click);
            // 
            // nameTextBox
            // 
            this.nameTextBox.Location = new System.Drawing.Point(83, 123);
            this.nameTextBox.Name = "nameTextBox";
            this.nameTextBox.ReadOnly = true;
            this.nameTextBox.Size = new System.Drawing.Size(181, 20);
            this.nameTextBox.TabIndex = 13;
            this.nameTextBox.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 126);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "Name";
            // 
            // modelTextBox
            // 
            this.modelTextBox.Location = new System.Drawing.Point(83, 97);
            this.modelTextBox.Name = "modelTextBox";
            this.modelTextBox.ReadOnly = true;
            this.modelTextBox.Size = new System.Drawing.Size(181, 20);
            this.modelTextBox.TabIndex = 11;
            this.modelTextBox.TabStop = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 100);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(36, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Model";
            // 
            // manufacturerTextBox
            // 
            this.manufacturerTextBox.Location = new System.Drawing.Point(83, 71);
            this.manufacturerTextBox.Name = "manufacturerTextBox";
            this.manufacturerTextBox.ReadOnly = true;
            this.manufacturerTextBox.Size = new System.Drawing.Size(181, 20);
            this.manufacturerTextBox.TabIndex = 9;
            this.manufacturerTextBox.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Manufacturer";
            // 
            // macAddressTextBox
            // 
            this.macAddressTextBox.Location = new System.Drawing.Point(83, 45);
            this.macAddressTextBox.Name = "macAddressTextBox";
            this.macAddressTextBox.ReadOnly = true;
            this.macAddressTextBox.Size = new System.Drawing.Size(181, 20);
            this.macAddressTextBox.TabIndex = 7;
            this.macAddressTextBox.TabStop = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(70, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "MAC address";
            // 
            // ipAddressTextBox
            // 
            this.ipAddressTextBox.Location = new System.Drawing.Point(83, 19);
            this.ipAddressTextBox.Name = "ipAddressTextBox";
            this.ipAddressTextBox.ReadOnly = true;
            this.ipAddressTextBox.Size = new System.Drawing.Size(181, 20);
            this.ipAddressTextBox.TabIndex = 5;
            this.ipAddressTextBox.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "IP address";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.nameTextBox);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.modelTextBox);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.manufacturerTextBox);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.macAddressTextBox);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.ipAddressTextBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 36);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(275, 158);
            this.groupBox1.TabIndex = 32;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Device";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.packetSizeTextBox);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.multicastPortTextBox);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.multicastIPTextBox);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Location = new System.Drawing.Point(12, 200);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(275, 101);
            this.groupBox2.TabIndex = 33;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Multicast Group";
            // 
            // packetSizeTextBox
            // 
            this.packetSizeTextBox.Location = new System.Drawing.Point(83, 71);
            this.packetSizeTextBox.Name = "packetSizeTextBox";
            this.packetSizeTextBox.ReadOnly = true;
            this.packetSizeTextBox.Size = new System.Drawing.Size(181, 20);
            this.packetSizeTextBox.TabIndex = 9;
            this.packetSizeTextBox.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 74);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(62, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Packet size";
            // 
            // multicastPortTextBox
            // 
            this.multicastPortTextBox.Location = new System.Drawing.Point(83, 45);
            this.multicastPortTextBox.Name = "multicastPortTextBox";
            this.multicastPortTextBox.ReadOnly = true;
            this.multicastPortTextBox.Size = new System.Drawing.Size(181, 20);
            this.multicastPortTextBox.TabIndex = 7;
            this.multicastPortTextBox.TabStop = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(7, 48);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(26, 13);
            this.label9.TabIndex = 6;
            this.label9.Text = "Port";
            // 
            // multicastIPTextBox
            // 
            this.multicastIPTextBox.Location = new System.Drawing.Point(83, 19);
            this.multicastIPTextBox.Name = "multicastIPTextBox";
            this.multicastIPTextBox.ReadOnly = true;
            this.multicastIPTextBox.Size = new System.Drawing.Size(181, 20);
            this.multicastIPTextBox.TabIndex = 5;
            this.multicastIPTextBox.TabStop = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(7, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(57, 13);
            this.label10.TabIndex = 4;
            this.label10.Text = "IP address";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(301, 399);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.playButton);
            this.Controls.Add(this.menuStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "MulticastMaster";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem parametersSettingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deviceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem communicationCOntrolToolStripMenuItem;
        private System.Windows.Forms.TextBox nameTextBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox modelTextBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox manufacturerTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox macAddressTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox ipAddressTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox multicastPortTextBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox multicastIPTextBox;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox packetSizeTextBox;
        private System.Windows.Forms.Label label6;
    }
}

