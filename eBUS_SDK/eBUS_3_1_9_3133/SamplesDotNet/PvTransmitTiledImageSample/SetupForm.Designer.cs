namespace PvTransmitTiledImageSample
{
    partial class SetupForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SetupForm));
            this.roleGroupBox = new System.Windows.Forms.GroupBox();
            this.dataReceiverRadioButton = new System.Windows.Forms.RadioButton();
            this.controlDataReceiverRadioButton = new System.Windows.Forms.RadioButton();
            this.streamDestinationGroupBox = new System.Windows.Forms.GroupBox();
            this.multicastPortTextBox = new System.Windows.Forms.TextBox();
            this.multicastPortLabel = new System.Windows.Forms.Label();
            this.multicastIPAddressTextBox = new System.Windows.Forms.TextBox();
            this.ipAddressLabel = new System.Windows.Forms.Label();
            this.multicastRadioButton = new System.Windows.Forms.RadioButton();
            this.unicastSpecificPortTextBox = new System.Windows.Forms.TextBox();
            this.portLabel = new System.Windows.Forms.Label();
            this.unicastSpecificRadioButton = new System.Windows.Forms.RadioButton();
            this.unicastAutomaticRadioButton = new System.Windows.Forms.RadioButton();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.roleGroupBox.SuspendLayout();
            this.streamDestinationGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // roleGroupBox
            // 
            this.roleGroupBox.Controls.Add(this.dataReceiverRadioButton);
            this.roleGroupBox.Controls.Add(this.controlDataReceiverRadioButton);
            this.roleGroupBox.Location = new System.Drawing.Point(13, 13);
            this.roleGroupBox.Name = "roleGroupBox";
            this.roleGroupBox.Size = new System.Drawing.Size(180, 177);
            this.roleGroupBox.TabIndex = 0;
            this.roleGroupBox.TabStop = false;
            this.roleGroupBox.Text = "Role";
            // 
            // dataReceiverRadioButton
            // 
            this.dataReceiverRadioButton.AutoSize = true;
            this.dataReceiverRadioButton.Location = new System.Drawing.Point(19, 42);
            this.dataReceiverRadioButton.Name = "dataReceiverRadioButton";
            this.dataReceiverRadioButton.Size = new System.Drawing.Size(89, 17);
            this.dataReceiverRadioButton.TabIndex = 1;
            this.dataReceiverRadioButton.Text = "Data receiver";
            this.dataReceiverRadioButton.UseVisualStyleBackColor = true;
            // 
            // controlDataReceiverRadioButton
            // 
            this.controlDataReceiverRadioButton.AutoSize = true;
            this.controlDataReceiverRadioButton.Checked = true;
            this.controlDataReceiverRadioButton.Location = new System.Drawing.Point(19, 19);
            this.controlDataReceiverRadioButton.Name = "controlDataReceiverRadioButton";
            this.controlDataReceiverRadioButton.Size = new System.Drawing.Size(155, 17);
            this.controlDataReceiverRadioButton.TabIndex = 0;
            this.controlDataReceiverRadioButton.TabStop = true;
            this.controlDataReceiverRadioButton.Text = "Controller and data receiver";
            this.controlDataReceiverRadioButton.UseVisualStyleBackColor = true;
            // 
            // streamDestinationGroupBox
            // 
            this.streamDestinationGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.streamDestinationGroupBox.Controls.Add(this.multicastPortTextBox);
            this.streamDestinationGroupBox.Controls.Add(this.multicastPortLabel);
            this.streamDestinationGroupBox.Controls.Add(this.multicastIPAddressTextBox);
            this.streamDestinationGroupBox.Controls.Add(this.ipAddressLabel);
            this.streamDestinationGroupBox.Controls.Add(this.multicastRadioButton);
            this.streamDestinationGroupBox.Controls.Add(this.unicastSpecificPortTextBox);
            this.streamDestinationGroupBox.Controls.Add(this.portLabel);
            this.streamDestinationGroupBox.Controls.Add(this.unicastSpecificRadioButton);
            this.streamDestinationGroupBox.Controls.Add(this.unicastAutomaticRadioButton);
            this.streamDestinationGroupBox.Location = new System.Drawing.Point(199, 13);
            this.streamDestinationGroupBox.Name = "streamDestinationGroupBox";
            this.streamDestinationGroupBox.Size = new System.Drawing.Size(274, 177);
            this.streamDestinationGroupBox.TabIndex = 1;
            this.streamDestinationGroupBox.TabStop = false;
            this.streamDestinationGroupBox.Text = "Stream destination";
            // 
            // multicastPortTextBox
            // 
            this.multicastPortTextBox.Enabled = false;
            this.multicastPortTextBox.Location = new System.Drawing.Point(115, 145);
            this.multicastPortTextBox.Name = "multicastPortTextBox";
            this.multicastPortTextBox.Size = new System.Drawing.Size(77, 20);
            this.multicastPortTextBox.TabIndex = 8;
            // 
            // multicastPortLabel
            // 
            this.multicastPortLabel.AutoSize = true;
            this.multicastPortLabel.Location = new System.Drawing.Point(49, 148);
            this.multicastPortLabel.Name = "multicastPortLabel";
            this.multicastPortLabel.Size = new System.Drawing.Size(26, 13);
            this.multicastPortLabel.TabIndex = 7;
            this.multicastPortLabel.Text = "Port";
            // 
            // multicastIPAddressTextBox
            // 
            this.multicastIPAddressTextBox.Enabled = false;
            this.multicastIPAddressTextBox.Location = new System.Drawing.Point(115, 119);
            this.multicastIPAddressTextBox.Name = "multicastIPAddressTextBox";
            this.multicastIPAddressTextBox.Size = new System.Drawing.Size(104, 20);
            this.multicastIPAddressTextBox.TabIndex = 6;
            this.multicastIPAddressTextBox.Text = " 0 . 0 . 0 . 0 ";
            // 
            // ipAddressLabel
            // 
            this.ipAddressLabel.AutoSize = true;
            this.ipAddressLabel.Location = new System.Drawing.Point(52, 122);
            this.ipAddressLabel.Name = "ipAddressLabel";
            this.ipAddressLabel.Size = new System.Drawing.Size(57, 13);
            this.ipAddressLabel.TabIndex = 5;
            this.ipAddressLabel.Text = "IP address";
            // 
            // multicastRadioButton
            // 
            this.multicastRadioButton.AutoSize = true;
            this.multicastRadioButton.Location = new System.Drawing.Point(21, 98);
            this.multicastRadioButton.Name = "multicastRadioButton";
            this.multicastRadioButton.Size = new System.Drawing.Size(67, 17);
            this.multicastRadioButton.TabIndex = 4;
            this.multicastRadioButton.Text = "Multicast";
            this.multicastRadioButton.UseVisualStyleBackColor = true;
            this.multicastRadioButton.CheckedChanged += new System.EventHandler(this.multicastRadioButton_CheckedChanged);
            // 
            // unicastSpecificPortTextBox
            // 
            this.unicastSpecificPortTextBox.Enabled = false;
            this.unicastSpecificPortTextBox.Location = new System.Drawing.Point(115, 68);
            this.unicastSpecificPortTextBox.Name = "unicastSpecificPortTextBox";
            this.unicastSpecificPortTextBox.Size = new System.Drawing.Size(77, 20);
            this.unicastSpecificPortTextBox.TabIndex = 3;
            // 
            // portLabel
            // 
            this.portLabel.AutoSize = true;
            this.portLabel.Location = new System.Drawing.Point(49, 68);
            this.portLabel.Name = "portLabel";
            this.portLabel.Size = new System.Drawing.Size(26, 13);
            this.portLabel.TabIndex = 2;
            this.portLabel.Text = "Port";
            // 
            // unicastSpecificRadioButton
            // 
            this.unicastSpecificRadioButton.AutoSize = true;
            this.unicastSpecificRadioButton.Location = new System.Drawing.Point(21, 42);
            this.unicastSpecificRadioButton.Name = "unicastSpecificRadioButton";
            this.unicastSpecificRadioButton.Size = new System.Drawing.Size(149, 17);
            this.unicastSpecificRadioButton.TabIndex = 1;
            this.unicastSpecificRadioButton.Text = "Unicast, specific local port";
            this.unicastSpecificRadioButton.UseVisualStyleBackColor = true;
            this.unicastSpecificRadioButton.CheckedChanged += new System.EventHandler(this.unicastSpecificRadioButton_CheckedChanged);
            // 
            // unicastAutomaticRadioButton
            // 
            this.unicastAutomaticRadioButton.AutoSize = true;
            this.unicastAutomaticRadioButton.Checked = true;
            this.unicastAutomaticRadioButton.Location = new System.Drawing.Point(21, 19);
            this.unicastAutomaticRadioButton.Name = "unicastAutomaticRadioButton";
            this.unicastAutomaticRadioButton.Size = new System.Drawing.Size(113, 17);
            this.unicastAutomaticRadioButton.TabIndex = 0;
            this.unicastAutomaticRadioButton.TabStop = true;
            this.unicastAutomaticRadioButton.Text = "Unicast, automatic";
            this.unicastAutomaticRadioButton.UseVisualStyleBackColor = true;
            this.unicastAutomaticRadioButton.CheckedChanged += new System.EventHandler(this.unicastAutomaticRadioButton_CheckedChanged);
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.Location = new System.Drawing.Point(315, 227);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 26);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.Location = new System.Drawing.Point(398, 227);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 26);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // SetupForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(485, 265);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.streamDestinationGroupBox);
            this.Controls.Add(this.roleGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "SetupForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Setup";
            this.Load += new System.EventHandler(this.SetupForm_Load);
            this.roleGroupBox.ResumeLayout(false);
            this.roleGroupBox.PerformLayout();
            this.streamDestinationGroupBox.ResumeLayout(false);
            this.streamDestinationGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

#endregion

        private System.Windows.Forms.GroupBox roleGroupBox;
        private System.Windows.Forms.RadioButton dataReceiverRadioButton;
        private System.Windows.Forms.RadioButton controlDataReceiverRadioButton;
        private System.Windows.Forms.GroupBox streamDestinationGroupBox;
        private System.Windows.Forms.TextBox multicastPortTextBox;
        private System.Windows.Forms.Label multicastPortLabel;
        private System.Windows.Forms.TextBox multicastIPAddressTextBox;
        private System.Windows.Forms.Label ipAddressLabel;
        private System.Windows.Forms.RadioButton multicastRadioButton;
        private System.Windows.Forms.TextBox unicastSpecificPortTextBox;
        private System.Windows.Forms.Label portLabel;
        private System.Windows.Forms.RadioButton unicastSpecificRadioButton;
        private System.Windows.Forms.RadioButton unicastAutomaticRadioButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
    }
}