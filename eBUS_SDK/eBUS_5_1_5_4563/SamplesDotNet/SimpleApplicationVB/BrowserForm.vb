' ****************************************************************************
'
'     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
'
' ****************************************************************************

Public Class BrowserForm
    Private Sub BrowserForm_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        If Modal <> True And e.CloseReason = CloseReason.UserClosing Then
            e.Cancel = True
            Hide()
        End If
    End Sub
End Class



