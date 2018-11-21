' *****************************************************************************
'
'     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
'
' *****************************************************************************

Public Class MainForm

#Region "Constructor"
    Public Sub New()
        InitializeComponent()

        ' Handlers used to callbacks in the UI thread
        mDisconnectedHandler = New GenericHandler(AddressOf OnDisconnected)
        mAcquisitionModeChangedHandler = New GenericHandler(AddressOf OnAcquisitionModeChanged)
        mAcquisitionStateChangedHandler = New GenericHandler(AddressOf OnAcquisitionStateChanged)

        ' Create pipeline - requires stream
        mPipeline = New PvPipeline(mStream)

        ' Set browser form owners
        mCommBrowser.Owner = Me
        mDeviceBrowser.Owner = Me
        mStreamBrowser.Owner = Me

        ' Create display thread
        mDisplayThread = New PvDisplayThread()
    End Sub
#End Region

#Region "Delegates"
    ' Declare delegate GenericHandler
    Delegate Sub GenericHandler()

    ' Handler used to bring link disconnected event in the main UI thread
    Dim mDisconnectedHandler As GenericHandler

    ' Handler used to bring acquisition mode changed event in the main UI thread
    Dim mAcquisitionModeChangedHandler As GenericHandler

    ' Handler used to bring acquisition state manager callbacks in the main UI thread
    Dim mAcquisitionStateChangedHandler As GenericHandler
#End Region

#Region "Private data members"
    ' Instantiates objects for PvDevice, PvStream, PvPipeline
    Private mDevice As New PvDevice
    Private mStream As PvStream = New PvStream
    Private mPipeline As PvPipeline = Nothing

    ' Acquisition state manager
    Private mAcquisitionManager As PvAcquisitionStateManager = Nothing

    ' Display thread
    Private mDisplayThread As PvDisplayThread = Nothing

    ' Parameter browsers
    Private mCommBrowser = New BrowserForm
    Private mDeviceBrowser = New BrowserForm
    Private mStreamBrowser = New BrowserForm
#End Region

