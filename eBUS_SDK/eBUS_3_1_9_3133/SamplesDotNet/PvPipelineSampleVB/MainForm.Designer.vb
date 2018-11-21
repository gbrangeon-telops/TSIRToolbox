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
        Me.stopButton = New System.Windows.Forms.Button()
        Me.displayControl = New PvGUIDotNet.PvDisplayControl()
        Me.sixLabel = New System.Windows.Forms.Label()
        Me.fiveLabel = New System.Windows.Forms.Label()
        Me.fourLabel = New System.Windows.Forms.Label()
        Me.threeLabel = New System.Windows.Forms.Label()
        Me.twoLabel = New System.Windows.Forms.Label()
        Me.oneLabel = New System.Windows.Forms.Label()
        Me.timer = New System.Windows.Forms.Timer(Me.components)
        Me.browser = New PvGUIDotNet.PvGenBrowserControl()
        Me.SuspendLayout()
        '
        'stopButton
        '
        Me.stopButton.Enabled = False
        Me.stopButton.Location = New System.Drawing.Point(26, 85)
        Me.stopButton.Name = "stopButton"
        Me.stopButton.Size = New System.Drawing.Size(75, 23)
        Me.stopButton.TabIndex = 16
        Me.stopButton.Text = "Stop"
        Me.stopButton.UseVisualStyleBackColor = True
        '
        'displayControl
        '
        Me.displayControl.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.displayControl.BackColor = System.Drawing.Color.Transparent
        Me.displayControl.BackgroundColor = System.Drawing.Color.DarkGray
        Me.displayControl.Location = New System.Drawing.Point(139, 9)
        Me.displayControl.Name = "displayControl"
        Me.displayControl.Size = New System.Drawing.Size(461, 411)
        Me.displayControl.TabIndex = 15
        '
        'sixLabel
        '
        Me.sixLabel.AutoSize = True
        Me.sixLabel.Enabled = False
        Me.sixLabel.Location = New System.Drawing.Point(12, 133)
        Me.sixLabel.Name = "sixLabel"
        Me.sixLabel.Size = New System.Drawing.Size(87, 13)
        Me.sixLabel.TabIndex = 14
        Me.sixLabel.Text = "6. Disconnecting"
        '
        'fiveLabel
        '
        Me.fiveLabel.AutoSize = True
        Me.fiveLabel.Enabled = False
        Me.fiveLabel.Location = New System.Drawing.Point(12, 113)
        Me.fiveLabel.Name = "fiveLabel"
        Me.fiveLabel.Size = New System.Drawing.Size(95, 13)
        Me.fiveLabel.TabIndex = 13
        Me.fiveLabel.Text = "5. Stopping stream"
        '
        'fourLabel
        '
        Me.fourLabel.AutoSize = True
        Me.fourLabel.Enabled = False
        Me.fourLabel.Location = New System.Drawing.Point(12, 69)
        Me.fourLabel.Name = "fourLabel"
        Me.fourLabel.Size = New System.Drawing.Size(66, 13)
        Me.fourLabel.TabIndex = 12
        Me.fourLabel.Text = "4. Streaming"
        '
        'threeLabel
        '
        Me.threeLabel.AutoSize = True
        Me.threeLabel.Enabled = False
        Me.threeLabel.Location = New System.Drawing.Point(12, 49)
        Me.threeLabel.Name = "threeLabel"
        Me.threeLabel.Size = New System.Drawing.Size(89, 13)
        Me.threeLabel.TabIndex = 11
        Me.threeLabel.Text = "3. Starting stream"
        '
        'twoLabel
        '
        Me.twoLabel.AutoSize = True
        Me.twoLabel.Enabled = False
        Me.twoLabel.Location = New System.Drawing.Point(12, 29)
        Me.twoLabel.Name = "twoLabel"
        Me.twoLabel.Size = New System.Drawing.Size(107, 13)
        Me.twoLabel.TabIndex = 10
        Me.twoLabel.Text = "2. Configuring device"
        '
        'oneLabel
        '
        Me.oneLabel.AutoSize = True
        Me.oneLabel.Enabled = False
        Me.oneLabel.Location = New System.Drawing.Point(12, 9)
        Me.oneLabel.Name = "oneLabel"
        Me.oneLabel.Size = New System.Drawing.Size(73, 13)
        Me.oneLabel.TabIndex = 9
        Me.oneLabel.Text = "1. Connecting"
        '
        'timer
        '
        Me.timer.Interval = 1000
        '
        'browser
        '
        Me.browser.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.browser.GenParameterArray = Nothing
        Me.browser.Location = New System.Drawing.Point(606, 12)
        Me.browser.Name = "browser"
        Me.browser.Size = New System.Drawing.Size(299, 408)
        Me.browser.TabIndex = 17
        Me.browser.Visibility = PvDotNet.PvGenVisibility.Beginner
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(917, 432)
        Me.Controls.Add(Me.browser)
        Me.Controls.Add(Me.stopButton)
        Me.Controls.Add(Me.displayControl)
        Me.Controls.Add(Me.sixLabel)
        Me.Controls.Add(Me.fiveLabel)
        Me.Controls.Add(Me.fourLabel)
        Me.Controls.Add(Me.threeLabel)
        Me.Controls.Add(Me.twoLabel)
        Me.Controls.Add(Me.oneLabel)
        Me.Name = "MainForm"
        Me.Text = "PvPipelineSampleVB"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents stopButton As System.Windows.Forms.Button
    Private WithEvents sixLabel As System.Windows.Forms.Label
    Private WithEvents fiveLabel As System.Windows.Forms.Label
    Private WithEvents fourLabel As System.Windows.Forms.Label
    Private WithEvents threeLabel As System.Windows.Forms.Label
    Private WithEvents twoLabel As System.Windows.Forms.Label
    Private WithEvents oneLabel As System.Windows.Forms.Label
    Friend WithEvents timer As System.Windows.Forms.Timer
    Private WithEvents displayControl As PvGUIDotNet.PvDisplayControl
    Friend WithEvents browser As PvGUIDotNet.PvGenBrowserControl

End Class
