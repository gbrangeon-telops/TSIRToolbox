' *****************************************************************************
'
'     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
'
' *****************************************************************************

Public Class MainForm

    Public Sub New()
        InitializeComponent()

        ' Handlers used to callbacks in the UI thread.
        mDisconnectedHandler = New GenericHandler(AddressOf OnDisconnected)
        mAcquisitionModeChangedHandler = New GenericHandler(AddressOf OnAcquisitionModeChanged)
        mAcquisitionStateChangedHandler = New GenericHandler(AddressOf OnAcquisitionStateChanged)

        ' Set browser form owners
        mCommBrowser.Owner = Me
        mDeviceBrowser.Owner = Me
        mStreamBrowser.Owner = Me

        ' Create display thread
        mDisplayThread = New PvDisplayThread()
    End Sub

    ' Declare delegate GenericHandler.
    Delegate Sub GenericHandler()

    ' Handler used to bring link disconnected event in the main UI thread.
    Dim mDisconnectedHandler As GenericHandler

    ' Handler used to bring acquisition mode changed event in the main UI thread.
    Dim mAcquisitionModeChangedHandler As GenericHandler

    ' Handler used to bring acquisition state manager callbacks in the main UI thread.
    Dim mAcquisitionStateChangedHandler As GenericHandler

    ' Initializes objects for PvDevice, PvStream, PvPipeline.
    Private mDevice As PvDevice = Nothing
    Private mStream As PvStream = Nothing
    Private mPipeline As PvPipeline = Nothing

    ' Acquisition state manager.
    Private mAcquisitionManager As PvAcquisitionStateManager = Nothing

    ' Display thread
    Private mDisplayThread As PvDisplayThread = Nothing

    ' Parameter browsers.
    Private mCommBrowser = New BrowserForm
    Private mDeviceBrowser = New BrowserForm
    Private mStreamBrowser = New BrowserForm

    ' Subroutine that set the state of the GUI.
    Private Sub SetGUIState()
        ' Set the state of connect and disconnect button.
        connectButton.Enabled = IsNothing(mDevice)
        disconnectButton.Enabled = Not IsNothing(mDevice)

        ' Set GigE Vision control settings.
        SetGenControlSettings()

        ' Set Acquisition controls.
        SetAcquisitionControl()
    End Sub

    ' Set the state of the Communication, Device and Image Stream control buttons.
    Private Sub SetGenControlSettings()
        ' Set the state of the communication, Device and Image Stream control button.
        communicationButton.Enabled = Not IsNothing(mDevice)
        deviceButton.Enabled = Not IsNothing(mDevice)
        streamButton.Enabled = Not IsNothing(mStream)
    End Sub

    ' Set the state of the acquisition control buttons.
    Private Sub SetAcquisitionControl()
        If Me.InvokeRequired Then
            Me.Invoke(New MethodInvoker(AddressOf SetAcquisitionControl))
        Else
            If IsNothing(mAcquisitionManager) Then
                ' Device is not connected so disables all acquisition controls.
                modeComboBox.Enabled = False
                playButton.Enabled = False
                stopButton.Enabled = False
            Else
                ' Read current acquisition state.
                Dim lLocked As Boolean = (mAcquisitionManager.State = PvAcquisitionState.Unlocked)

                ' Mode and play are enabled if not locked.
                modeComboBox.Enabled = lLocked
                playButton.Enabled = lLocked

                ' Stop is enabled only if locked.
                stopButton.Enabled = Not lLocked
            End If
        End If
    End Sub

    ' Connects to the device from a device info.
    Private Sub Connect(ByVal aDI As PvDeviceInfo)
        ' Just in case we came here still connected...
        Disconnect()

        Try

            ' Connect to device using device info.
            mDevice = PvDevice.CreateAndConnect(aDI)

            ' Open stream using device IP address.
            mStream = PvStream.CreateAndOpen(aDI.ConnectionID)

            ' Perform GigE Vision specific initialization
            If aDI.Type = PvDeviceInfoType.GEV Then

                Dim lDeviceGEV As PvDeviceGEV = TryCast(mDevice, PvDeviceGEV)
                Dim lStreamGEV As PvStreamGEV = TryCast(mStream, PvStreamGEV)

                ' Negotiate packet size
                lDeviceGEV.NegotiatePacketSize()

                ' Set stream destination to our stream object.
                lDeviceGEV.SetStreamDestination(lStreamGEV.LocalIPAddress, lStreamGEV.LocalPort)

            End If

            ' Create pipeline - requires stream.
            mPipeline = New PvPipeline(mStream)

        Catch lEx As PvException

            ' Display error message, disconnect device and abort.
            MessageBox.Show(lEx.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error)
            Disconnect()
            Return

        End Try

        If mDevice.IsConnected Then

            Try

                ' Register to all events of the parameters in the device's node map.
                For Each lParameter As PvGenParameter In mDevice.Parameters
                    AddHandler lParameter.OnParameterUpdate, AddressOf OnParameterChanged
                Next

                ' Connect device disconnect notification.
                AddHandler mDevice.OnLinkDisconnected, AddressOf OnLinkDisconnected

                ' Update the UI attributes
                UpdateAttributes(aDI)

                ' Add items in acquisition mode combo box.
                modeComboBox.Items.Clear()
                Dim lMode As PvGenEnum = mDevice.Parameters.GetEnum("AcquisitionMode")
                If IsNothing(lMode) <> True Then

                    For Each lEntry As PvGenEnumEntry In lMode

                        If IsNothing(lEntry) <> True Then

                            Dim lIndex As Integer = modeComboBox.Items.Add(lEntry.ValueString)
                            If lEntry.ValueInt = lMode.ValueInt Then
                                modeComboBox.SelectedIndex = lIndex
                            End If

                        End If

                    Next

                End If

            Catch lEx As PvException

                MessageBox.Show(lEx.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error)
                Disconnect()
                Return

            End Try

        End If

        If mStream.IsOpen Then
            ' Start receiving image.
            StartStreaming()
        End If

        ' Set the GUI according to our new status.
        SetGUIState()

    End Sub

    ' Update device attributes on the GUID
    Private Sub UpdateAttributes(ByVal aDI As PvDeviceInfo)

        Dim lVendorName As String = ""
        Dim lModelName As String = ""
        Dim lMACAddress As String = ""
        Dim lIPAddress As String = ""
        Dim lGUID As String = ""
        Dim lUserDefinedName As String = ""

        If Not IsNothing(aDI) Then

            ' Get device attributes
            lVendorName = aDI.VendorName
            lModelName = aDI.ModelName
            lMACAddress = "N/A"
            lIPAddress = "N/A"
            lGUID = "N/A"
            lUserDefinedName = aDI.UserDefinedName

            ' GigE Vision specific device attributes
            Dim lDeviceInfoGEV As PvDeviceInfoGEV = TryCast(aDI, PvDeviceInfoGEV)
            If Not IsNothing(lDeviceInfoGEV) Then

                lMACAddress = lDeviceInfoGEV.MACAddress
                lIPAddress = lDeviceInfoGEV.IPAddress

            End If

            ' USB3 Vision specific device attributes
            Dim lDeviceInfoU3V As PvDeviceInfoU3V = TryCast(aDI, PvDeviceInfoU3V)
            If Not IsNothing(lDeviceInfoU3V) Then

                lGUID = lDeviceInfoU3V.DeviceGUID

            End If

        End If

        ' Fill device attribute text boxes
        manufacturerTextBox.Text = lVendorName
        modelTextBox.Text = lModelName
        macAddressTextBox.Text = lMACAddress
        ipAddressTextBox.Text = lIPAddress
        guidTextBox.Text = lGUID
        nameTextBox.Text = lUserDefinedName

    End Sub

    ' Disconnects from the device.
    Private Sub Disconnect()

        ' Close all configuration child windows.
        mDeviceBrowser.Browser.GenParameterArray = Nothing
        CloseGenWindow(mDeviceBrowser)
        mStreamBrowser.Browser.GenParameterArray = Nothing
        CloseGenWindow(mStreamBrowser)
        If IsNothing(mStream) <> True Then
            ' If streaming, stop streaming.
            If mStream.IsOpen = True Then
                StopStreaming()
                mStream.Close()
                mStream = Nothing
            End If
        End If
        If IsNothing(mDevice) <> True Then
            If mDevice.IsConnected = True Then
                ' Disconnect events.
                RemoveHandler mDevice.OnLinkDisconnected, AddressOf OnLinkDisconnected
                For Each lP As PvGenParameter In mDevice.Parameters
                    RemoveHandler lP.OnParameterUpdate, AddressOf OnParameterChanged
                Next

                mDevice.Disconnect()
                mDevice = Nothing
            End If
        End If

        display.Clear()
        UpdateAttributes(Nothing)
        statusControl.Warning = ""

        ' Sync the UI with our new status.
        SetGUIState()
    End Sub

    ' Setups streaming. After calling this method the application is ready to receive data.
    ' StartAcquisition will instruct the device to actually start sending data.
    Private Sub StartStreaming()
        ' Configure display thread.
        AddHandler mDisplayThread.OnBufferDisplay, AddressOf OnBufferDisplay

        ' Configure status control.
        statusControl.Stream = mStream
        statusControl.DisplayThread = mDisplayThread

        ' Start threads.
        mDisplayThread.Start(mPipeline, mDevice.Parameters)
        mDisplayThread.Priority = PvThreadPriority.AboveNormal

        ' Configure acquisition state manager.
        mAcquisitionManager = New PvAcquisitionStateManager(mDevice, mStream)
        AddHandler mAcquisitionManager.OnAcquisitionStateChanged, AddressOf OnAcquisitionStateChanged

        ' Start pipeline.
        mPipeline.Start()
    End Sub

    ' Stops streaming. After calling this method the application is no longer armed or ready to receive data.
    Private Sub StopStreaming()
        ' Are we streaming?
        If Not mDisplayThread.IsRunning Then
            Return
        End If

        ' Stop display thread.
        mDisplayThread.Stop(False)

        ' Release acquisition manager.
        mAcquisitionManager.Dispose()
        mAcquisitionManager = Nothing
        If IsNothing(mPipeline) <> True Then
            ' Stop pipeline
            If mPipeline.IsStarted Then
                mPipeline.Stop()
            End If
        End If
        ' Wait on and release display thread.
        mDisplayThread.WaitComplete()
    End Sub

    ' Starts acquisition.
    Private Sub StartAcquisition()
        ' Get payload size
        Dim lPayloadSize = mDevice.PayloadSize

        ' Propagate to pipeline to make sure buffers are big enough.
        mPipeline.BufferSize = lPayloadSize

        ' Reset pipeline.
        mPipeline.Reset()

        ' Reset stream statistics.
        Dim lResetStats As PvGenCommand = mStream.Parameters.GetCommand("Reset")
        lResetStats.Execute()

        ' Reset display thread stats ( mostly frames displayed per seconds).
        mDisplayThread.ResetStatistics()

        ' Use acquisition manager to send the acquisition start command to the device.
        mAcquisitionManager.Start()
    End Sub

    ' Stops the acquisition.
    Private Sub StopAcquisition()
        Try
            ' Use acquisition manager to send the acquisition stop command to the device.
            mAcquisitionManager.Stop()
        Catch lEx As PvException
            MessageBox.Show(lEx.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try

    End Sub

    ' Callback from display thread requesting a buffer to be displayed.
    Private Sub OnBufferDisplay(aDisplayThread As PvDisplayThread, aBuffer As PvBuffer)
        display.Display(aBuffer)
    End Sub

    ' Displays a GenICam browser form. If already visible, close it for toggle effect.
    Private Sub ShowGenWindow(ByVal aForm As BrowserForm, ByVal aParams As PvGenParameterArray, ByVal aTitle As String)
        If aForm.Visible Then
            CloseGenWindow(aForm)
            Return
        End If

        ' Create, assigned parameters, set title and show modeless.
        aForm.Text = aTitle
        aForm.Browser.GenParameterArray = aParams
        aForm.Show()
    End Sub

    ' Closes a GenICam browser form.
    Private Sub CloseGenWindow(ByVal aForm As Form)
        aForm.Hide()
    End Sub

    'Reaction to device disconnected event: stop streaming, close device connection.
    Private Sub OnDisconnected()
        MessageBox.Show("Connection to device lost.", Text)
        StopStreaming()
        Disconnect()
    End Sub

    ' Direct device disconnect handler. Just jump back to main UI thread.
    Private Sub OnLinkDisconnected(ByVal aDevice As PvDevice)
        BeginInvoke(mDisconnectedHandler)
    End Sub

    ' GenICam parameter invalidation event, registered for all parameters.
    Private Sub OnParameterChanged(ByVal aParameter As PvGenParameter)
        Dim lName As String = aParameter.Name
        If lName = "AcquisitionMode" Then
            ' Have main UI thread update the acquisition mode combo box status.
            BeginInvoke(mAcquisitionModeChangedHandler)
        End If
    End Sub

    ' Acquisition mode event handler in main thread.
    Private Sub OnAcquisitionModeChanged()
        ' Get parameter
        Dim lParameter As PvGenEnum = mDevice.Parameters.GetEnum("AcquisitionMode")

        ' Update value: find which matches in the combo box.
        Dim lValue As String = lParameter.ValueString
        For Each lString As String In modeComboBox.Items
            If lValue = lString Then
                modeComboBox.SelectedValue = modeComboBox.Items.ToString()
            End If
        Next
    End Sub

    ' Direct acquisition state changed handler. Bring back to main UI thread.
    Private Sub OnAcquisitionStateChanged(ByVal aDevice As PvDevice, ByVal aStream As PvStream, ByVal aSource As UInt32, ByVal aState As PvAcquisitionState)
        'Invoke event in main UI thread
        BeginInvoke(mAcquisitionStateChangedHandler)
    End Sub

    'OnAcquisitionStateChanged main UI thread counterpart. Syncs acquisition control status.
    Private Sub OnAcquisitionStateChanged()
        SetAcquisitionControl()
    End Sub

    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' Just syncs the enabled state of the UI.
        SetGUIState()
    End Sub

    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles connectButton.Click
        ' create a device finder form and open the select device dialog.
        Dim lFinder As PvDeviceFinderForm = New PvDeviceFinderForm()
        Try
            ' Show device finder
            If ((lFinder.ShowDialog() <> DialogResult.OK) OrElse (IsNothing(lFinder.Selected) = True)) Then
                Return
            End If

            'Display hourglass.
            Dim lPrevCursor As Cursor = Cursor
            Cursor = Cursors.WaitCursor

            ' Connect to device.
            Connect(lFinder.Selected)

            ' Restore cursor.
            Cursor = lPrevCursor
        Catch lEx As Exception
            MessageBox.Show(lEx.Message, Text, MessageBoxButtons.OK, MessageBoxIcon.Error)
            Return
        End Try

    End Sub

    Private Sub btnDisconnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles disconnectButton.Click
        ' Show hourglass.
        Dim lPrevCursor As Cursor = Cursor
        Cursor = Cursors.WaitCursor

        ' Disconnect from device.
        Disconnect()

        ' Restore cursor.
        Cursor = lPrevCursor
    End Sub

    Private Sub btnPlay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles playButton.Click
        StartAcquisition()
    End Sub

    Private Sub btnStop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles stopButton.Click
        StopAcquisition()
    End Sub

    Private Sub btnCommunication_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles communicationButton.Click
        ShowGenWindow(mCommBrowser, mDevice.CommunicationParameters, "Communication Control")
    End Sub

    Private Sub btnDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles deviceButton.Click
        ShowGenWindow(mDeviceBrowser, mDevice.Parameters, "Device Control")
    End Sub

    Private Sub btnStream_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles streamButton.Click
        ShowGenWindow(mStreamBrowser, mStream.Parameters, "Image Stream Control")
    End Sub

    Private Sub cbxMode_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles modeComboBox.SelectedIndexChanged
        'If nothing selected, do nothing.
        If modeComboBox.SelectedIndex < 0 Then
            Return
        End If

        Dim lMode As PvGenEnum = mDevice.Parameters.GetEnum("AcquisitionMode")
        ' Take selection, propagate to parameter.
        Dim lSelected As String = CType(modeComboBox.SelectedItem, String)
        lMode.ValueString = modeComboBox.SelectedItem.ToString()
    End Sub

    Private Sub MainForm_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Disconnect()
    End Sub

End Class
