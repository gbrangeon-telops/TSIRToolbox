Imports System.Threading
Imports System.Text
Public Class MainForm

    Const cWidthResolution As Integer = 640
    Const cHeightResolution As Integer = 480
    Const cBufferCount As Integer = 4
    Const cFPS As Integer = 0
    Const cPacketSize As Integer = 1440
    Const cDestinationPortAddress As String = "1042"
    Const cDestinationIPAddress As String = "239.192.1.1"

    Public Sub New()
        ' This call is required by the designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.      
        InitializeParameters()
        timer.Interval = 1 * 10000 ' Timer tick every 1 second
        timer.Enabled = True
        timer.Start()
        transmitStatusLabel.BorderStyle = BorderStyle.FixedSingle
        pixelTypeTexBox.ReadOnly = True
    End Sub

    Private mPixelTypeMap As New Dictionary(Of String, PvPixelType)
    Private mTransmitterRaw As New PvTransmitterRaw()
    Private mDevice As New PvVirtualDevice()
    Private mThread As Thread = Nothing
    Private mStopTransmit As Boolean = False
    Private mEmitTestPattern As Boolean = True
    Private mPixelType As PvPixelType = PvPixelType.Mono8

    ' To initialize the default parameters.
    Private Sub InitializeParameters()
        Dim lPvSystem As New PvSystem
        ' Find NIC in the sytem
        lPvSystem.Find()

        sourceAddressComboBox.Items.Clear()
        For i As UInteger = 0 To lPvSystem.InterfaceCount - 1
            If lPvSystem.GetInterface(i).IPAddress.CompareTo("0.0.0.0") <> 0 Then
                ' Add found IP address of the NIC to sourceAddressComboBox
                sourceAddressComboBox.Items.Add(lPvSystem.GetInterface(i).IPAddress)
            End If
        Next

        sourceAddressComboBox.SelectedIndex = 0
        destinationPortTextBox.Text = cDestinationPortAddress
        destinationAddressTextBox.Text = cDestinationIPAddress
        sourcePortTextBox.Text = "0"
        widthTextBox.Text = cWidthResolution.ToString()
        heightTextBox.Text = cHeightResolution.ToString()
        fpsTextBox.Text = cFPS.ToString()
        packetSizeTextBox.Text = cPacketSize.ToString()
        poolSizeTextBox.Text = cBufferCount.ToString()

    End Sub
    ' To Start transmitData
    Private Function TransmitData() As Boolean
        Dim lResult As Boolean = True
        Dim lPoolSize As UInteger = Convert.ToUInt32(poolSizeTextBox.Text)
        Dim lWidth As UInteger = Convert.ToUInt32(widthTextBox.Text)
        Dim lHeight As UInteger = Convert.ToUInt32(heightTextBox.Text)
        Dim lPacketSize As UInteger = Convert.ToUInt32(packetSizeTextBox.Text)
        Dim lFPS As UInteger = Convert.ToUInt32(fpsTextBox.Text)
        Dim lSize As UInteger = lWidth * lHeight * PvImage.GetPixelBitCount(mPixelType) / 8

        Dim lPvBufferList As New List(Of PvBuffer)

        If lFPS > 0 Then
            Dim lPayLoadSize As Single = lWidth * lHeight * PvImage.GetPixelBitCount(mPixelType) * lFPS
            mTransmitterRaw.MaxPayloadThroughput = lPayLoadSize
        End If

        ' Initializes list of PvBuffer
        For i As UInteger = 0 To lPoolSize
            lPvBufferList.Add(New PvBuffer)
            lPvBufferList.Item(i).Image.Alloc(lWidth, lHeight, mPixelType)
        Next

        Try
            ' Start responding to GVCP traffic on the specified NIC.
            mDevice.StartListening(sourceAddressComboBox.Text)

            ' Sets packet size
            mTransmitterRaw.PacketSize = lPacketSize

            ' Loads free buffers into the pool
            mTransmitterRaw.LoadBufferPool(lPvBufferList)
            lPvBufferList.Clear()

            ' Initializes a socket to begin transmitting data.
            mTransmitterRaw.Open(destinationAddressTextBox.Text, Convert.ToUInt16(destinationPortTextBox.Text), sourceAddressComboBox.Text, Convert.ToUInt16(sourcePortTextBox.Text))

            mThread = New Thread(New ParameterizedThreadStart(AddressOf DoThreadWork))
            Dim lParameters() As Object = {lSize}

            'Signal to start transmit data.
            mStopTransmit = False
            mThread.Start(lParameters)

        Catch lPvExc As PvException
            MessageBox.Show(lPvExc.Message)
            lResult = False
        End Try

        Return lResult
    End Function

    ' To retrieve and  queue a buffer into mTransmitterRaw. It is called when mThread is started in method TransmitData().
    ' aParameters - Object passed to the subroutine when the mThread is started.
    Private Sub DoThreadWork(ByVal aParameters As Object)
        Dim lParameters() As Object = aParameters
        Dim lSize As UInteger = CUInt(lParameters(0))
        Dim lPvBuffer As PvBuffer = Nothing
        Dim lSeed As Byte = 0

        While mStopTransmit <> True
            Try
                ' Retrieve a buffer from a list of available buffers
                If mTransmitterRaw.RetrieveFreeBuffer(lPvBuffer).IsOK Then '
                    ' Generates the test pattern. 
                    lPvBuffer.Image.GenerateTestPattern(lSeed)
                    lSeed += 1

                    ' Process the image in the PvBuffer before display it.  
                    ' ....
                    ' Displays the image.
                    displayControl.Display(lPvBuffer)

                    ' Queue a buffer in to be transmitted.
                    mTransmitterRaw.QueueBuffer(lPvBuffer)

                    If (lSeed >= 255) Then
                        lSeed = 0
                    End If
                End If
            Catch lPvExc As PvException
                MessageBox.Show(lPvExc.Message)
            End Try

        End While
    End Sub


    ' To gather and validate all the parameters
    Private Function GatherParameters() As Boolean
        Dim lResult As Boolean = True
        Dim lValue As UInt32 = 0
        Try
            lValue = Convert.ToUInt32(destinationPortTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Source port should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(sourcePortTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Source port should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(widthTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Width should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(heightTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Height should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(fpsTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Frame per second should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(packetSizeTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Packet size should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            lValue = Convert.ToUInt32(packetSizeTextBox.Text)
            If lValue < 0 Then
                MessageBox.Show("Packet size should be a number and greater than 0!", "PvTransmitTestPatternSample")
                Return False
            End If
        Catch lExc As Exception
            MessageBox.Show(lExc.Message)
            Return False
        End Try

        Try
            PvNetworkUtils.IPStrToDWORD(sourceAddressComboBox.Text)
        Catch lExc As Exception
            MessageBox.Show("Invalid source IP address!", "PvTransmitTestPatternSample")
            Return False
        End Try

        Try
            PvNetworkUtils.IPStrToDWORD(destinationAddressTextBox.Text)
        Catch lExc As Exception
            MessageBox.Show("Invalid destination IP address!", "PvTransmitTestPatternSample")
            Return False
        End Try

        Return lResult
    End Function

    Private Sub resetOptionsButton_Click(sender As System.Object, e As System.EventArgs) Handles resetOptionsButton.Click
        Me.Cursor = Cursors.WaitCursor
        InitializeParameters()
        Me.Cursor = Cursors.Default
    End Sub

    Private Sub playButton_Click(sender As System.Object, e As System.EventArgs) Handles playButton.Click
        If GatherParameters() = True Then
            ' Start transmit data.
            If TransmitData() = True Then
                playButton.Enabled = False
                stopButton.Enabled = True
                optionsGroupBox.Enabled = False
            End If
        End If
    End Sub

    Private Sub stopButton_Click(sender As System.Object, e As System.EventArgs) Handles stopButton.Click
        mStopTransmit = True
        If mThread.IsAlive Then
            ' Blocks the calling thread untils the thread terminated
            mThread.Join()
        End If

        mThread = Nothing
        Dim lPvBuffer As PvBuffer = Nothing
        Try
            ' Aborts all pending buffers.
            mTransmitterRaw.AbortQueuedBuffers()

            ' Retrieve the free buffer and release it.
            While (mTransmitterRaw.RetrieveFreeBuffer(lPvBuffer, 0).IsOK)
                lPvBuffer = Nothing
            End While

            ' Close the transmitter
            mTransmitterRaw.Close()

            ' Stop responding to GVCP traffic and close the connection..
            mDevice.StopListening()

        Catch lPvExc As PvException
            MessageBox.Show(lPvExc.Message)
            Return
        End Try
        playButton.Enabled = True
        stopButton.Enabled = False
        optionsGroupBox.Enabled = True
    End Sub

    Private Sub Timer1_Tick(sender As System.Object, e As System.EventArgs) Handles timer.Tick
        Dim lTransmitStatus As New StringBuilder()

        lTransmitStatus.AppendFormat("Transmitted {0, 0:F1} blocks at average rate of {1, 0:F1} FPS. Instantaneous rate of {2, 0:F1} FPS.{3}Average bandwidth is {4, 0:F1} Mb/s. Instantaneous bandwidth is {4, 0:F1} Mb/s.", mTransmitterRaw.BlocksTransmitted, mTransmitterRaw.AverageTransmissionRate, mTransmitterRaw.InstantaneousTransmissionRate, vbCrLf, mTransmitterRaw.AveragePayloadThroughput / 1000000.0, mTransmitterRaw.InstantaneousPayloadThroughput / 1000000.0, Environment.NewLine)
        transmitStatusLabel.Text = lTransmitStatus.ToString()
    End Sub

    Private Sub ExitToolStripMenuItem_Click_1(sender As System.Object, e As System.EventArgs) Handles ExitToolStripMenuItem.Click
        Close()
    End Sub

End Class
