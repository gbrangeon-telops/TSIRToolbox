' *****************************************************************************
'
'     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
'
' *****************************************************************************

Public Class MainForm

    Private mDevice As PvDevice = New PvDevice
    Private mStream As PvStream = New PvStream
    Private mPipeline As PvPipeline = Nothing

    Private mThread As Thread = Nothing
    Private mIsStopping As Boolean = False
    Private mStep As Integer = 1

    ' Form Load event handler. Starts the state machine.
    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        timer.Start()

    End Sub

    ' Form Closing event handler. If called when still streaming or with the device connected,
    ' takes expected measures to gracefully terminate the application.
    Private Sub MainForm_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing

        If mPipeline.IsStarted Then
            Step5StoppingStream()
        End If

        If mDevice.IsConnected Then
            Step6Disconnecting()
        End If

    End Sub

    ' Stop button Click handler. Resumes the state machine.
    Private Sub stopButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles stopButton.Click

        mStep += 1
        timer.Start()

    End Sub

    ' State machine Step 1
    ' Connecting:
    ' 1. User selects the device
    ' 2. Device is connected
    ' 3. Stream is opened
    ' 4. Pipeline finally created (requires opened stream)
    Private Sub Step1Connecting()

        oneLabel.Enabled = True

        ' Pop device finder, let user select a device
        Dim lForm As PvDeviceFinderForm = New PvDeviceFinderForm
        If lForm.ShowDialog() = DialogResult.OK Then

            Try

                ' Connect device
                mDevice.Connect(lForm.Selected)

                ' Open stream
                mStream.Open(lForm.Selected.IPAddress)

                ' Set stream parameters/statistics on browser
                browser.GenParameterArray = mStream.Parameters

                ' Create pipeline
                mPipeline = New PvPipeline(mStream)

            Catch ex As PvException

                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
                Close()

            End Try

        Else

            Close()

        End If

    End Sub

    ' State machine step 2
    ' Configuring:
    ' 1. Negotiating ideal packet size
    ' 2. Setting stream destination on the device (to our PvStream)
    ' 3. Read payload size of the device
    ' 4. Configure pipeline: buffer size and buffer count
    Private Sub Step2Configuring()

        oneLabel.Enabled = False
        twoLabel.Enabled = True

        Try

            ' Negotiate packet size. Failure means default value as configured on 
            ' the device is used.
            mDevice.NegotiatePacketSize()

        Catch ex As PvException

            MessageBox.Show(ex.Message, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning)

        End Try

        ' Set stream destination
        mDevice.SetStreamDestination(mStream.LocalIPAddress, mStream.LocalPort)

        ' Read payload size, pre-allocate buffers of the pipeline
        Dim lPayloadSize As Int64 = mDevice.GenParameters.GetIntegerValue("PayloadSize")
        mPipeline.BufferSize = CType(lPayloadSize, UInt32)

        ' Set buffer count. Use more buffers (at expense of using more memory) to 
        ' eleminate missing block IDs
        mPipeline.BufferCount = 16

    End Sub

    ' State machine step 3
    ' Starting stream
    ' 1. Start the pipeline
    ' 2. Start the display thread
    ' 3. Set TLParamsLocked parameter of the device to 1
    ' 4. Execute the AcquisitionStart command on the device
    Private Sub Step3StartingStream()

        twoLabel.Enabled = False
        threeLabel.Enabled = True

        ' Start (arm) the pipeline
        mPipeline.Start()

        ' Start display thread
        mThread = New Thread(AddressOf Me.ThreadProc)
        Dim lParameters As Object() = {Me}
        mIsStopping = False
        mThread.Start(lParameters)

        ' Set TLParamsLocked to 1
        mDevice.GenParameters.SetIntegerValue("TLParamsLocked", 1)

        ' Start acquisition on the device
        mDevice.GenParameters.ExecuteCommand("AcquisitionStart")

    End Sub

    ' State machine step 4
    ' Streaming:
    ' Here everything is handled through the display thread to free-up the 
    ' UI and keep the application responsive. State machine resumes when
    ' the Stop button is clicked.
    Private Sub Step4Streaming()

        threeLabel.Enabled = False
        fourLabel.Enabled = True
        stopButton.Enabled = True

    End Sub

    ' State machine step 5
    ' Stopping stream:
    ' 1. Execute the AcquisitionStop command on the device
    ' 2. Release TLParamsLocked parameter on the device (set it to 0)
    ' 3. Stop the display thread
    ' 4. Stop the pipeline
    Private Sub Step5StoppingStream()

        stopButton.Enabled = False
        fourLabel.Enabled = False
        fiveLabel.Enabled = True

        ' Stop acquisition
        mDevice.GenParameters.ExecuteCommand("AcquisitionStop")

        ' Release TLParamsLocked
        mDevice.GenParameters.SetIntegerValue("TLParamsLocked", 0)

        ' Stop display thread
        mIsStopping = True
        mThread.Join()
        mThread = Nothing

        ' Stop the pipeline
        mPipeline.Stop()

    End Sub

    ' State machine step 6
    ' Disconnecting
    ' 1. Closing stream
    ' 2. Disconnect device
    Private Sub Step6Disconnecting()

        fiveLabel.Enabled = False
        sixLabel.Enabled = True

        ' Release browser reference on stream parameters
        browser.GenParameterArray = Nothing

        ' Close stream
        mStream.Close()

        ' Disconnect device
        mDevice.Disconnect()

    End Sub

    ' Display thread:
    ' Until requested to exit, simply pumps buffers out of the pipeline
    ' and sends them to the display.
    Private Sub ThreadProc(ByVal aParameters As Object)

        Dim lParameters As Object() = CType(aParameters, Object())
        Dim lMe As MainForm = CType(lParameters(0), MainForm)

        Dim lBuffer As PvBuffer = Nothing

        Do

            If lMe.mIsStopping Then

                ' Signaled to terminate thread, return
                Return

            End If

            ' Retrieve next buffer from acquisition pipeline
            Dim lResult As PvResult = lMe.mPipeline.RetrieveNextBuffer(lBuffer)
            If lResult.IsOK Then

                ' Operation result of buffer is OK, display
                If lBuffer.OperationResult.IsOK Then

                    ' //////////////////////////////////////////////////////////////
                    ' Typically do your image processing/handling here
                    ' //////////////////////////////////////////////////////////////

                    ' Display image
                    lMe.displayControl.Display(lBuffer)

                End If

                ' We got a buffer (good or not) we must release it back
                lMe.mPipeline.ReleaseBuffer(lBuffer)

            End If

        Loop

    End Sub

    ' Timer Tick hanlder. Drives the state machine forward. A timer-based
    Private Sub timer_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles timer.Tick

        timer.Stop()
        Select Case mStep

            Case 1

                Step1Connecting()

            Case 2

                Step2Configuring()

            Case 3

                Step3StartingStream()

            Case 4

                ' Don't let state machine move up to 5, Stop button will do that
                Step4Streaming()
                Return

            Case 5

                Step5StoppingStream()

            Case 6

                Step6Disconnecting()

            Case Else

                ' End of line, no need to let state machine go up any more
                Close()
                Return

        End Select


        mStep += 1
        timer.Start()

    End Sub

End Class
