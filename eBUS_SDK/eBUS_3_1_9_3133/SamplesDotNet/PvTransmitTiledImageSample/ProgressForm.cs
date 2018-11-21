// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;


namespace PvTransmitTiledImageSample
{
    /// <summary>
    /// To display the status of the process.
    /// </summary>
    public partial class ProgressForm : Form
    {
#region Delegate
        // Delegate to update text box.
        private delegate void UpdateTextBoxDelegate(string aNewText);
        #endregion
#region Constructors
        /// <summary>
        /// Constructor
        /// </summary>
        public ProgressForm()
        {
            InitializeComponent();
        }
#endregion

#region Datra members
        //Index of bitmap array. 
        private int mImageIndex = 0;
        // Process message.
        private string mMessage = "Building GenICam interface...";
        private Object mProgressObject = new Object();
        private bool mTaskDone = false;
#endregion

#region Properties

        /// <summary>
        /// Message to be displayed on the status text box of the progress form.
        /// </summary>
        public string Message
        {
            get
            {
                lock(mProgressObject)
                {
                    string lValue = mMessage;
                    return lValue;
                }
            }
            set
            {
                lock(mProgressObject)
                {
                    mMessage = value;
                }
            }
        }

        /// Signal that the task has been done..
        /// </summary>
        public void TaskDone()
        {
            mTaskDone = true;          
        }

#endregion

#region Utilities methods
       
        /// Method to response to the delegate UpdateTextDelegate
        /// </summary>
        /// <param name="aNewText">New text to be updated.</param>
        private void UpdateTextBox(string aNewText)
        {

            if (statusLabel.InvokeRequired == true)
            {
                UpdateTextBoxDelegate lUpdateTextBox = new UpdateTextBoxDelegate(UpdateTextBox);
                statusLabel.Invoke(lUpdateTextBox, aNewText);
            }
            else
            {
                statusLabel.Text = aNewText;
            }
        }
        
#endregion        
        
#region Windows events

        private void timer1_Tick(object sender, EventArgs e)
        {
            pictureBox.Image = imageList.Images[mImageIndex];
            mImageIndex++;
            mImageIndex %= imageList.Images.Count;

            lock(mProgressObject)
            {
                if (mMessage != statusLabel.Text)
                {  
                    UpdateTextBox(mMessage);
                }
            }

            if (mTaskDone)
            {
                DialogResult = DialogResult.OK;
                Close();
            }
        }

        private void ProgressForm_Load(object sender, EventArgs e)
        {
            lock(mProgressObject)
            {
                UpdateTextBox(mMessage);
            }
        }

#endregion
        
    }
}
