namespace MulticastSlave
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
            this.pvDisplayControl1 = new PvGUIDotNet.PvDisplayControl();
            this.statusControl = new PvGUIDotNet.PvStatusControl();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.parametersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.streamToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pvDisplayControl1
            // 
            this.pvDisplayControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pvDisplayControl1.BackgroundColor = System.Drawing.Color.DarkGray;
            this.pvDisplayControl1.Location = new System.Drawing.Point(0, 24);
            this.pvDisplayControl1.Name = "pvDisplayControl1";
            this.pvDisplayControl1.Size = new System.Drawing.Size(553, 349);
            this.pvDisplayControl1.TabIndex = 0;
            // 
            // statusControl
            // 
            this.statusControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusControl.BuffersReallocated = false;
            this.statusControl.DisplayThread = null;
            this.statusControl.Location = new System.Drawing.Point(0, 379);
            this.statusControl.MinimumSize = new System.Drawing.Size(400, 32);
            this.statusControl.Name = "statusControl";
            this.statusControl.Size = new System.Drawing.Size(553, 44);
            this.statusControl.Stream = null;
            this.statusControl.TabIndex = 26;
            this.statusControl.Warning = "";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.parametersToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(553, 24);
            this.menuStrip1.TabIndex = 27;
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
            // parametersToolStripMenuItem
            // 
            this.parametersToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.streamToolStripMenuItem});
            this.parametersToolStripMenuItem.Name = "parametersToolStripMenuItem";
            this.parametersToolStripMenuItem.Size = new System.Drawing.Size(78, 20);
            this.parametersToolStripMenuItem.Text = "Parameters";
            // 
            // streamToolStripMenuItem
            // 
            this.streamToolStripMenuItem.Name = "streamToolStripMenuItem";
            this.streamToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.streamToolStripMenuItem.Text = "Stream";
            this.streamToolStripMenuItem.Click += new System.EventHandler(this.streamToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(553, 423);
            this.Controls.Add(this.statusControl);
            this.Controls.Add(this.pvDisplayControl1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "MulticastSlave";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private PvGUIDotNet.PvDisplayControl pvDisplayControl1;
        private PvGUIDotNet.PvStatusControl statusControl;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem parametersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem streamToolStripMenuItem;
    }
}

