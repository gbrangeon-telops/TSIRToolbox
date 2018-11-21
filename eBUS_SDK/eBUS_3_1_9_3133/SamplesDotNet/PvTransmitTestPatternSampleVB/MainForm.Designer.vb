<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class MainForm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MainForm))
        Me.menuStrip = New System.Windows.Forms.MenuStrip()
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ExitToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.sourceAddressComboBox = New System.Windows.Forms.ComboBox()
        Me.poolSizeTextBox = New System.Windows.Forms.TextBox()
        Me.resetOptionsButton = New System.Windows.Forms.Button()
        Me.optionsGroupBox = New System.Windows.Forms.GroupBox()
        Me.widthLabel = New System.Windows.Forms.Label()
        Me.widthTextBox = New System.Windows.Forms.TextBox()
        Me.heightLabel = New System.Windows.Forms.Label()
        Me.heightTextBox = New System.Windows.Forms.TextBox()
        Me.fpsLabel = New System.Windows.Forms.Label()
        Me.fpsTextBox = New System.Windows.Forms.TextBox()
        Me.pixelTypeLabel = New System.Windows.Forms.Label()
        Me.poolSizeLabel = New System.Windows.Forms.Label()
        Me.packetSizeTextBox = New System.Windows.Forms.TextBox()
        Me.sourceAddressLabel = New System.Windows.Forms.Label()
        Me.packetSizeLabel = New System.Windows.Forms.Label()
        Me.destinationAddressLabel = New System.Windows.Forms.Label()
        Me.sourcePortLabel = New System.Windows.Forms.Label()
        Me.sourcePortTextBox = New System.Windows.Forms.TextBox()
        Me.destinationPortTextBox = New System.Windows.Forms.TextBox()
        Me.destinationPortLabel = New System.Windows.Forms.Label()
        Me.transmitStatusLabel = New System.Windows.Forms.Label()
        Me.stopButton = New System.Windows.Forms.Button()
        Me.playButton = New System.Windows.Forms.Button()
        Me.timer = New System.Windows.Forms.Timer(Me.components)
        Me.toolTip = New System.Windows.Forms.ToolTip(Me.components)
        Me.displayControl = New PvGUIDotNet.PvDisplayControl()
        Me.pixelTypeTexBox = New System.Windows.Forms.TextBox()
        Me.destinationAddressTextBox = New System.Windows.Forms.TextBox()
        Me.menuStrip.SuspendLayout()
        Me.optionsGroupBox.SuspendLayout()
        Me.SuspendLayout()
        '
        'menuStrip
        '
        Me.menuStrip.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem})
        Me.menuStrip.Location = New System.Drawing.Point(0, 0)
        Me.menuStrip.Name = "menuStrip"
        Me.menuStrip.Size = New System.Drawing.Size(783, 24)
        Me.menuStrip.TabIndex = 41
        Me.menuStrip.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ExitToolStripMenuItem})
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(37, 20)
        Me.FileToolStripMenuItem.Text = "File"
        '
        'ExitToolStripMenuItem
        '
        Me.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem"
        Me.ExitToolStripMenuItem.Size = New System.Drawing.Size(92, 22)
        Me.ExitToolStripMenuItem.Text = "Exit"
        '
        'sourceAddressComboBox
        '
        Me.sourceAddressComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.sourceAddressComboBox.FormattingEnabled = True
        Me.sourceAddressComboBox.Location = New System.Drawing.Point(112, 145)
        Me.sourceAddressComboBox.Name = "sourceAddressComboBox"
        Me.sourceAddressComboBox.Size = New System.Drawing.Size(124, 21)
        Me.sourceAddressComboBox.TabIndex = 34
        Me.toolTip.SetToolTip(Me.sourceAddressComboBox, "Source address in the form 123.456.789.101 - default - first valid address encoun" & _
        "tered while enumerating interfaces.")
        '
        'poolSizeTextBox
        '
        Me.poolSizeTextBox.Location = New System.Drawing.Point(112, 120)
        Me.poolSizeTextBox.Name = "poolSizeTextBox"
        Me.poolSizeTextBox.Size = New System.Drawing.Size(124, 20)
        Me.poolSizeTextBox.TabIndex = 8
        Me.poolSizeTextBox.Text = "4"
        Me.toolTip.SetToolTip(Me.poolSizeTextBox, "Number of buffers to pre-allocate - default 4. Increase this number when sending " & _
        "smaller images at high frame rates.")
        '
        'resetOptionsButton
        '
        Me.resetOptionsButton.Location = New System.Drawing.Point(143, 305)
        Me.resetOptionsButton.Name = "resetOptionsButton"
        Me.resetOptionsButton.Size = New System.Drawing.Size(93, 23)
        Me.resetOptionsButton.TabIndex = 32
        Me.resetOptionsButton.Text = "Reset Options"
        Me.resetOptionsButton.UseVisualStyleBackColor = True
        '
        'optionsGroupBox
        '
        Me.optionsGroupBox.Controls.Add(Me.destinationAddressTextBox)
        Me.optionsGroupBox.Controls.Add(Me.pixelTypeTexBox)
        Me.optionsGroupBox.Controls.Add(Me.sourceAddressComboBox)
        Me.optionsGroupBox.Controls.Add(Me.poolSizeTextBox)
        Me.optionsGroupBox.Controls.Add(Me.resetOptionsButton)
        Me.optionsGroupBox.Controls.Add(Me.widthLabel)
        Me.optionsGroupBox.Controls.Add(Me.widthTextBox)
        Me.optionsGroupBox.Controls.Add(Me.heightLabel)
        Me.optionsGroupBox.Controls.Add(Me.heightTextBox)
        Me.optionsGroupBox.Controls.Add(Me.fpsLabel)
        Me.optionsGroupBox.Controls.Add(Me.fpsTextBox)
        Me.optionsGroupBox.Controls.Add(Me.pixelTypeLabel)
        Me.optionsGroupBox.Controls.Add(Me.poolSizeLabel)
        Me.optionsGroupBox.Controls.Add(Me.packetSizeTextBox)
        Me.optionsGroupBox.Controls.Add(Me.sourceAddressLabel)
        Me.optionsGroupBox.Controls.Add(Me.packetSizeLabel)
        Me.optionsGroupBox.Controls.Add(Me.destinationAddressLabel)
        Me.optionsGroupBox.Controls.Add(Me.sourcePortLabel)
        Me.optionsGroupBox.Controls.Add(Me.sourcePortTextBox)
        Me.optionsGroupBox.Controls.Add(Me.destinationPortTextBox)
        Me.optionsGroupBox.Controls.Add(Me.destinationPortLabel)
        Me.optionsGroupBox.Location = New System.Drawing.Point(12, 39)
        Me.optionsGroupBox.Name = "optionsGroupBox"
        Me.optionsGroupBox.Size = New System.Drawing.Size(251, 340)
        Me.optionsGroupBox.TabIndex = 45
        Me.optionsGroupBox.TabStop = False
        Me.optionsGroupBox.Text = "Options"
        '
        'widthLabel
        '
        Me.widthLabel.AutoSize = True
        Me.widthLabel.Location = New System.Drawing.Point(6, 16)
        Me.widthLabel.Name = "widthLabel"
        Me.widthLabel.Size = New System.Drawing.Size(35, 13)
        Me.widthLabel.TabIndex = 1
        Me.widthLabel.Text = "Width"
        Me.toolTip.SetToolTip(Me.widthLabel, "Width in pixels - default 640.")
        '
        'widthTextBox
        '
        Me.widthTextBox.Location = New System.Drawing.Point(112, 16)
        Me.widthTextBox.Name = "widthTextBox"
        Me.widthTextBox.Size = New System.Drawing.Size(124, 20)
        Me.widthTextBox.TabIndex = 2
        Me.widthTextBox.Text = "640"
        Me.toolTip.SetToolTip(Me.widthTextBox, "Width in pixels - default 640.")
        '
        'heightLabel
        '
        Me.heightLabel.AutoSize = True
        Me.heightLabel.Location = New System.Drawing.Point(6, 42)
        Me.heightLabel.Name = "heightLabel"
        Me.heightLabel.Size = New System.Drawing.Size(38, 13)
        Me.heightLabel.TabIndex = 3
        Me.heightLabel.Text = "Height"
        Me.toolTip.SetToolTip(Me.heightLabel, "Height in pixels - default 480.")
        '
        'heightTextBox
        '
        Me.heightTextBox.Location = New System.Drawing.Point(112, 42)
        Me.heightTextBox.Name = "heightTextBox"
        Me.heightTextBox.Size = New System.Drawing.Size(124, 20)
        Me.heightTextBox.TabIndex = 4
        Me.heightTextBox.Text = "480"
        Me.toolTip.SetToolTip(Me.heightTextBox, "Height in pixels - default 480.")
        '
        'fpsLabel
        '
        Me.fpsLabel.AutoSize = True
        Me.fpsLabel.Location = New System.Drawing.Point(6, 68)
        Me.fpsLabel.Name = "fpsLabel"
        Me.fpsLabel.Size = New System.Drawing.Size(27, 13)
        Me.fpsLabel.TabIndex = 5
        Me.fpsLabel.Text = "FPS"
        Me.toolTip.SetToolTip(Me.fpsLabel, "Desired frame rate - default UINT_MAX - no limitation on frame rate.")
        '
        'fpsTextBox
        '
        Me.fpsTextBox.Location = New System.Drawing.Point(112, 68)
        Me.fpsTextBox.Name = "fpsTextBox"
        Me.fpsTextBox.Size = New System.Drawing.Size(124, 20)
        Me.fpsTextBox.TabIndex = 6
        Me.fpsTextBox.Text = "0"
        Me.toolTip.SetToolTip(Me.fpsTextBox, "Desired frame rate - default UINT_MAX - no limitation on frame rate.")
        '
        'pixelTypeLabel
        '
        Me.pixelTypeLabel.AutoSize = True
        Me.pixelTypeLabel.Location = New System.Drawing.Point(5, 249)
        Me.pixelTypeLabel.Name = "pixelTypeLabel"
        Me.pixelTypeLabel.Size = New System.Drawing.Size(56, 13)
        Me.pixelTypeLabel.TabIndex = 23
        Me.pixelTypeLabel.Text = "Pixel Type"
        Me.toolTip.SetToolTip(Me.pixelTypeLabel, "GEV pixel type - default Mono8.")
        '
        'poolSizeLabel
        '
        Me.poolSizeLabel.AutoSize = True
        Me.poolSizeLabel.Location = New System.Drawing.Point(6, 120)
        Me.poolSizeLabel.Name = "poolSizeLabel"
        Me.poolSizeLabel.Size = New System.Drawing.Size(82, 13)
        Me.poolSizeLabel.TabIndex = 7
        Me.poolSizeLabel.Text = "Buffer Pool Size"
        Me.toolTip.SetToolTip(Me.poolSizeLabel, "Number of buffers to pre-allocate - default 4. Increase this number when sending " & _
        "smaller images at high frame rates.")
        '
        'packetSizeTextBox
        '
        Me.packetSizeTextBox.Location = New System.Drawing.Point(112, 94)
        Me.packetSizeTextBox.Name = "packetSizeTextBox"
        Me.packetSizeTextBox.Size = New System.Drawing.Size(124, 20)
        Me.packetSizeTextBox.TabIndex = 22
        Me.packetSizeTextBox.Text = "1440"
        Me.toolTip.SetToolTip(Me.packetSizeTextBox, "Maximimum size of streaming packets - default 1440. For best results, set ""Jumbo " & _
        "Frames"" property on your NIC and increase this value accordingly.")
        '
        'sourceAddressLabel
        '
        Me.sourceAddressLabel.AutoSize = True
        Me.sourceAddressLabel.Location = New System.Drawing.Point(6, 146)
        Me.sourceAddressLabel.Name = "sourceAddressLabel"
        Me.sourceAddressLabel.Size = New System.Drawing.Size(82, 13)
        Me.sourceAddressLabel.TabIndex = 9
        Me.sourceAddressLabel.Text = "Source Address"
        Me.toolTip.SetToolTip(Me.sourceAddressLabel, "Source address in the form 123.456.789.101 - default - first valid address encoun" & _
        "tered while enumerating interfaces.")
        '
        'packetSizeLabel
        '
        Me.packetSizeLabel.AutoSize = True
        Me.packetSizeLabel.Location = New System.Drawing.Point(6, 94)
        Me.packetSizeLabel.Name = "packetSizeLabel"
        Me.packetSizeLabel.Size = New System.Drawing.Size(64, 13)
        Me.packetSizeLabel.TabIndex = 21
        Me.packetSizeLabel.Text = "Packet Size"
        Me.toolTip.SetToolTip(Me.packetSizeLabel, "Maximimum size of streaming packets - default 1440. For best results, set ""Jumbo " & _
        "Frames"" property on your NIC and increase this value accordingly.")
        '
        'destinationAddressLabel
        '
        Me.destinationAddressLabel.AutoSize = True
        Me.destinationAddressLabel.Location = New System.Drawing.Point(6, 172)
        Me.destinationAddressLabel.Name = "destinationAddressLabel"
        Me.destinationAddressLabel.Size = New System.Drawing.Size(101, 13)
        Me.destinationAddressLabel.TabIndex = 11
        Me.destinationAddressLabel.Text = "Destination Address"
        Me.toolTip.SetToolTip(Me.destinationAddressLabel, "Destination address in the form 123.456.789.101 - default 239.192.1.1")
        '
        'sourcePortLabel
        '
        Me.sourcePortLabel.AutoSize = True
        Me.sourcePortLabel.Location = New System.Drawing.Point(6, 198)
        Me.sourcePortLabel.Name = "sourcePortLabel"
        Me.sourcePortLabel.Size = New System.Drawing.Size(63, 13)
        Me.sourcePortLabel.TabIndex = 13
        Me.sourcePortLabel.Text = "Source Port"
        Me.toolTip.SetToolTip(Me.sourcePortLabel, "source port - default 0. A port is automatically assigned when the socket is open" & _
        "ed.")
        '
        'sourcePortTextBox
        '
        Me.sourcePortTextBox.Location = New System.Drawing.Point(112, 198)
        Me.sourcePortTextBox.Name = "sourcePortTextBox"
        Me.sourcePortTextBox.Size = New System.Drawing.Size(124, 20)
        Me.sourcePortTextBox.TabIndex = 14
        Me.toolTip.SetToolTip(Me.sourcePortTextBox, "source port - default 0. A port is automatically assigned when the socket is open" & _
        "ed.")
        '
        'destinationPortTextBox
        '
        Me.destinationPortTextBox.Location = New System.Drawing.Point(112, 224)
        Me.destinationPortTextBox.Name = "destinationPortTextBox"
        Me.destinationPortTextBox.Size = New System.Drawing.Size(124, 20)
        Me.destinationPortTextBox.TabIndex = 16
        Me.destinationPortTextBox.Text = "1042"
        Me.toolTip.SetToolTip(Me.destinationPortTextBox, "destination port - default 1042.")
        '
        'destinationPortLabel
        '
        Me.destinationPortLabel.AutoSize = True
        Me.destinationPortLabel.Location = New System.Drawing.Point(6, 224)
        Me.destinationPortLabel.Name = "destinationPortLabel"
        Me.destinationPortLabel.Size = New System.Drawing.Size(82, 13)
        Me.destinationPortLabel.TabIndex = 15
        Me.destinationPortLabel.Text = "Destination Port"
        Me.toolTip.SetToolTip(Me.destinationPortLabel, "destination port - default 1042.")
        '
        'transmitStatusLabel
        '
        Me.transmitStatusLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.transmitStatusLabel.Location = New System.Drawing.Point(269, 334)
        Me.transmitStatusLabel.MaximumSize = New System.Drawing.Size(502, 40)
        Me.transmitStatusLabel.MinimumSize = New System.Drawing.Size(502, 40)
        Me.transmitStatusLabel.Name = "transmitStatusLabel"
        Me.transmitStatusLabel.Size = New System.Drawing.Size(502, 40)
        Me.transmitStatusLabel.TabIndex = 46
        '
        'stopButton
        '
        Me.stopButton.Enabled = False
        Me.stopButton.Image = CType(resources.GetObject("stopButton.Image"), System.Drawing.Image)
        Me.stopButton.Location = New System.Drawing.Point(688, 387)
        Me.stopButton.Name = "stopButton"
        Me.stopButton.Size = New System.Drawing.Size(82, 41)
        Me.stopButton.TabIndex = 44
        Me.stopButton.Text = "Stop"
        Me.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText
        Me.stopButton.UseVisualStyleBackColor = True
        '
        'playButton
        '
        Me.playButton.Image = CType(resources.GetObject("playButton.Image"), System.Drawing.Image)
        Me.playButton.Location = New System.Drawing.Point(600, 387)
        Me.playButton.Name = "playButton"
        Me.playButton.Size = New System.Drawing.Size(82, 41)
        Me.playButton.TabIndex = 43
        Me.playButton.Text = "Play"
        Me.playButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.playButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText
        Me.playButton.UseVisualStyleBackColor = True
        '
        'timer
        '
        '
        'displayControl
        '
        Me.displayControl.BackColor = System.Drawing.Color.Transparent
        Me.displayControl.BackgroundColor = System.Drawing.Color.DarkGray
        Me.displayControl.Location = New System.Drawing.Point(269, 39)
        Me.displayControl.Name = "displayControl"
        Me.displayControl.Size = New System.Drawing.Size(502, 292)
        Me.displayControl.TabIndex = 42
        '
        'pixelTypeTexBox
        '
        Me.pixelTypeTexBox.Location = New System.Drawing.Point(112, 250)
        Me.pixelTypeTexBox.Name = "pixelTypeTexBox"
        Me.pixelTypeTexBox.Size = New System.Drawing.Size(124, 20)
        Me.pixelTypeTexBox.TabIndex = 36
        Me.pixelTypeTexBox.Text = "Mono8"
        '
        'destinationAddressTextBox
        '
        Me.destinationAddressTextBox.Location = New System.Drawing.Point(113, 172)
        Me.destinationAddressTextBox.Name = "destinationAddressTextBox"
        Me.destinationAddressTextBox.Size = New System.Drawing.Size(124, 20)
        Me.destinationAddressTextBox.TabIndex = 37
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(783, 439)
        Me.Controls.Add(Me.menuStrip)
        Me.Controls.Add(Me.optionsGroupBox)
        Me.Controls.Add(Me.transmitStatusLabel)
        Me.Controls.Add(Me.stopButton)
        Me.Controls.Add(Me.playButton)
        Me.Controls.Add(Me.displayControl)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "MainForm"
        Me.Text = "PvTransmitTestPatternSampleVB"
        Me.menuStrip.ResumeLayout(False)
        Me.menuStrip.PerformLayout()
        Me.optionsGroupBox.ResumeLayout(False)
        Me.optionsGroupBox.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents menuStrip As System.Windows.Forms.MenuStrip
    Friend WithEvents FileToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ExitToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Private WithEvents sourceAddressComboBox As System.Windows.Forms.ComboBox
    Friend WithEvents toolTip As System.Windows.Forms.ToolTip
    Private WithEvents poolSizeTextBox As System.Windows.Forms.TextBox
    Private WithEvents resetOptionsButton As System.Windows.Forms.Button
    Private WithEvents optionsGroupBox As System.Windows.Forms.GroupBox
    Private WithEvents widthLabel As System.Windows.Forms.Label
    Private WithEvents widthTextBox As System.Windows.Forms.TextBox
    Private WithEvents heightLabel As System.Windows.Forms.Label
    Private WithEvents heightTextBox As System.Windows.Forms.TextBox
    Private WithEvents fpsLabel As System.Windows.Forms.Label
    Private WithEvents fpsTextBox As System.Windows.Forms.TextBox
    Private WithEvents pixelTypeLabel As System.Windows.Forms.Label
    Private WithEvents poolSizeLabel As System.Windows.Forms.Label
    Private WithEvents packetSizeTextBox As System.Windows.Forms.TextBox
    Private WithEvents sourceAddressLabel As System.Windows.Forms.Label
    Private WithEvents packetSizeLabel As System.Windows.Forms.Label
    Private WithEvents destinationAddressLabel As System.Windows.Forms.Label
    Private WithEvents sourcePortLabel As System.Windows.Forms.Label
    Private WithEvents sourcePortTextBox As System.Windows.Forms.TextBox
    Private WithEvents destinationPortTextBox As System.Windows.Forms.TextBox
    Private WithEvents destinationPortLabel As System.Windows.Forms.Label
    Private WithEvents transmitStatusLabel As System.Windows.Forms.Label
    Private WithEvents stopButton As System.Windows.Forms.Button
    Private WithEvents playButton As System.Windows.Forms.Button
    Friend WithEvents timer As System.Windows.Forms.Timer
    Private WithEvents displayControl As PvGUIDotNet.PvDisplayControl
    Private WithEvents pixelTypeTexBox As System.Windows.Forms.TextBox
    Private WithEvents destinationAddressTextBox As System.Windows.Forms.TextBox

End Class
