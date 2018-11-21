namespace DirectShowDisplay
{
    partial class RoleForm
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
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.controllerRadioButton = new System.Windows.Forms.RadioButton();
            this.unicastReceiverRadioButton = new System.Windows.Forms.RadioButton();
            this.multicastReceiverRadioButton = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.localPortSpinEdit = new System.Windows.Forms.NumericUpDown();
            this.multicastPortSpinEdit = new System.Windows.Forms.NumericUpDown();
            this.multicastIPAddressEdit = new PvGUIDotNet.PvIPAddressEdit();
            ((System.ComponentModel.ISupportInitialize)(this.localPortSpinEdit)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.multicastPortSpinEdit)).BeginInit();
            this.SuspendLayout();
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(218, 214);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 9;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(299, 214);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 10;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // controllerRadioButton
            // 
            this.controllerRadioButton.AutoSize = true;
            this.controllerRadioButton.Checked = true;
            this.controllerRadioButton.Location = new System.Drawing.Point(12, 12);
            this.controllerRadioButton.Name = "controllerRadioButton";
            this.controllerRadioButton.Size = new System.Drawing.Size(285, 17);
            this.controllerRadioButton.TabIndex = 0;
            this.controllerRadioButton.TabStop = true;
            this.controllerRadioButton.Text = "GigE Vision or USB3 Vision controller and data receiver";
            this.controllerRadioButton.UseVisualStyleBackColor = true;
            // 
            // unicastReceiverRadioButton
            // 
            this.unicastReceiverRadioButton.AutoSize = true;
            this.unicastReceiverRadioButton.Location = new System.Drawing.Point(12, 35);
            this.unicastReceiverRadioButton.Name = "unicastReceiverRadioButton";
            this.unicastReceiverRadioButton.Size = new System.Drawing.Size(157, 17);
            this.unicastReceiverRadioButton.TabIndex = 1;
            this.unicastReceiverRadioButton.Text = "GigE Vision unicast receiver";
            this.unicastReceiverRadioButton.UseVisualStyleBackColor = true;
            // 
            // multicastReceiverRadioButton
            // 
            this.multicastReceiverRadioButton.AutoSize = true;
            this.multicastReceiverRadioButton.Location = new System.Drawing.Point(12, 96);
            this.multicastReceiverRadioButton.Name = "multicastReceiverRadioButton";
            this.multicastReceiverRadioButton.Size = new System.Drawing.Size(164, 17);
            this.multicastReceiverRadioButton.TabIndex = 4;
            this.multicastReceiverRadioButton.Text = "GigE Vision multicast receiver";
            this.multicastReceiverRadioButton.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(48, 65);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Local port";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(47, 126);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "IP address";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(48, 150);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(26, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Port";
            // 
            // localPortSpinEdit
            // 
            this.localPortSpinEdit.Location = new System.Drawing.Point(129, 63);
            this.localPortSpinEdit.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.localPortSpinEdit.Name = "localPortSpinEdit";
            this.localPortSpinEdit.Size = new System.Drawing.Size(83, 20);
            this.localPortSpinEdit.TabIndex = 3;
            // 
            // multicastPortSpinEdit
            // 
            this.multicastPortSpinEdit.Location = new System.Drawing.Point(129, 148);
            this.multicastPortSpinEdit.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.multicastPortSpinEdit.Name = "multicastPortSpinEdit";
            this.multicastPortSpinEdit.Size = new System.Drawing.Size(83, 20);
            this.multicastPortSpinEdit.TabIndex = 8;
            this.multicastPortSpinEdit.Value = new decimal(new int[] {
            1042,
            0,
            0,
            0});
            // 
            // multicastIPAddressEdit
            // 
            this.multicastIPAddressEdit.AddressText = "239.192.1.1";
            this.multicastIPAddressEdit.AddressUInt32 = ((uint)(4022337793u));
            this.multicastIPAddressEdit.BackColor = System.Drawing.SystemColors.Window;
            this.multicastIPAddressEdit.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.multicastIPAddressEdit.Location = new System.Drawing.Point(129, 122);
            this.multicastIPAddressEdit.Name = "multicastIPAddressEdit";
            this.multicastIPAddressEdit.Size = new System.Drawing.Size(158, 20);
            this.multicastIPAddressEdit.TabIndex = 6;
            // 
            // RoleForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(386, 249);
            this.Controls.Add(this.multicastIPAddressEdit);
            this.Controls.Add(this.multicastPortSpinEdit);
            this.Controls.Add(this.localPortSpinEdit);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.multicastReceiverRadioButton);
            this.Controls.Add(this.unicastReceiverRadioButton);
            this.Controls.Add(this.controllerRadioButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RoleForm";
            this.ShowIcon = false;
            this.Text = "Source Filter Role";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.RoleForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.localPortSpinEdit)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.multicastPortSpinEdit)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.RadioButton controllerRadioButton;
        private System.Windows.Forms.RadioButton unicastReceiverRadioButton;
        private System.Windows.Forms.RadioButton multicastReceiverRadioButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown localPortSpinEdit;
        private System.Windows.Forms.NumericUpDown multicastPortSpinEdit;
        private PvGUIDotNet.PvIPAddressEdit multicastIPAddressEdit;
    }
}