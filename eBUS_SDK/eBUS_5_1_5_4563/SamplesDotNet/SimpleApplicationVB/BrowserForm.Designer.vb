<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class BrowserForm
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
        'Me.PvGenBrowserControl1 = New PvGUIDotNet.PvGenBrowserControl
        Me.Browser = New PvGUIDotNet.PvGenBrowserControl()
        Me.SuspendLayout()
        '
        'Browser
        '
        Me.Browser.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Browser.GenParameterArray = Nothing
        Me.Browser.Location = New System.Drawing.Point(0, 0)
        Me.Browser.Name = "Browser"
        Me.Browser.Size = New System.Drawing.Size(353, 530)
        Me.Browser.TabIndex = 0
        Me.Browser.Visibility = PvDotNet.PvGenVisibility.Beginner
        '
        'BrowserForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(353, 530)
        Me.Controls.Add(Me.Browser)
        Me.Name = "BrowserForm"
        Me.Text = "BrowserForm"
        Me.ResumeLayout(False)

    End Sub
    Public WithEvents Browser As PvGUIDotNet.PvGenBrowserControl
End Class