#Region "Utilities subroutines"
    ' Subroutine that set the state of the GUI
    Private Sub SetGUIState()
        ' Set the state of connect and disconnect button
        connectButton.Enabled = (mDevice.IsConnected = False) AndAlso (mStream.IsOpened = False)
        disconnectButton.Enabled = (mDevice.IsConnected = True) OrElse (mStream.IsOpened = True)

        ' Set Gev control settings
        SetGenControlSettings()

        ' Set Acquisition controls
        SetAcquisitionControl()
    End Sub

    ' Set the state of the Communication, Device and Image Stream control buttons
    Private Sub SetGenControlSettings()
        ' Set the state of the Device and Image Stream control button
        deviceButton.Enabled = mDevice.IsConnected
        streamButton.Enabled = mStream.IsOpened
        ' Communication button is always enabled
        communicationButton.Enabled = True
    End Sub

    ' Set the state of the acquisitio control buttons
    Private Sub SetAcquisitionControl()
        If Me.InvokeRequired Then
            Me.Invoke(New MethodInvoker(AddressOf SetAcquisitionControl))
        Else
            If mDevice.IsConnected = False Then
                ' Device is not connected so diables all acquisition controls
                modeComboBox.Enabled = False
                playButton.Enabled = False
                stopButton.Enabled = False
            Else
                ' Read current acquisition state
                Dim lLocked As Boolean = (mAcquisitionManager.State = PvAcquisitionState.Unlocked)

                ' Mode and play are enabled if not locked
                modeComboBox.Enabled = lLocked
                playButton.Enabled = lLocked

                ' Stop is enabled only if locked
                stopButton.Enabled = Not lLocked
            End If
        End If
    End Sub

    ' Connects to the device from a device info
    Private Sub Connect(ByVal aDI As PvDeviceInfo)
        ' Just in case we came here still connected...
        Disconnect()

        Try
            ' Connect to device using device info
            mDevice.Connect(aDI)

            ' Open stream using device IP address
            mStream.Open(aDI.IPAddress)

            ' Negotiate packete size
            mDevice.NegotiatePacketSize()

            ' Set stream destination to our stream object
            mDevice.SetStreamDestination(mStream.LocalIPAddress, mStream.LocalPort)
        Catch lEx As PvException
            ' Display error message, disconnect device and abort
            MessageBox.Show(lEx.Message)
            Disconnect()
            Return
        End Try

        If mDevice.IsConnected = True Then
            Try
                ' Register to all events of the parameters in the device's node map
                For Each lParameter As PvGenParameter In mDevice.GenParameters
                    AddHandler lParameter.OnParameterUpdate, AddressOf OnParameterChanged
                Next

                ' Connect device disconnect notification
                AddHandler mDevice.OnLinkDisconnected, AddressOf OnLinkDisconnected

                ' Add device information
                manufacturerTextBox.Text = aDI.Vendor
                modelTextBox.Text = aDI.Model
                ipAddressTextBox.Text = aDI.IPAddress
                macAddressTextBox.Text = aDI.MACAddress
                If aDI.UserDefinedName = "" Then
                    nameTextBox.Text = "N/A"
                Else
                    nameTextBox.Text = aDI.UserDefinedName
                End If

                ' Add items in acquisition mode combo box
                modeComboBox.Items.Clear()
                Dim lMode As PvGenEnum = mDevice.GenParameters.GetEnum("AcquisitionMode")
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
                MessageBox.Show(lEx.Message)
                Disconnect()
                Return
            End Try
        End If

        If mStream.IsOpened Then
            ' Start receiving image
            StartStreaming()
        End If

        ' Set the GUI according to our new status
        SetGUIState()
    End Sub

    ' Disconnects from the device
    Private Sub Disconnect()

        ' Close all configuration child windows
        mDeviceBrowser.Browser.GenParameterArray = Nothing
        CloseGenWindow(mDeviceBrowser)
        mStreamBrowser.Browser.GenParameterArray = Nothing
        CloseGenWindow(mStreamBrowser)

        ' If streaming, stop streaming
        If mStream.IsOpened = True Then
            StopStreaming()
            mStream.Close()
        End If

        If mDevice.IsConnected = True Then
            ' Disconnect events
            RemoveHandler mDevice.OnLinkDisconnected, AddressOf OnLinkDisconnected
            For Each lP As PvGenParameter In mDevice.GenParameters
                RemoveHandler lP.OnParameterUpdate, AddressOf OnParameterChanged
            Next

            mDevice.Disconnect()
        End If

        display.Clear()

        manufacturerTextBox.Text = ""
        modelTextBox.Text = ""
        ipAddressTextBox.Text = ""
        macAddressTextBox.Text = ""
        nameTextBox.Text = ""

        statusControl.Warning = ""

        ' Sync the UI with our new status
        SetGUIState()
    End Sub

    ' Setups streaming. After calling this method the application is ready to receive data.
    ' StartAcquisition will instruct the device to actually start sending data.
    Private Sub StartStreaming()
        ' Configure display thread
        AddHandler mDisplayThread.OnBufferDisplay, AddressOf OnBufferDisplay

        ' Configure status control
        statusControl.Stream = mStream
        statusControl.DisplayThread = mDisplayThread

        ' Start threads
        mDisplayThread.Start(mPipeline, mDevice.GenParameters)
        mDisplayThread.Priority = PvThreadPriority.AboveNormal

        ' Configure acquisition state manager
        mAcquisitionManager = New PvAcquisitionStateManager(mDevice, mStream)
        AddHandler mAcquisitionManager.OnAcquisitionStateChanged, AddressOf OnAcquisitionStateChanged

        ' Start pipeline
        mPipeline.Start()
    End Sub

    ' Stops streaming. After calling this method the application is no longer armed or ready to receive data.
    Private Sub StopStreaming()
        ' Are we streaming?
        If Not mDisplayThread.IsRunning Then
            Return
        End If

        ' Stop display thread
        mDisplayThread.Stop(False)

        ' Release acquisition manager
        mAcquisitionManager = Nothing

        ' Stop pipeline
        If mPipeline.IsStarted Then
            mPipeline.Stop()
        End If

        ' Wait on and release display thread
        mDisplayThread.WaitComplete()
    End Sub

    ' Starts acquisition.
    Private Sub StartAcquisition()
        ' Get payload size
        Dim lPayloadSize As UInt32 = PayloadSize
        If lPayloadSize > 0 Then
            ' Propagate to pipeline to make sure buffers are big enough
            mPipeline.BufferSize = lPayloadSize
        End If

        ' Reset pipeline
        mPipeline.Reset()

        ' Reset stream statistics
        Dim lResetStats As PvGenCommand = mStream.Parameters.GetCommand("Reset")
        lResetStats.Execute()

        ' Reset display thread stats ( mostly frames displayed per seconds)
        mDisplayThread.ResetStatistics()

        ' Use acquisition manager to send the acquisition start command to the device
        mAcquisitionManager.Start()
    End Sub

    ' Stops the acquisition.
    Private Sub StopAcquisition()
        Try
            ' Use acquisition manager to send the acquisition stop command to the device
            mAcquisitionManager.Stop()
        Catch lEx As PvException
            MessageBox.Show(lEx.Message, Text)
        End Try

    End Sub

    ' Callback from display thread requesting a buffer to be displayed
    Private Sub OnBufferDisplay(aDisplayThread As PvDisplayThread, aBuffer As PvBuffer)
        display.Display(aBuffer)
    End Sub

    ' Retrieve or guess the payload size
    Public ReadOnly Property PayloadSize() As UInt32
        Get
            Try
                ' Get parameters required
                Dim lPayloadSize As PvGenInteger = mDevice.GenParameters.GetInteger("PayloadSize")
                Dim lWidth As PvGenInteger = mDevice.GenParameters.GetInteger("Width")
                Dim lHeight As PvGenInteger = mDevice.GenParameters.GetInteger("Height")
                Dim lPixelFormat As PvGenEnum = mDevice.GenParameters.GetEnum("PixelFormat")

                ' Try getting the payload size from the PayloadSize mandatory parameter
                Dim lPayloadSizeValue As Int64 = 0
                If IsNothing(lPayloadSize) <> True Then
                    lPayloadSizeValue = lPayloadSize.Value
                End If

                ' Compute poor man's payload size - for devices not maintaining PayloadSize properly
                Dim lPoorMansPayloadSize As Int64 = 0
                If (IsNothing(lWidth) <> True) AndAlso (IsNothing(lHeight) <> True) AndAlso (IsNothing(lPixelFormat) <> True) Then
                    Dim lWidthValue As Int64 = lWidth.Value
                    Dim lHeightValue As Int64 = lHeight.Value

                    Dim lPixelFormatValue As Int64 = lPixelFormat.ValueInt
                    Dim lPixelSizeInBits As Int64 = PvImage.GetPixelBitCount(CType(lPixelFormatValue, PvPixelType))

                    lPoorMansPayloadSize = (lWidthValue * lHeightValue * lPixelSizeInBits) / 8
                End If

                ' Take max
                Dim lBestPayloadSize As Int64 = Math.Max(lPayloadSizeValue, lPoorMansPayloadSize)
                If ((lBestPayloadSize > 0) AndAlso (lBestPayloadSize < UInt32.MaxValue)) Then
                    ' Round up to make it mod 32 (works around an issue with some devices)
                    If (lBestPayloadSize Mod 32) <> 0 Then
                        lBestPayloadSize = ((lBestPayloadSize / 32) + 1) * 32
                    End If

                    Return CType(lBestPayloadSize, UInt32)
                End If
            Catch lEx As Exception
                MessageBox.Show(lEx.Message)
                Disconnect()
                Return 0
            End Try
            ' Could not compute/retrieve payload size...
            Return 0
        End Get
    End Property

    ' Displays a GenICam browser form. If already visible, close it for toggle effect.
    Private Sub ShowGenWindow(ByVal aForm As BrowserForm, ByVal aParams As PvGenParameterArray, ByVal aTitle As String)
        If aForm.Visible Then
            CloseGenWindow(aForm)
            Return
        End If

        ' Create, assigned parameters, set title and show modeless
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
        MessageBox.Show("Connection to device lost.", Text, MessageBoxButtons.OK, MessageBoxIcon.Information)
        StopStreaming()
        Disconnect()
    End Sub

    ' Direct device disconnect handler. Just jump back to main UI thread.
    Private Sub OnLinkDisconnected(ByVal aDevice As PvDevice)
        BeginInvoke(mDisconnectedHandler)
    End Sub

    ' GenICam parameter invalidation event, registered for all parameters
    Private Sub OnParameterChanged(ByVal aParameter As PvGenParameter)
        Dim lName As String = aParameter.Name
        If lName = "AcquisitionMode" Then
            ' Have main UI thread update the acquisition mode combo box status
            BeginInvoke(mAcquisitionModeChangedHandler)
        End If
    End Sub

    ' Acquisition mode event handler in main thread.
    Private Sub OnAcquisitionModeChanged()
        ' Get parameter
        Dim lParameter As PvGenEnum = mDevice.GenParameters.GetEnum("AcquisitionMode")

        ' Update value: find which matches in the combo box
        Dim lValue As String = lParameter.ValueString
        For Each lString As String In modeComboBox.Items
            If lValue = lString Then
                modeComboBox.SelectedValue = modeComboBox.Items.ToString()
            End If
        Next
    End Sub

    ' Direct acquisition state changed handler. Bring back to main UI thread.
    Private Sub OnAcquisitionStateChanged(ByVal aDevice As PvDevice, ByVal aStream As PvStreamBase, ByVal aSource As UInt32, ByVal aState As PvAcquisitionState)
        'Invoke event in main UI thread
        BeginInvoke(mAcquisitionStateChangedHandler)
    End Sub

    'OnAcquisitionStateChanged main UI thread counterpart. Syncs acquisition control status
    Private Sub OnAcquisitionStateChanged()
        SetAcquisitionControl()
    End Sub
