namespace PvMulticastSlaveSample
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
            this.GenBrowserControl = new PvGUIDotNet.PvGenBrowserControl();
            this.SuspendLayout();
            // 
            // GenBrowserControl
            // 
            this.GenBrowserControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.GenBrowserControl.GenParameterArray = null;
            this.GenBrowserControl.Location = new System.Drawing.Point(0, 0);
            this.GenBrowserControl.Name = "GenBrowserControl";
            this.GenBrowserControl.Size = new System.Drawing.Size(370, 546);
            this.GenBrowserControl.TabIndex = 0;
            this.GenBrowserControl.Visibility = PvDotNet.PvGenVisibility.Beginner;
            // 
            // BrowserForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(370, 546);
            this.Controls.Add(this.GenBrowserControl);
            this.Name = "BrowserForm";
            this.Text = "BrowserForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BrowserForm_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        public PvGUIDotNet.PvGenBrowserControl GenBrowserControl;
    }
}