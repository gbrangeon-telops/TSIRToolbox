namespace TransmitTiledImages
{
    partial class ProgressForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProgressForm));
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.pictureBox = new System.Windows.Forms.PictureBox();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.statusLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // pictureBox
            // 
            this.pictureBox.Location = new System.Drawing.Point(13, 13);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(32, 38);
            this.pictureBox.TabIndex = 2;
            this.pictureBox.TabStop = false;
            this.pictureBox.UseWaitCursor = true;
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.White;
            this.imageList.Images.SetKeyName(0, "wheel0.bmp");
            this.imageList.Images.SetKeyName(1, "wheel1.bmp");
            this.imageList.Images.SetKeyName(2, "wheel2.bmp");
            this.imageList.Images.SetKeyName(3, "wheel3.bmp");
            this.imageList.Images.SetKeyName(4, "wheel4.bmp");
            this.imageList.Images.SetKeyName(5, "wheel5.bmp");
            this.imageList.Images.SetKeyName(6, "wheel6.bmp");
            this.imageList.Images.SetKeyName(7, "wheel7.bmp");
            // 
            // statusLabel
            // 
            this.statusLabel.AutoSize = true;
            this.statusLabel.Location = new System.Drawing.Point(60, 13);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(170, 13);
            this.statusLabel.TabIndex = 3;
            this.statusLabel.Text = "Status updates are displayed here.";
            this.statusLabel.UseWaitCursor = true;
            // 
            // ProgressForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(323, 90);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.pictureBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProgressForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Connection Progress";
            this.UseWaitCursor = true;
            this.Load += new System.EventHandler(this.ProgressForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

#endregion

        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.PictureBox pictureBox;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Label statusLabel;
    }
}