#End Region

#Region "Windows Events"
    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' Just syncs the enabled state of the UI
        SetGUIState()
    End Sub

    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles connectButton.Click
        ' create a device finder form and open the select device dialog
        Dim lFinder As PvDeviceFinderForm = New PvDeviceFinderForm()
        Try
            ' Show device finder
            If ((lFinder.ShowDialog() <> DialogResult.OK) OrElse (IsNothing(lFinder.Selected) = True)) Then
                Return
            End If

            'Display hourglass
            Dim lPrevCursor As Cursor = Cursor
            Cursor = Cursors.WaitCursor

            ' Connect to device
            Connect(lFinder.Selected)

            ' Restore cursor
            Cursor = lPrevCursor
        Catch lEx As Exception
            MessageBox.Show(lEx.Message)
            Return
        End Try

    End Sub

    Private Sub btnDisconnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles disconnectButton.Click
        ' Show hourglass
        Dim lPrevCursor As Cursor = Cursor
        Cursor = Cursors.WaitCursor

        ' Disconnect from device
        Disconnect()

        ' Restore cursor
        Cursor = lPrevCursor
    End Sub

    Private Sub btnPlay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles playButton.Click
        StartAcquisition()
    End Sub

    Private Sub btnStop_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles stopButton.Click
        StopAcquisition()
    End Sub

    Private Sub btnCommunication_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles communicationButton.Click
        ShowGenWindow(mCommBrowser, mDevice.GenLink, "Communication Control")
    End Sub

    Private Sub btnDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles deviceButton.Click
        ShowGenWindow(mDeviceBrowser, mDevice.GenParameters, "Device Control")
    End Sub

    Private Sub btnStream_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles streamButton.Click
        ShowGenWindow(mStreamBrowser, mStream.Parameters, "Image Stream Control")
    End Sub

    Private Sub cbxMode_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles modeComboBox.SelectedIndexChanged
        'If nothing selected, do nothing
        If modeComboBox.SelectedIndex < 0 Then
            Return
        End If

        Dim lMode As PvGenEnum = mDevice.GenParameters.GetEnum("AcquisitionMode")
        ' Take selection, propagate to parameter
        Dim lSelected As String = CType(modeComboBox.SelectedItem, String)
        lMode.ValueString = modeComboBox.SelectedItem.ToString()
    End Sub

    Private Sub MainForm_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Disconnect()
    End Sub
#End Region

End Class
