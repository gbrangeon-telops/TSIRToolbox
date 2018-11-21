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
        Me.groupBox4 = New System.Windows.Forms.GroupBox()
        Me.display = New PvGUIDotNet.PvDisplayControl()
        Me.statusControl = New PvGUIDotNet.PvStatusControl()
        Me.groupBox3 = New System.Windows.Forms.GroupBox()
        Me.label7 = New System.Windows.Forms.Label()
        Me.modeComboBox = New System.Windows.Forms.ComboBox()
        Me.playButton = New System.Windows.Forms.Button()
        Me.ImageList = New System.Windows.Forms.ImageList(Me.components)
        Me.stopButton = New System.Windows.Forms.Button()
        Me.groupBox2 = New System.Windows.Forms.GroupBox()
        Me.communicationButton = New System.Windows.Forms.Button()
        Me.deviceButton = New System.Windows.Forms.Button()
        Me.streamButton = New System.Windows.Forms.Button()
        Me.groupBox1 = New System.Windows.Forms.GroupBox()
        Me.nameTextBox = New System.Windows.Forms.TextBox()
        Me.label5 = New System.Windows.Forms.Label()
        Me.modelTextBox = New System.Windows.Forms.TextBox()
        Me.label4 = New System.Windows.Forms.Label()
        Me.manufacturerTextBox = New System.Windows.Forms.TextBox()
        Me.label3 = New System.Windows.Forms.Label()
        Me.macAddressTextBox = New System.Windows.Forms.TextBox()
        Me.label2 = New System.Windows.Forms.Label()
        Me.ipAddressTextBox = New System.Windows.Forms.TextBox()
        Me.label1 = New System.Windows.Forms.Label()
        Me.connectButton = New System.Windows.Forms.Button()
        Me.disconnectButton = New System.Windows.Forms.Button()
        Me.guidTextBox = New System.Windows.Forms.TextBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.groupBox4.SuspendLayout()
        Me.groupBox3.SuspendLayout()
        Me.groupBox2.SuspendLayout()
        Me.groupBox1.SuspendLayout()
        Me.SuspendLayout()
        '
        'groupBox4
        '
        Me.groupBox4.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.groupBox4.Controls.Add(Me.display)
        Me.groupBox4.Controls.Add(Me.statusControl)
        Me.groupBox4.Location = New System.Drawing.Point(290, 12)
        Me.groupBox4.Name = "groupBox4"
        Me.groupBox4.Size = New System.Drawing.Size(528, 449)
        Me.groupBox4.TabIndex = 27
        Me.groupBox4.TabStop = False
        Me.groupBox4.Text = "Display"
        '
        'display
        '
        Me.display.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.display.BackColor = System.Drawing.Color.Transparent
        Me.display.BackgroundColor = System.Drawing.Color.DarkGray
        Me.display.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.display.Location = New System.Drawing.Point(6, 19)
        Me.display.Name = "display"
        Me.display.Size = New System.Drawing.Size(516, 368)
        Me.display.TabIndex = 10
        Me.display.TextOverlay = ""
        Me.display.TextOverlayColor = System.Drawing.Color.Lime
        Me.display.TextOverlayOffsetX = 10
        Me.display.TextOverlayOffsetY = 10
        Me.display.TextOverlaySize = 18
        '
        'statusControl
        '
        Me.statusControl.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.statusControl.BuffersReallocated = False
        Me.statusControl.DisplayThread = Nothing
        Me.statusControl.Location = New System.Drawing.Point(6, 395)
        Me.statusControl.MinimumSize = New System.Drawing.Size(400, 32)
        Me.statusControl.Name = "statusControl"
        Me.statusControl.Size = New System.Drawing.Size(516, 44)
        Me.statusControl.Stream = Nothing
        Me.statusControl.TabIndex = 11
        Me.statusControl.Warning = ""
        '
        'groupBox3
        '
        Me.groupBox3.Controls.Add(Me.label7)
        Me.groupBox3.Controls.Add(Me.modeComboBox)
        Me.groupBox3.Controls.Add(Me.playButton)
        Me.groupBox3.Controls.Add(Me.stopButton)
        Me.groupBox3.Location = New System.Drawing.Point(12, 235)
        Me.groupBox3.Name = "groupBox3"
        Me.groupBox3.Size = New System.Drawing.Size(272, 97)
        Me.groupBox3.TabIndex = 26
        Me.groupBox3.TabStop = False
        Me.groupBox3.Text = "Acquisition Control"
        '
        'label7
        '
        Me.label7.AutoSize = True
        Me.label7.Location = New System.Drawing.Point(6, 22)
        Me.label7.Name = "label7"
        Me.label7.Size = New System.Drawing.Size(34, 13)
        Me.label7.TabIndex = 12
        Me.label7.Text = "Mode"
        '
        'modeComboBox
        '
        Me.modeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.modeComboBox.Enabled = False
        Me.modeComboBox.FormattingEnabled = True
        Me.modeComboBox.Location = New System.Drawing.Point(81, 19)
        Me.modeComboBox.Name = "modeComboBox"
        Me.modeComboBox.Size = New System.Drawing.Size(182, 21)
        Me.modeComboBox.TabIndex = 11
        '
        'playButton
        '
        Me.playButton.Enabled = False
        Me.playButton.ImageIndex = 0
        Me.playButton.ImageList = Me.ImageList
        Me.playButton.Location = New System.Drawing.Point(9, 46)
        Me.playButton.Name = "playButton"
        Me.playButton.Size = New System.Drawing.Size(123, 39)
        Me.playButton.TabIndex = 7
        Me.playButton.Text = "Play"
        Me.playButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.playButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText
        Me.playButton.UseVisualStyleBackColor = True
        '
        'ImageList
        '
        Me.ImageList.ImageStream = CType(resources.GetObject("ImageList.ImageStream"), System.Windows.Forms.ImageListStreamer)
        Me.ImageList.TransparentColor = System.Drawing.Color.White
        Me.ImageList.Images.SetKeyName(0, "play.bmp")
        Me.ImageList.Images.SetKeyName(1, "stop.bmp")
        '
        'stopButton
        '
        Me.stopButton.Enabled = False
        Me.stopButton.ImageIndex = 1
        Me.stopButton.ImageList = Me.ImageList
        Me.stopButton.Location = New System.Drawing.Point(138, 46)
        Me.stopButton.Name = "stopButton"
        Me.stopButton.Size = New System.Drawing.Size(125, 39)
        Me.stopButton.TabIndex = 8
        Me.stopButton.Text = "Stop"
        Me.stopButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.stopButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText
        Me.stopButton.UseVisualStyleBackColor = True
        '
        'groupBox2
        '
        Me.groupBox2.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.groupBox2.Controls.Add(Me.communicationButton)
        Me.groupBox2.Controls.Add(Me.deviceButton)
        Me.groupBox2.Controls.Add(Me.streamButton)
        Me.groupBox2.Location = New System.Drawing.Point(12, 338)
        Me.groupBox2.Name = "groupBox2"
        Me.groupBox2.Size = New System.Drawing.Size(272, 123)
        Me.groupBox2.TabIndex = 25
        Me.groupBox2.TabStop = False
        Me.groupBox2.Text = "Parameters and Controls"
        '
        'communicationButton
        '
        Me.communicationButton.Enabled = False
        Me.communicationButton.Location = New System.Drawing.Point(6, 25)
        Me.communicationButton.Name = "communicationButton"
        Me.communicationButton.Size = New System.Drawing.Size(257, 26)
        Me.communicationButton.TabIndex = 13
        Me.communicationButton.Text = "Communication control"
        Me.communicationButton.UseVisualStyleBackColor = True
        '
        'deviceButton
        '
        Me.deviceButton.Enabled = False
        Me.deviceButton.Location = New System.Drawing.Point(6, 57)
        Me.deviceButton.Name = "deviceButton"
        Me.deviceButton.Size = New System.Drawing.Size(257, 26)
        Me.deviceButton.TabIndex = 14
        Me.deviceButton.Text = "Device control"
        Me.deviceButton.UseVisualStyleBackColor = True
        '
        'streamButton
        '
        Me.streamButton.Enabled = False
        Me.streamButton.Location = New System.Drawing.Point(6, 89)
        Me.streamButton.Name = "streamButton"
        Me.streamButton.Size = New System.Drawing.Size(257, 26)
        Me.streamButton.TabIndex = 15
        Me.streamButton.Text = "Image stream control"
        Me.streamButton.UseVisualStyleBackColor = True
        '
        'groupBox1
        '
        Me.groupBox1.Controls.Add(Me.guidTextBox)
        Me.groupBox1.Controls.Add(Me.Label6)
        Me.groupBox1.Controls.Add(Me.nameTextBox)
        Me.groupBox1.Controls.Add(Me.label5)
        Me.groupBox1.Controls.Add(Me.modelTextBox)
        Me.groupBox1.Controls.Add(Me.label4)
        Me.groupBox1.Controls.Add(Me.manufacturerTextBox)
        Me.groupBox1.Controls.Add(Me.label3)
        Me.groupBox1.Controls.Add(Me.macAddressTextBox)
        Me.groupBox1.Controls.Add(Me.label2)
        Me.groupBox1.Controls.Add(Me.ipAddressTextBox)
        Me.groupBox1.Controls.Add(Me.label1)
        Me.groupBox1.Controls.Add(Me.connectButton)
        Me.groupBox1.Controls.Add(Me.disconnectButton)
        Me.groupBox1.Location = New System.Drawing.Point(12, 12)
        Me.groupBox1.Name = "groupBox1"
        Me.groupBox1.Size = New System.Drawing.Size(272, 217)
        Me.groupBox1.TabIndex = 24
        Me.groupBox1.TabStop = False
        Me.groupBox1.Text = "Connection"
        '
        'nameTextBox
        '
        Me.nameTextBox.Location = New System.Drawing.Point(81, 187)
        Me.nameTextBox.Name = "nameTextBox"
        Me.nameTextBox.ReadOnly = True
        Me.nameTextBox.Size = New System.Drawing.Size(182, 20)
        Me.nameTextBox.TabIndex = 13
        Me.nameTextBox.TabStop = False
        '
        'label5
        '
        Me.label5.AutoSize = True
        Me.label5.Location = New System.Drawing.Point(6, 190)
        Me.label5.Name = "label5"
        Me.label5.Size = New System.Drawing.Size(35, 13)
        Me.label5.TabIndex = 12
        Me.label5.Text = "Name"
        '
        'modelTextBox
        '
        Me.modelTextBox.Location = New System.Drawing.Point(81, 161)
        Me.modelTextBox.Name = "modelTextBox"
        Me.modelTextBox.ReadOnly = True
        Me.modelTextBox.Size = New System.Drawing.Size(182, 20)
        Me.modelTextBox.TabIndex = 11
        Me.modelTextBox.TabStop = False
        '
        'label4
        '
        Me.label4.AutoSize = True
        Me.label4.Location = New System.Drawing.Point(6, 164)
        Me.label4.Name = "label4"
        Me.label4.Size = New System.Drawing.Size(36, 13)
        Me.label4.TabIndex = 10
        Me.label4.Text = "Model"
        '
        'manufacturerTextBox
        '
        Me.manufacturerTextBox.Location = New System.Drawing.Point(81, 135)
        Me.manufacturerTextBox.Name = "manufacturerTextBox"
        Me.manufacturerTextBox.ReadOnly = True
        Me.manufacturerTextBox.Size = New System.Drawing.Size(182, 20)
        Me.manufacturerTextBox.TabIndex = 9
        Me.manufacturerTextBox.TabStop = False
        '
        'label3
        '
        Me.label3.AutoSize = True
        Me.label3.Location = New System.Drawing.Point(6, 138)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(70, 13)
        Me.label3.TabIndex = 8
        Me.label3.Text = "Manufacturer"
        '
        'macAddressTextBox
        '
        Me.macAddressTextBox.Location = New System.Drawing.Point(81, 83)
        Me.macAddressTextBox.Name = "macAddressTextBox"
        Me.macAddressTextBox.ReadOnly = True
        Me.macAddressTextBox.Size = New System.Drawing.Size(182, 20)
        Me.macAddressTextBox.TabIndex = 7
        Me.macAddressTextBox.TabStop = False
        '
        'label2
        '
        Me.label2.AutoSize = True
        Me.label2.Location = New System.Drawing.Point(6, 86)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(70, 13)
        Me.label2.TabIndex = 6
        Me.label2.Text = "MAC address"
        '
        'ipAddressTextBox
        '
        Me.ipAddressTextBox.Location = New System.Drawing.Point(81, 57)
        Me.ipAddressTextBox.Name = "ipAddressTextBox"
        Me.ipAddressTextBox.ReadOnly = True
        Me.ipAddressTextBox.Size = New System.Drawing.Size(182, 20)
        Me.ipAddressTextBox.TabIndex = 5
        Me.ipAddressTextBox.TabStop = False
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(6, 60)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(57, 13)
        Me.label1.TabIndex = 4
        Me.label1.Text = "IP address"
        '
        'connectButton
        '
        Me.connectButton.Location = New System.Drawing.Point(6, 19)
        Me.connectButton.Name = "connectButton"
        Me.connectButton.Size = New System.Drawing.Size(126, 26)
        Me.connectButton.TabIndex = 0
        Me.connectButton.Text = "Select / Connect"
        Me.connectButton.UseVisualStyleBackColor = True
        '
        'disconnectButton
        '
        Me.disconnectButton.Enabled = False
        Me.disconnectButton.Location = New System.Drawing.Point(138, 19)
        Me.disconnectButton.Name = "disconnectButton"
        Me.disconnectButton.Size = New System.Drawing.Size(125, 26)
        Me.disconnectButton.TabIndex = 3
        Me.disconnectButton.Text = "Disconnect"
        Me.disconnectButton.UseVisualStyleBackColor = True
        '
        'guidTextBox
        '
        Me.guidTextBox.Location = New System.Drawing.Point(81, 109)
        Me.guidTextBox.Name = "guidTextBox"
        Me.guidTextBox.ReadOnly = True
        Me.guidTextBox.Size = New System.Drawing.Size(182, 20)
        Me.guidTextBox.TabIndex = 15
        Me.guidTextBox.TabStop = False
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(6, 112)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(34, 13)
        Me.Label6.TabIndex = 14
        Me.Label6.Text = "GUID"
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(830, 473)
        Me.Controls.Add(Me.groupBox4)
        Me.Controls.Add(Me.groupBox3)
        Me.Controls.Add(Me.groupBox2)
        Me.Controls.Add(Me.groupBox1)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MinimumSize = New System.Drawing.Size(846, 512)
        Me.Name = "MainForm"
        Me.Text = "SimpleApplicationVB"
        Me.groupBox4.ResumeLayout(False)
        Me.groupBox3.ResumeLayout(False)
        Me.groupBox3.PerformLayout()
        Me.groupBox2.ResumeLayout(False)
        Me.groupBox1.ResumeLayout(False)
        Me.groupBox1.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Private WithEvents groupBox4 As System.Windows.Forms.GroupBox
    Private WithEvents display As PvGUIDotNet.PvDisplayControl
    Private WithEvents statusControl As PvGUIDotNet.PvStatusControl
    Private WithEvents groupBox3 As System.Windows.Forms.GroupBox
    Private WithEvents label7 As System.Windows.Forms.Label
    Private WithEvents modeComboBox As System.Windows.Forms.ComboBox
    Private WithEvents playButton As System.Windows.Forms.Button
    Private WithEvents stopButton As System.Windows.Forms.Button
    Private WithEvents groupBox2 As System.Windows.Forms.GroupBox
    Private WithEvents communicationButton As System.Windows.Forms.Button
    Private WithEvents deviceButton As System.Windows.Forms.Button
    Private WithEvents streamButton As System.Windows.Forms.Button
    Private WithEvents groupBox1 As System.Windows.Forms.GroupBox
    Private WithEvents nameTextBox As System.Windows.Forms.TextBox
    Private WithEvents label5 As System.Windows.Forms.Label
    Private WithEvents modelTextBox As System.Windows.Forms.TextBox
    Private WithEvents label4 As System.Windows.Forms.Label
    Private WithEvents manufacturerTextBox As System.Windows.Forms.TextBox
    Private WithEvents label3 As System.Windows.Forms.Label
    Private WithEvents macAddressTextBox As System.Windows.Forms.TextBox
    Private WithEvents label2 As System.Windows.Forms.Label
    Private WithEvents ipAddressTextBox As System.Windows.Forms.TextBox
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents connectButton As System.Windows.Forms.Button
    Private WithEvents disconnectButton As System.Windows.Forms.Button
    Friend WithEvents ImageList As System.Windows.Forms.ImageList
    Private WithEvents guidTextBox As System.Windows.Forms.TextBox
    Private WithEvents Label6 As System.Windows.Forms.Label

End Class
