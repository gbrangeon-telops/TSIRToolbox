namespace TransmitTiledImages
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
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connect11Button = new System.Windows.Forms.Button();
            this.connect12Button = new System.Windows.Forms.Button();
            this.connect21Button = new System.Windows.Forms.Button();
            this.connect22Button = new System.Windows.Forms.Button();
            this.outputGroupBox = new System.Windows.Forms.GroupBox();
            this.tilingModeComboBox = new System.Windows.Forms.ComboBox();
            this.tilingModeLabel = new System.Windows.Forms.Label();
            this.heightTextBox = new System.Windows.Forms.TextBox();
            this.heightLabel = new System.Windows.Forms.Label();
            this.widthTextBox = new System.Windows.Forms.TextBox();
            this.widthLabel = new System.Windows.Forms.Label();
            this.videoOutputGroupBox = new System.Windows.Forms.GroupBox();
            this.maxPacketSizeTextBox = new System.Windows.Forms.TextBox();
            this.maxPacketSizeLabel = new System.Windows.Forms.Label();
            this.desiredFrameRateTextBox = new System.Windows.Forms.TextBox();
            this.desiredFrameRateLabel = new System.Windows.Forms.Label();
            this.interfaceAddressComboBox = new System.Windows.Forms.ComboBox();
            this.interfaceAddressLabel = new System.Windows.Forms.Label();
            this.destinationPortTextBox = new System.Windows.Forms.TextBox();
            this.destinationPortLabel = new System.Windows.Forms.Label();
            this.destinationAddressTextBox = new System.Windows.Forms.TextBox();
            this.destinationAddressLabel = new System.Windows.Forms.Label();
            this.transmitterStatusLabel = new System.Windows.Forms.Label();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.displayGroupBox = new System.Windows.Forms.GroupBox();
            this.pvDisplayControl = new PvGUIDotNet.PvDisplayControl();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.browserButton1 = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.modelLabel1 = new System.Windows.Forms.Label();
            this.macLabel1 = new System.Windows.Forms.Label();
            this.ipLabel1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.browserButton2 = new System.Windows.Forms.Button();
            this.modelLabel2 = new System.Windows.Forms.Label();
            this.macLabel2 = new System.Windows.Forms.Label();
            this.ipLabel2 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.browserButton3 = new System.Windows.Forms.Button();
            this.modelLabel3 = new System.Windows.Forms.Label();
            this.macLabel3 = new System.Windows.Forms.Label();
            this.ipLabel3 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.browserButton4 = new System.Windows.Forms.Button();
            this.modelLabel4 = new System.Windows.Forms.Label();
            this.macLabel4 = new System.Windows.Forms.Label();
            this.ipLabel4 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.startButton = new System.Windows.Forms.Button();
            this.stopButton = new System.Windows.Forms.Button();
            this.browserMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.commMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deviceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.streamMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.errorProvider = new System.Windows.Forms.ErrorProvider(this.components);
            this.serialNumber1 = new System.Windows.Forms.Label();
            this.serialNumber2 = new System.Windows.Forms.Label();
            this.serialNumber3 = new System.Windows.Forms.Label();
            this.serialNumber4 = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.outputGroupBox.SuspendLayout();
            this.videoOutputGroupBox.SuspendLayout();
            this.displayGroupBox.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.browserMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1128, 24);
            this.menuStrip1.TabIndex = 8;
            this.menuStrip1.Text = "menuStrip";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.openToolStripMenuItem.Text = "Open...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Enabled = false;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(230, 22);
            this.aboutToolStripMenuItem.Text = "About TransmitTiledImages...";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // connect11Button
            // 
            this.connect11Button.Location = new System.Drawing.Point(6, 72);
            this.connect11Button.Name = "connect11Button";
            this.connect11Button.Size = new System.Drawing.Size(156, 26);
            this.connect11Button.TabIndex = 3;
            this.connect11Button.Text = "Select/Connect";
            this.connect11Button.UseVisualStyleBackColor = true;
            this.connect11Button.Click += new System.EventHandler(this.connect11Button_Click);
            // 
            // connect12Button
            // 
            this.connect12Button.Location = new System.Drawing.Point(6, 72);
            this.connect12Button.Name = "connect12Button";
            this.connect12Button.Size = new System.Drawing.Size(154, 26);
            this.connect12Button.TabIndex = 4;
            this.connect12Button.Text = "Select/Connect";
            this.connect12Button.UseVisualStyleBackColor = true;
            this.connect12Button.Click += new System.EventHandler(this.connect12Button_Click);
            // 
            // connect21Button
            // 
            this.connect21Button.Location = new System.Drawing.Point(6, 70);
            this.connect21Button.Name = "connect21Button";
            this.connect21Button.Size = new System.Drawing.Size(156, 26);
            this.connect21Button.TabIndex = 3;
            this.connect21Button.Text = "Select/Connect";
            this.connect21Button.UseVisualStyleBackColor = true;
            this.connect21Button.Click += new System.EventHandler(this.connect21Button_Click);
            // 
            // connect22Button
            // 
            this.connect22Button.Location = new System.Drawing.Point(6, 70);
            this.connect22Button.Name = "connect22Button";
            this.connect22Button.Size = new System.Drawing.Size(154, 26);
            this.connect22Button.TabIndex = 3;
            this.connect22Button.Text = "Select/Connect";
            this.connect22Button.UseVisualStyleBackColor = true;
            this.connect22Button.Click += new System.EventHandler(this.connect22Button_Click);
            // 
            // outputGroupBox
            // 
            this.outputGroupBox.Controls.Add(this.tilingModeComboBox);
            this.outputGroupBox.Controls.Add(this.tilingModeLabel);
            this.outputGroupBox.Controls.Add(this.heightTextBox);
            this.outputGroupBox.Controls.Add(this.heightLabel);
            this.outputGroupBox.Controls.Add(this.widthTextBox);
            this.outputGroupBox.Controls.Add(this.widthLabel);
            this.outputGroupBox.Location = new System.Drawing.Point(12, 322);
            this.outputGroupBox.Name = "outputGroupBox";
            this.outputGroupBox.Size = new System.Drawing.Size(450, 102);
            this.outputGroupBox.TabIndex = 5;
            this.outputGroupBox.TabStop = false;
            this.outputGroupBox.Text = "Video Output";
            // 
            // tilingModeComboBox
            // 
            this.tilingModeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tilingModeComboBox.FormattingEnabled = true;
            this.tilingModeComboBox.Items.AddRange(new object[] {
            "Maintain Aspect Ratio",
            "Maintain Resolution",
            "Stretch to Fit"});
            this.tilingModeComboBox.Location = new System.Drawing.Point(145, 72);
            this.tilingModeComboBox.Name = "tilingModeComboBox";
            this.tilingModeComboBox.Size = new System.Drawing.Size(154, 21);
            this.tilingModeComboBox.TabIndex = 5;
            // 
            // tilingModeLabel
            // 
            this.tilingModeLabel.AutoSize = true;
            this.tilingModeLabel.Location = new System.Drawing.Point(7, 75);
            this.tilingModeLabel.Name = "tilingModeLabel";
            this.tilingModeLabel.Size = new System.Drawing.Size(65, 13);
            this.tilingModeLabel.TabIndex = 4;
            this.tilingModeLabel.Text = "Tiling Mode:";
            // 
            // heightTextBox
            // 
            this.heightTextBox.Location = new System.Drawing.Point(145, 44);
            this.heightTextBox.Name = "heightTextBox";
            this.heightTextBox.Size = new System.Drawing.Size(109, 20);
            this.heightTextBox.TabIndex = 3;
            this.heightTextBox.Text = "480";
            this.heightTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.heightTextBox_Validating);
            // 
            // heightLabel
            // 
            this.heightLabel.AutoSize = true;
            this.heightLabel.Location = new System.Drawing.Point(8, 47);
            this.heightLabel.Name = "heightLabel";
            this.heightLabel.Size = new System.Drawing.Size(41, 13);
            this.heightLabel.TabIndex = 2;
            this.heightLabel.Text = "Height:";
            // 
            // widthTextBox
            // 
            this.widthTextBox.Location = new System.Drawing.Point(145, 19);
            this.widthTextBox.Name = "widthTextBox";
            this.widthTextBox.Size = new System.Drawing.Size(109, 20);
            this.widthTextBox.TabIndex = 1;
            this.widthTextBox.Text = "640";
            this.widthTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.widthTextBox_Validating);
            // 
            // widthLabel
            // 
            this.widthLabel.AutoSize = true;
            this.widthLabel.Location = new System.Drawing.Point(8, 22);
            this.widthLabel.Name = "widthLabel";
            this.widthLabel.Size = new System.Drawing.Size(38, 13);
            this.widthLabel.TabIndex = 0;
            this.widthLabel.Text = "Width:";
            // 
            // videoOutputGroupBox
            // 
            this.videoOutputGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.videoOutputGroupBox.Controls.Add(this.maxPacketSizeTextBox);
            this.videoOutputGroupBox.Controls.Add(this.maxPacketSizeLabel);
            this.videoOutputGroupBox.Controls.Add(this.desiredFrameRateTextBox);
            this.videoOutputGroupBox.Controls.Add(this.desiredFrameRateLabel);
            this.videoOutputGroupBox.Controls.Add(this.interfaceAddressComboBox);
            this.videoOutputGroupBox.Controls.Add(this.interfaceAddressLabel);
            this.videoOutputGroupBox.Controls.Add(this.destinationPortTextBox);
            this.videoOutputGroupBox.Controls.Add(this.destinationPortLabel);
            this.videoOutputGroupBox.Controls.Add(this.destinationAddressTextBox);
            this.videoOutputGroupBox.Controls.Add(this.destinationAddressLabel);
            this.videoOutputGroupBox.Location = new System.Drawing.Point(12, 430);
            this.videoOutputGroupBox.Name = "videoOutputGroupBox";
            this.videoOutputGroupBox.Size = new System.Drawing.Size(450, 152);
            this.videoOutputGroupBox.TabIndex = 6;
            this.videoOutputGroupBox.TabStop = false;
            this.videoOutputGroupBox.Text = "Transmission";
            // 
            // maxPacketSizeTextBox
            // 
            this.maxPacketSizeTextBox.Location = new System.Drawing.Point(151, 124);
            this.maxPacketSizeTextBox.Name = "maxPacketSizeTextBox";
            this.maxPacketSizeTextBox.Size = new System.Drawing.Size(103, 20);
            this.maxPacketSizeTextBox.TabIndex = 9;
            this.maxPacketSizeTextBox.Text = "1476";
            this.maxPacketSizeTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.maxPacketSizeTextBox_Validating);
            // 
            // maxPacketSizeLabel
            // 
            this.maxPacketSizeLabel.AutoSize = true;
            this.maxPacketSizeLabel.Location = new System.Drawing.Point(9, 127);
            this.maxPacketSizeLabel.Name = "maxPacketSizeLabel";
            this.maxPacketSizeLabel.Size = new System.Drawing.Size(114, 13);
            this.maxPacketSizeLabel.TabIndex = 8;
            this.maxPacketSizeLabel.Text = "Maximum Packet Size:";
            // 
            // desiredFrameRateTextBox
            // 
            this.desiredFrameRateTextBox.Location = new System.Drawing.Point(151, 99);
            this.desiredFrameRateTextBox.Name = "desiredFrameRateTextBox";
            this.desiredFrameRateTextBox.Size = new System.Drawing.Size(103, 20);
            this.desiredFrameRateTextBox.TabIndex = 7;
            this.desiredFrameRateTextBox.Text = "30";
            this.desiredFrameRateTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.desiredFrameRateTextBox_Validating);
            // 
            // desiredFrameRateLabel
            // 
            this.desiredFrameRateLabel.AutoSize = true;
            this.desiredFrameRateLabel.Location = new System.Drawing.Point(9, 102);
            this.desiredFrameRateLabel.Name = "desiredFrameRateLabel";
            this.desiredFrameRateLabel.Size = new System.Drawing.Size(127, 13);
            this.desiredFrameRateLabel.TabIndex = 6;
            this.desiredFrameRateLabel.Text = "Desired Frame Rate (fps):";
            // 
            // interfaceAddressComboBox
            // 
            this.interfaceAddressComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.interfaceAddressComboBox.FormattingEnabled = true;
            this.interfaceAddressComboBox.Location = new System.Drawing.Point(151, 72);
            this.interfaceAddressComboBox.Name = "interfaceAddressComboBox";
            this.interfaceAddressComboBox.Size = new System.Drawing.Size(148, 21);
            this.interfaceAddressComboBox.TabIndex = 5;
            this.interfaceAddressComboBox.SelectedIndexChanged += new System.EventHandler(this.interfaceAddressComboBox_SelectedIndexChanged);
            // 
            // interfaceAddressLabel
            // 
            this.interfaceAddressLabel.AutoSize = true;
            this.interfaceAddressLabel.Location = new System.Drawing.Point(8, 75);
            this.interfaceAddressLabel.Name = "interfaceAddressLabel";
            this.interfaceAddressLabel.Size = new System.Drawing.Size(93, 13);
            this.interfaceAddressLabel.TabIndex = 4;
            this.interfaceAddressLabel.Text = "Interface Address:";
            // 
            // destinationPortTextBox
            // 
            this.destinationPortTextBox.Location = new System.Drawing.Point(151, 44);
            this.destinationPortTextBox.Name = "destinationPortTextBox";
            this.destinationPortTextBox.Size = new System.Drawing.Size(103, 20);
            this.destinationPortTextBox.TabIndex = 3;
            this.destinationPortTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.destinationPortTextBox_Validating);
            // 
            // destinationPortLabel
            // 
            this.destinationPortLabel.AutoSize = true;
            this.destinationPortLabel.Location = new System.Drawing.Point(9, 47);
            this.destinationPortLabel.Name = "destinationPortLabel";
            this.destinationPortLabel.Size = new System.Drawing.Size(85, 13);
            this.destinationPortLabel.TabIndex = 2;
            this.destinationPortLabel.Text = "Destination Port:";
            // 
            // destinationAddressTextBox
            // 
            this.destinationAddressTextBox.Location = new System.Drawing.Point(151, 19);
            this.destinationAddressTextBox.Name = "destinationAddressTextBox";
            this.destinationAddressTextBox.Size = new System.Drawing.Size(103, 20);
            this.destinationAddressTextBox.TabIndex = 1;
            this.destinationAddressTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.destinationAddressTextBox_Validating);
            // 
            // destinationAddressLabel
            // 
            this.destinationAddressLabel.AutoSize = true;
            this.destinationAddressLabel.Location = new System.Drawing.Point(9, 22);
            this.destinationAddressLabel.Name = "destinationAddressLabel";
            this.destinationAddressLabel.Size = new System.Drawing.Size(104, 13);
            this.destinationAddressLabel.TabIndex = 0;
            this.destinationAddressLabel.Text = "Destination Address:";
            // 
            // transmitterStatusLabel
            // 
            this.transmitterStatusLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.transmitterStatusLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.transmitterStatusLabel.Location = new System.Drawing.Point(6, 508);
            this.transmitterStatusLabel.Name = "transmitterStatusLabel";
            this.transmitterStatusLabel.Size = new System.Drawing.Size(636, 40);
            this.transmitterStatusLabel.TabIndex = 1;
            // 
            // timer
            // 
            this.timer.Interval = 300;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // displayGroupBox
            // 
            this.displayGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.displayGroupBox.Controls.Add(this.pvDisplayControl);
            this.displayGroupBox.Controls.Add(this.transmitterStatusLabel);
            this.displayGroupBox.Location = new System.Drawing.Point(468, 27);
            this.displayGroupBox.Name = "displayGroupBox";
            this.displayGroupBox.Size = new System.Drawing.Size(648, 555);
            this.displayGroupBox.TabIndex = 7;
            this.displayGroupBox.TabStop = false;
            this.displayGroupBox.Text = "Display";
            //
            // pvDisplayControl
            //
            this.pvDisplayControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
            | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pvDisplayControl.BackColor = System.Drawing.Color.Transparent;
            this.pvDisplayControl.BackgroundColor = System.Drawing.Color.DarkGray;
            this.pvDisplayControl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pvDisplayControl.Location = new System.Drawing.Point(6, 19);
            this.pvDisplayControl.Name = "pvDisplayControl";
            this.pvDisplayControl.Size = new System.Drawing.Size(636, 482);
            this.pvDisplayControl.TabIndex = 0;
            this.pvDisplayControl.TextOverlay = "";
            this.pvDisplayControl.TextOverlayColor = System.Drawing.Color.Lime;
            this.pvDisplayControl.TextOverlayOffsetX = 10;
            this.pvDisplayControl.TextOverlayOffsetY = 10;
            this.pvDisplayControl.TextOverlaySize = 18;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.serialNumber1);
            this.groupBox1.Controls.Add(this.browserButton1);
            this.groupBox1.Controls.Add(this.modelLabel1);
            this.groupBox1.Controls.Add(this.macLabel1);
            this.groupBox1.Controls.Add(this.ipLabel1);
            this.groupBox1.Controls.Add(this.connect11Button);
            this.groupBox1.Location = new System.Drawing.Point(12, 27);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(223, 105);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Video Source 1";
            // 
            // browserButton1
            // 
            this.browserButton1.ImageIndex = 2;
            this.browserButton1.ImageList = this.imageList;
            this.browserButton1.Location = new System.Drawing.Point(168, 72);
            this.browserButton1.Name = "browserButton1";
            this.browserButton1.Size = new System.Drawing.Size(49, 26);
            this.browserButton1.TabIndex = 5;
            this.browserButton1.UseVisualStyleBackColor = true;
            this.browserButton1.Click += new System.EventHandler(this.browserButton_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.White;
            this.imageList.Images.SetKeyName(0, "play.bmp");
            this.imageList.Images.SetKeyName(1, "stop.bmp");
            this.imageList.Images.SetKeyName(2, "downarrow.bmp");
            // 
            // modelLabel1
            // 
            this.modelLabel1.AutoSize = true;
            this.modelLabel1.Location = new System.Drawing.Point(6, 56);
            this.modelLabel1.Name = "modelLabel1";
            this.modelLabel1.Size = new System.Drawing.Size(39, 13);
            this.modelLabel1.TabIndex = 2;
            this.modelLabel1.Text = "Model:";
            // 
            // macLabel1
            // 
            this.macLabel1.AutoSize = true;
            this.macLabel1.Location = new System.Drawing.Point(6, 39);
            this.macLabel1.Name = "macLabel1";
            this.macLabel1.Size = new System.Drawing.Size(73, 13);
            this.macLabel1.TabIndex = 1;
            this.macLabel1.Text = "MAC address:";
            // 
            // ipLabel1
            // 
            this.ipLabel1.AutoSize = true;
            this.ipLabel1.Location = new System.Drawing.Point(6, 22);
            this.ipLabel1.Name = "ipLabel1";
            this.ipLabel1.Size = new System.Drawing.Size(60, 13);
            this.ipLabel1.TabIndex = 0;
            this.ipLabel1.Text = "IP address:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.serialNumber2);
            this.groupBox2.Controls.Add(this.browserButton2);
            this.groupBox2.Controls.Add(this.modelLabel2);
            this.groupBox2.Controls.Add(this.connect12Button);
            this.groupBox2.Controls.Add(this.macLabel2);
            this.groupBox2.Controls.Add(this.ipLabel2);
            this.groupBox2.Location = new System.Drawing.Point(241, 27);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(221, 105);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Video Source 2";
            // 
            // browserButton2
            // 
            this.browserButton2.ImageIndex = 2;
            this.browserButton2.ImageList = this.imageList;
            this.browserButton2.Location = new System.Drawing.Point(166, 72);
            this.browserButton2.Name = "browserButton2";
            this.browserButton2.Size = new System.Drawing.Size(49, 26);
            this.browserButton2.TabIndex = 6;
            this.browserButton2.UseVisualStyleBackColor = true;
            this.browserButton2.Click += new System.EventHandler(this.browserButton_Click);
            // 
            // modelLabel2
            // 
            this.modelLabel2.AutoSize = true;
            this.modelLabel2.Location = new System.Drawing.Point(6, 56);
            this.modelLabel2.Name = "modelLabel2";
            this.modelLabel2.Size = new System.Drawing.Size(39, 13);
            this.modelLabel2.TabIndex = 2;
            this.modelLabel2.Text = "Model:";
            // 
            // macLabel2
            // 
            this.macLabel2.AutoSize = true;
            this.macLabel2.Location = new System.Drawing.Point(6, 39);
            this.macLabel2.Name = "macLabel2";
            this.macLabel2.Size = new System.Drawing.Size(73, 13);
            this.macLabel2.TabIndex = 1;
            this.macLabel2.Text = "MAC address:";
            // 
            // ipLabel2
            // 
            this.ipLabel2.AutoSize = true;
            this.ipLabel2.Location = new System.Drawing.Point(6, 22);
            this.ipLabel2.Name = "ipLabel2";
            this.ipLabel2.Size = new System.Drawing.Size(60, 13);
            this.ipLabel2.TabIndex = 0;
            this.ipLabel2.Text = "IP address:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.serialNumber3);
            this.groupBox3.Controls.Add(this.browserButton3);
            this.groupBox3.Controls.Add(this.modelLabel3);
            this.groupBox3.Controls.Add(this.macLabel3);
            this.groupBox3.Controls.Add(this.ipLabel3);
            this.groupBox3.Controls.Add(this.connect21Button);
            this.groupBox3.Location = new System.Drawing.Point(12, 138);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(223, 105);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Video Source 3";
            // 
            // browserButton3
            // 
            this.browserButton3.ImageIndex = 2;
            this.browserButton3.ImageList = this.imageList;
            this.browserButton3.Location = new System.Drawing.Point(168, 70);
            this.browserButton3.Name = "browserButton3";
            this.browserButton3.Size = new System.Drawing.Size(49, 26);
            this.browserButton3.TabIndex = 7;
            this.browserButton3.UseVisualStyleBackColor = true;
            this.browserButton3.Click += new System.EventHandler(this.browserButton_Click);
            // 
            // modelLabel3
            // 
            this.modelLabel3.AutoSize = true;
            this.modelLabel3.Location = new System.Drawing.Point(6, 54);
            this.modelLabel3.Name = "modelLabel3";
            this.modelLabel3.Size = new System.Drawing.Size(39, 13);
            this.modelLabel3.TabIndex = 2;
            this.modelLabel3.Text = "Model:";
            // 
            // macLabel3
            // 
            this.macLabel3.AutoSize = true;
            this.macLabel3.Location = new System.Drawing.Point(6, 37);
            this.macLabel3.Name = "macLabel3";
            this.macLabel3.Size = new System.Drawing.Size(73, 13);
            this.macLabel3.TabIndex = 1;
            this.macLabel3.Text = "MAC address:";
            // 
            // ipLabel3
            // 
            this.ipLabel3.AutoSize = true;
            this.ipLabel3.Location = new System.Drawing.Point(6, 20);
            this.ipLabel3.Name = "ipLabel3";
            this.ipLabel3.Size = new System.Drawing.Size(60, 13);
            this.ipLabel3.TabIndex = 0;
            this.ipLabel3.Text = "IP address:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.serialNumber4);
            this.groupBox4.Controls.Add(this.browserButton4);
            this.groupBox4.Controls.Add(this.modelLabel4);
            this.groupBox4.Controls.Add(this.macLabel4);
            this.groupBox4.Controls.Add(this.connect22Button);
            this.groupBox4.Controls.Add(this.ipLabel4);
            this.groupBox4.Location = new System.Drawing.Point(241, 138);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(221, 105);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Video Source 4";
            // 
            // browserButton4
            // 
            this.browserButton4.ImageIndex = 2;
            this.browserButton4.ImageList = this.imageList;
            this.browserButton4.Location = new System.Drawing.Point(166, 70);
            this.browserButton4.Name = "browserButton4";
            this.browserButton4.Size = new System.Drawing.Size(49, 26);
            this.browserButton4.TabIndex = 7;
            this.browserButton4.UseVisualStyleBackColor = true;
            this.browserButton4.Click += new System.EventHandler(this.browserButton_Click);
            // 
            // modelLabel4
            // 
            this.modelLabel4.AutoSize = true;
            this.modelLabel4.Location = new System.Drawing.Point(6, 54);
            this.modelLabel4.Name = "modelLabel4";
            this.modelLabel4.Size = new System.Drawing.Size(39, 13);
            this.modelLabel4.TabIndex = 2;
            this.modelLabel4.Text = "Model:";
            // 
            // macLabel4
            // 
            this.macLabel4.AutoSize = true;
            this.macLabel4.Location = new System.Drawing.Point(6, 37);
            this.macLabel4.Name = "macLabel4";
            this.macLabel4.Size = new System.Drawing.Size(73, 13);
            this.macLabel4.TabIndex = 1;
            this.macLabel4.Text = "MAC address:";
            // 
            // ipLabel4
            // 
            this.ipLabel4.AutoSize = true;
            this.ipLabel4.Location = new System.Drawing.Point(6, 20);
            this.ipLabel4.Name = "ipLabel4";
            this.ipLabel4.Size = new System.Drawing.Size(60, 13);
            this.ipLabel4.TabIndex = 0;
            this.ipLabel4.Text = "IP address:";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.startButton);
            this.groupBox5.Controls.Add(this.stopButton);
            this.groupBox5.Location = new System.Drawing.Point(12, 249);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(450, 67);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Acquisition Controls";
            // 
            // startButton
            // 
            this.startButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.startButton.ImageIndex = 0;
            this.startButton.ImageList = this.imageList;
            this.startButton.Location = new System.Drawing.Point(6, 19);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(125, 39);
            this.startButton.TabIndex = 0;
            this.startButton.Text = "Play";
            this.startButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.startButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // stopButton
            // 
            this.stopButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.stopButton.ImageIndex = 1;
            this.stopButton.ImageList = this.imageList;
            this.stopButton.Location = new System.Drawing.Point(137, 19);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(125, 39);
            this.stopButton.TabIndex = 1;
            this.stopButton.Text = "Stop";
            this.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // browserMenu
            // 
            this.browserMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.commMenuItem,
            this.deviceMenuItem,
            this.streamMenuItem});
            this.browserMenu.Name = "browserMenu";
            this.browserMenu.Size = new System.Drawing.Size(224, 70);
            // 
            // commMenuItem
            // 
            this.commMenuItem.Name = "commMenuItem";
            this.commMenuItem.Size = new System.Drawing.Size(223, 22);
            this.commMenuItem.Text = "Communication parameters";
            this.commMenuItem.Click += new System.EventHandler(this.commMenuItem_Click);
            // 
            // deviceMenuItem
            // 
            this.deviceMenuItem.Name = "deviceMenuItem";
            this.deviceMenuItem.Size = new System.Drawing.Size(223, 22);
            this.deviceMenuItem.Text = "Device parameters";
            this.deviceMenuItem.Click += new System.EventHandler(this.deviceMenuItem_Click);
            // 
            // streamMenuItem
            // 
            this.streamMenuItem.Name = "streamMenuItem";
            this.streamMenuItem.Size = new System.Drawing.Size(223, 22);
            this.streamMenuItem.Text = "Image stream parameters";
            this.streamMenuItem.Click += new System.EventHandler(this.streamMenuItem_Click);
            // 
            // errorProvider
            // 
            this.errorProvider.ContainerControl = this;
            // 
            // serialNumber1
            // 
            this.serialNumber1.AutoSize = true;
            this.serialNumber1.Location = new System.Drawing.Point(7, 22);
            this.serialNumber1.Name = "serialNumber1";
            this.serialNumber1.Size = new System.Drawing.Size(74, 13);
            this.serialNumber1.TabIndex = 6;
            this.serialNumber1.Text = "Serial number:";
            // 
            // serialNumber2
            // 
            this.serialNumber2.AutoSize = true;
            this.serialNumber2.Location = new System.Drawing.Point(6, 22);
            this.serialNumber2.Name = "serialNumber2";
            this.serialNumber2.Size = new System.Drawing.Size(74, 13);
            this.serialNumber2.TabIndex = 7;
            this.serialNumber2.Text = "Serial number:";
            // 
            // serialNumber3
            // 
            this.serialNumber3.AutoSize = true;
            this.serialNumber3.Location = new System.Drawing.Point(6, 20);
            this.serialNumber3.Name = "serialNumber3";
            this.serialNumber3.Size = new System.Drawing.Size(74, 13);
            this.serialNumber3.TabIndex = 9;
            this.serialNumber3.Text = "Serial number:";
            // 
            // serialNumber4
            // 
            this.serialNumber4.AutoSize = true;
            this.serialNumber4.Location = new System.Drawing.Point(6, 20);
            this.serialNumber4.Name = "serialNumber4";
            this.serialNumber4.Size = new System.Drawing.Size(74, 13);
            this.serialNumber4.TabIndex = 7;
            this.serialNumber4.Text = "Serial number:";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1128, 592);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.displayGroupBox);
            this.Controls.Add(this.videoOutputGroupBox);
            this.Controls.Add(this.outputGroupBox);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TransmitTiledImages";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.outputGroupBox.ResumeLayout(false);
            this.outputGroupBox.PerformLayout();
            this.videoOutputGroupBox.ResumeLayout(false);
            this.videoOutputGroupBox.PerformLayout();
            this.displayGroupBox.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.browserMenu.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

#endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Button connect11Button;
        private System.Windows.Forms.Button connect12Button;
        private System.Windows.Forms.Button connect21Button;
        private System.Windows.Forms.Button connect22Button;
        private System.Windows.Forms.GroupBox outputGroupBox;
        private System.Windows.Forms.ComboBox tilingModeComboBox;
        private System.Windows.Forms.Label tilingModeLabel;
        private System.Windows.Forms.TextBox heightTextBox;
        private System.Windows.Forms.Label heightLabel;
        private System.Windows.Forms.TextBox widthTextBox;
        private System.Windows.Forms.Label widthLabel;
        private System.Windows.Forms.GroupBox videoOutputGroupBox;
        private System.Windows.Forms.TextBox maxPacketSizeTextBox;
        private System.Windows.Forms.Label maxPacketSizeLabel;
        private System.Windows.Forms.TextBox desiredFrameRateTextBox;
        private System.Windows.Forms.Label desiredFrameRateLabel;
        private System.Windows.Forms.ComboBox interfaceAddressComboBox;
        private System.Windows.Forms.Label interfaceAddressLabel;
        private System.Windows.Forms.TextBox destinationPortTextBox;
        private System.Windows.Forms.Label destinationPortLabel;
        private System.Windows.Forms.TextBox destinationAddressTextBox;
        private System.Windows.Forms.Label destinationAddressLabel;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.Label transmitterStatusLabel;
        private System.Windows.Forms.Timer timer;
        private PvGUIDotNet.PvDisplayControl pvDisplayControl;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.GroupBox displayGroupBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label modelLabel1;
        private System.Windows.Forms.Label macLabel1;
        private System.Windows.Forms.Label ipLabel1;
        private System.Windows.Forms.Label modelLabel2;
        private System.Windows.Forms.Label macLabel2;
        private System.Windows.Forms.Label ipLabel2;
        private System.Windows.Forms.Label modelLabel3;
        private System.Windows.Forms.Label macLabel3;
        private System.Windows.Forms.Label ipLabel3;
        private System.Windows.Forms.Label modelLabel4;
        private System.Windows.Forms.Label macLabel4;
        private System.Windows.Forms.Label ipLabel4;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button browserButton1;
        private System.Windows.Forms.ContextMenuStrip browserMenu;
        private System.Windows.Forms.ToolStripMenuItem commMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deviceMenuItem;
        private System.Windows.Forms.ToolStripMenuItem streamMenuItem;
        private System.Windows.Forms.Button browserButton2;
        private System.Windows.Forms.Button browserButton3;
        private System.Windows.Forms.Button browserButton4;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ErrorProvider errorProvider;
        private System.Windows.Forms.Label serialNumber1;
        private System.Windows.Forms.Label serialNumber2;
        private System.Windows.Forms.Label serialNumber3;
        private System.Windows.Forms.Label serialNumber4;

    }
}

