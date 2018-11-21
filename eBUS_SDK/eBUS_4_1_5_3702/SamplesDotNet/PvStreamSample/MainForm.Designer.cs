namespace PvStreamSample
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
            this.oneLabel = new System.Windows.Forms.Label();
            this.twoLabel = new System.Windows.Forms.Label();
            this.threeLabel = new System.Windows.Forms.Label();
            this.fourLabel = new System.Windows.Forms.Label();
            this.fiveLabel = new System.Windows.Forms.Label();
            this.sixLabel = new System.Windows.Forms.Label();
            this.stopButton = new System.Windows.Forms.Button();
            this.displayControl = new PvGUIDotNet.PvDisplayControl();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.browser = new PvGUIDotNet.PvGenBrowserControl();
            this.SuspendLayout();
            // 
            // oneLabel
            // 
            this.oneLabel.AutoSize = true;
            this.oneLabel.Enabled = false;
            this.oneLabel.Location = new System.Drawing.Point(12, 9);
            this.oneLabel.Name = "oneLabel";
            this.oneLabel.Size = new System.Drawing.Size(73, 13);
            this.oneLabel.TabIndex = 0;
            this.oneLabel.Text = "1. Connecting";
            // 
            // twoLabel
            // 
            this.twoLabel.AutoSize = true;
            this.twoLabel.Enabled = false;
            this.twoLabel.Location = new System.Drawing.Point(12, 29);
            this.twoLabel.Name = "twoLabel";
            this.twoLabel.Size = new System.Drawing.Size(107, 13);
            this.twoLabel.TabIndex = 1;
            this.twoLabel.Text = "2. Configuring device";
            // 
            // threeLabel
            // 
            this.threeLabel.AutoSize = true;
            this.threeLabel.Enabled = false;
            this.threeLabel.Location = new System.Drawing.Point(12, 49);
            this.threeLabel.Name = "threeLabel";
            this.threeLabel.Size = new System.Drawing.Size(89, 13);
            this.threeLabel.TabIndex = 2;
            this.threeLabel.Text = "3. Starting stream";
            // 
            // fourLabel
            // 
            this.fourLabel.AutoSize = true;
            this.fourLabel.Enabled = false;
            this.fourLabel.Location = new System.Drawing.Point(12, 69);
            this.fourLabel.Name = "fourLabel";
            this.fourLabel.Size = new System.Drawing.Size(66, 13);
            this.fourLabel.TabIndex = 3;
            this.fourLabel.Text = "4. Streaming";
            // 
            // fiveLabel
            // 
            this.fiveLabel.AutoSize = true;
            this.fiveLabel.Enabled = false;
            this.fiveLabel.Location = new System.Drawing.Point(12, 113);
            this.fiveLabel.Name = "fiveLabel";
            this.fiveLabel.Size = new System.Drawing.Size(95, 13);
            this.fiveLabel.TabIndex = 4;
            this.fiveLabel.Text = "5. Stopping stream";
            // 
            // sixLabel
            // 
            this.sixLabel.AutoSize = true;
            this.sixLabel.Enabled = false;
            this.sixLabel.Location = new System.Drawing.Point(12, 133);
            this.sixLabel.Name = "sixLabel";
            this.sixLabel.Size = new System.Drawing.Size(87, 13);
            this.sixLabel.TabIndex = 5;
            this.sixLabel.Text = "6. Disconnecting";
            // 
            // stopButton
            // 
            this.stopButton.Enabled = false;
            this.stopButton.Location = new System.Drawing.Point(26, 85);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(75, 23);
            this.stopButton.TabIndex = 8;
            this.stopButton.Text = "Stop";
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // displayControl
            // 
            this.displayControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.displayControl.BackColor = System.Drawing.Color.Transparent;
            this.displayControl.BackgroundColor = System.Drawing.Color.DarkGray;
            this.displayControl.Location = new System.Drawing.Point(139, 9);
            this.displayControl.Name = "displayControl";
            this.displayControl.Size = new System.Drawing.Size(461, 411);
            this.displayControl.TabIndex = 7;
            // 
            // timer
            // 
            this.timer.Interval = 1000;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // browser
            // 
            this.browser.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.browser.GenParameterArray = null;
            this.browser.Location = new System.Drawing.Point(606, 12);
            this.browser.Name = "browser";
            this.browser.Size = new System.Drawing.Size(299, 408);
            this.browser.TabIndex = 18;
            this.browser.Visibility = PvDotNet.PvGenVisibility.Beginner;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(917, 432);
            this.Controls.Add(this.browser);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.displayControl);
            this.Controls.Add(this.sixLabel);
            this.Controls.Add(this.fiveLabel);
            this.Controls.Add(this.fourLabel);
            this.Controls.Add(this.threeLabel);
            this.Controls.Add(this.twoLabel);
            this.Controls.Add(this.oneLabel);
            this.Name = "MainForm";
            this.Text = "PvStreamSample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label oneLabel;
        private System.Windows.Forms.Label twoLabel;
        private System.Windows.Forms.Label threeLabel;
        private System.Windows.Forms.Label fourLabel;
        private System.Windows.Forms.Label fiveLabel;
        private System.Windows.Forms.Label sixLabel;
        private PvGUIDotNet.PvDisplayControl displayControl;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Timer timer;
        internal PvGUIDotNet.PvGenBrowserControl browser;
    }
}

