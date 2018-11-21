namespace PvSimpleUISample
{
    partial class BrowserForm
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
            this.Browser = new PvGUIDotNet.PvGenBrowserControl();
            this.SuspendLayout();
            // 
            // Browser
            // 
            this.Browser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Browser.GenParameterArray = null;
            this.Browser.Location = new System.Drawing.Point(0, 0);
            this.Browser.Name = "Browser";
            this.Browser.Size = new System.Drawing.Size(374, 512);
            this.Browser.TabIndex = 0;
            this.Browser.Visibility = PvDotNet.PvGenVisibility.Beginner;
            // 
            // BrowserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(374, 512);
            this.Controls.Add(this.Browser);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.ShowInTaskbar = false;
            this.Name = "BrowserForm";
            this.ShowIcon = false;
            this.Text = "GenICam Browser";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BrowserForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        public PvGUIDotNet.PvGenBrowserControl Browser;

    }
}