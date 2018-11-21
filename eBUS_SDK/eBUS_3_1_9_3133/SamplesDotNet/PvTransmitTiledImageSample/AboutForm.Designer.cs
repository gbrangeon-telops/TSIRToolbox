namespace PvTransmitTiledImageSample
{
    partial class AboutForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutForm));
            this.ProgramNameLabel = new System.Windows.Forms.Label();
            this.CompanyNameLabel = new System.Windows.Forms.Label();
            this.VersionLabel = new System.Windows.Forms.Label();
            this.CopyrightLabel = new System.Windows.Forms.Label();
            this.OKButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ProgramNameLabel
            // 
            this.ProgramNameLabel.AutoSize = true;
            this.ProgramNameLabel.Location = new System.Drawing.Point(41, 12);
            this.ProgramNameLabel.Name = "ProgramNameLabel";
            this.ProgramNameLabel.Size = new System.Drawing.Size(111, 13);
            this.ProgramNameLabel.TabIndex = 2;
            this.ProgramNameLabel.Text = "PvTransmitTilesImage";
            // 
            // CompanyNameLabel
            // 
            this.CompanyNameLabel.AutoSize = true;
            this.CompanyNameLabel.Location = new System.Drawing.Point(41, 36);
            this.CompanyNameLabel.Name = "CompanyNameLabel";
            this.CompanyNameLabel.Size = new System.Drawing.Size(91, 13);
            this.CompanyNameLabel.TabIndex = 3;
            this.CompanyNameLabel.Text = "<company name>";
            // 
            // VersionLabel
            // 
            this.VersionLabel.AutoSize = true;
            this.VersionLabel.Location = new System.Drawing.Point(41, 56);
            this.VersionLabel.Name = "VersionLabel";
            this.VersionLabel.Size = new System.Drawing.Size(53, 13);
            this.VersionLabel.TabIndex = 4;
            this.VersionLabel.Text = "<version>";
            // 
            // CopyrightLabel
            // 
            this.CopyrightLabel.AutoSize = true;
            this.CopyrightLabel.Location = new System.Drawing.Point(41, 76);
            this.CopyrightLabel.Name = "CopyrightLabel";
            this.CopyrightLabel.Size = new System.Drawing.Size(62, 13);
            this.CopyrightLabel.TabIndex = 5;
            this.CopyrightLabel.Text = "<copyright>";
            // 
            // OKButton
            // 
            this.OKButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.OKButton.Location = new System.Drawing.Point(134, 107);
            this.OKButton.Name = "OKButton";
            this.OKButton.Size = new System.Drawing.Size(75, 23);
            this.OKButton.TabIndex = 6;
            this.OKButton.Text = "OK";
            this.OKButton.UseVisualStyleBackColor = true;
            // 
            // AboutForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.OKButton;
            this.ClientSize = new System.Drawing.Size(221, 142);
            this.Controls.Add(this.OKButton);
            this.Controls.Add(this.CopyrightLabel);
            this.Controls.Add(this.VersionLabel);
            this.Controls.Add(this.CompanyNameLabel);
            this.Controls.Add(this.ProgramNameLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "About";
            this.Load += new System.EventHandler(this.AboutForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ProgramNameLabel;
        private System.Windows.Forms.Label CompanyNameLabel;
        private System.Windows.Forms.Label VersionLabel;
        private System.Windows.Forms.Label CopyrightLabel;
        private System.Windows.Forms.Button OKButton;

    }
}