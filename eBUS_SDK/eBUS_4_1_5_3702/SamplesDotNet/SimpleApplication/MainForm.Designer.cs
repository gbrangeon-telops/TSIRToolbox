namespace SimpleApplication
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
            this.display = new PvGUIDotNet.PvDisplayControl();
            this.statusControl = new PvGUIDotNet.PvStatusControl();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.modeComboBox = new System.Windows.Forms.ComboBox();
            this.playButton = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.stopButton = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.communicationButton = new System.Windows.Forms.Button();
            this.deviceButton = new System.Windows.Forms.Button();
            this.streamButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.guidTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
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
            this.connectButton = new System.Windows.Forms.Button();
            this.disconnectButton = new System.Windows.Forms.Button();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.display);
            this.groupBox4.Controls.Add(this.statusControl);
            this.groupBox4.Location = new System.Drawing.Point(290, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(528, 449);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Display";
            // 
            // display
            // 
            this.display.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.display.BackColor = System.Drawing.Color.Transparent;
            this.display.BackgroundColor = System.Drawing.Color.DarkGray;
            this.display.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.display.Location = new System.Drawing.Point(6, 19);
            this.display.Name = "display";
            this.display.Size = new System.Drawing.Size(516, 374);
            this.display.TabIndex = 0;
            this.display.TextOverlay = "";
            this.display.TextOverlayColor = System.Drawing.Color.Lime;
            this.display.TextOverlayOffsetX = 10;
            this.display.TextOverlayOffsetY = 10;
            this.display.TextOverlaySize = 18;
            // 
            // statusControl
            // 
            this.statusControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusControl.BuffersReallocated = false;
            this.statusControl.DisplayThread = null;
            this.statusControl.Location = new System.Drawing.Point(6, 399);
            this.statusControl.MinimumSize = new System.Drawing.Size(400, 32);
            this.statusControl.Name = "statusControl";
            this.statusControl.Size = new System.Drawing.Size(516, 44);
            this.statusControl.Stream = null;
            this.statusControl.TabIndex = 1;
            this.statusControl.Warning = "";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.modeComboBox);
            this.groupBox3.Controls.Add(this.playButton);
            this.groupBox3.Controls.Add(this.stopButton);
            this.groupBox3.Location = new System.Drawing.Point(12, 233);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(272, 97);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Acquisition Control";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(34, 13);
            this.label7.TabIndex = 0;
            this.label7.Text = "Mode";
            // 
            // modeComboBox
            // 
            this.modeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modeComboBox.Enabled = false;
            this.modeComboBox.FormattingEnabled = true;
            this.modeComboBox.Location = new System.Drawing.Point(82, 19);
            this.modeComboBox.Name = "modeComboBox";
            this.modeComboBox.Size = new System.Drawing.Size(181, 21);
            this.modeComboBox.TabIndex = 1;
            this.modeComboBox.SelectedIndexChanged += new System.EventHandler(this.modeComboBox_SelectedIndexChanged);
            // 
            // playButton
            // 
            this.playButton.Enabled = false;
            this.playButton.ImageIndex = 0;
            this.playButton.ImageList = this.imageList;
            this.playButton.Location = new System.Drawing.Point(9, 46);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(123, 39);
            this.playButton.TabIndex = 2;
            this.playButton.Text = "Play";
            this.playButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.playButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.playButton_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.White;
            this.imageList.Images.SetKeyName(0, "play.bmp");
            this.imageList.Images.SetKeyName(1, "stop.bmp");
            // 
            // stopButton
            // 
            this.stopButton.Enabled = false;
            this.stopButton.ImageIndex = 1;
            this.stopButton.ImageList = this.imageList;
            this.stopButton.Location = new System.Drawing.Point(138, 46);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(125, 39);
            this.stopButton.TabIndex = 3;
            this.stopButton.Text = "Stop";
            this.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.communicationButton);
            this.groupBox2.Controls.Add(this.deviceButton);
            this.groupBox2.Controls.Add(this.streamButton);
            this.groupBox2.Location = new System.Drawing.Point(12, 336);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(272, 125);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Parameters and Controls";
            // 
            // communicationButton
            // 
            this.communicationButton.Enabled = false;
            this.communicationButton.Location = new System.Drawing.Point(6, 25);
            this.communicationButton.Name = "communicationButton";
            this.communicationButton.Size = new System.Drawing.Size(257, 26);
            this.communicationButton.TabIndex = 0;
            this.communicationButton.Text = "Communication control";
            this.communicationButton.UseVisualStyleBackColor = true;
            this.communicationButton.Click += new System.EventHandler(this.communicationButton_Click);
            // 
            // deviceButton
            // 
            this.deviceButton.Enabled = false;
            this.deviceButton.Location = new System.Drawing.Point(6, 57);
            this.deviceButton.Name = "deviceButton";
            this.deviceButton.Size = new System.Drawing.Size(257, 26);
            this.deviceButton.TabIndex = 1;
            this.deviceButton.Text = "Device control";
            this.deviceButton.UseVisualStyleBackColor = true;
            this.deviceButton.Click += new System.EventHandler(this.deviceButton_Click);
            // 
            // streamButton
            // 
            this.streamButton.Enabled = false;
            this.streamButton.Location = new System.Drawing.Point(6, 89);
            this.streamButton.Name = "streamButton";
            this.streamButton.Size = new System.Drawing.Size(257, 26);
            this.streamButton.TabIndex = 2;
            this.streamButton.Text = "Image stream control";
            this.streamButton.UseVisualStyleBackColor = true;
            this.streamButton.Click += new System.EventHandler(this.streamButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.guidTextBox);
            this.groupBox1.Controls.Add(this.label6);
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
            this.groupBox1.Controls.Add(this.connectButton);
            this.groupBox1.Controls.Add(this.disconnectButton);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(272, 215);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Connection";
            // 
            // guidTextBox
            // 
            this.guidTextBox.Location = new System.Drawing.Point(82, 109);
            this.guidTextBox.Name = "guidTextBox";
            this.guidTextBox.ReadOnly = true;
            this.guidTextBox.Size = new System.Drawing.Size(181, 20);
            this.guidTextBox.TabIndex = 7;
            this.guidTextBox.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 112);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(34, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "GUID";
            // 
            // nameTextBox
            // 
            this.nameTextBox.Location = new System.Drawing.Point(82, 187);
            this.nameTextBox.Name = "nameTextBox";
            this.nameTextBox.ReadOnly = true;
            this.nameTextBox.Size = new System.Drawing.Size(181, 20);
            this.nameTextBox.TabIndex = 13;
            this.nameTextBox.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 190);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "Name";
            // 
            // modelTextBox
            // 
            this.modelTextBox.Location = new System.Drawing.Point(82, 161);
            this.modelTextBox.Name = "modelTextBox";
            this.modelTextBox.ReadOnly = true;
            this.modelTextBox.Size = new System.Drawing.Size(181, 20);
            this.modelTextBox.TabIndex = 11;
            this.modelTextBox.TabStop = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 164);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(36, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Model";
            // 
            // manufacturerTextBox
            // 
            this.manufacturerTextBox.Location = new System.Drawing.Point(82, 135);
            this.manufacturerTextBox.Name = "manufacturerTextBox";
            this.manufacturerTextBox.ReadOnly = true;
            this.manufacturerTextBox.Size = new System.Drawing.Size(181, 20);
            this.manufacturerTextBox.TabIndex = 9;
            this.manufacturerTextBox.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 138);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Manufacturer";
            // 
            // macAddressTextBox
            // 
            this.macAddressTextBox.Location = new System.Drawing.Point(82, 83);
            this.macAddressTextBox.Name = "macAddressTextBox";
            this.macAddressTextBox.ReadOnly = true;
            this.macAddressTextBox.Size = new System.Drawing.Size(181, 20);
            this.macAddressTextBox.TabIndex = 5;
            this.macAddressTextBox.TabStop = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 86);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(70, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "MAC address";
            // 
            // ipAddressTextBox
            // 
            this.ipAddressTextBox.Location = new System.Drawing.Point(82, 57);
            this.ipAddressTextBox.Name = "ipAddressTextBox";
            this.ipAddressTextBox.ReadOnly = true;
            this.ipAddressTextBox.Size = new System.Drawing.Size(181, 20);
            this.ipAddressTextBox.TabIndex = 3;
            this.ipAddressTextBox.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "IP address";
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(6, 19);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(126, 26);
            this.connectButton.TabIndex = 0;
            this.connectButton.Text = "Select / Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // disconnectButton
            // 
            this.disconnectButton.Enabled = false;
            this.disconnectButton.Location = new System.Drawing.Point(138, 19);
            this.disconnectButton.Name = "disconnectButton";
            this.disconnectButton.Size = new System.Drawing.Size(125, 26);
            this.disconnectButton.TabIndex = 1;
            this.disconnectButton.Text = "Disconnect";
            this.disconnectButton.UseVisualStyleBackColor = true;
            this.disconnectButton.Click += new System.EventHandler(this.disconnectButton_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(830, 474);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(846, 512);
            this.Name = "MainForm";
            this.Text = "SimpleApplication";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox4.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox4;
        private PvGUIDotNet.PvDisplayControl display;
        private PvGUIDotNet.PvStatusControl statusControl;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox modeComboBox;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button communicationButton;
        private System.Windows.Forms.Button deviceButton;
        private System.Windows.Forms.Button streamButton;
        private System.Windows.Forms.GroupBox groupBox1;
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
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button disconnectButton;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.TextBox guidTextBox;
        private System.Windows.Forms.Label label6;
    }
}

