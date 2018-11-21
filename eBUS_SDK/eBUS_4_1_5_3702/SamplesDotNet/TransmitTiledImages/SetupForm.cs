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
using PvGUIDotNet;

namespace TransmitTiledImages
{
    /// <summary>
    /// Connection setup form.
    /// </summary>
    public partial class SetupForm : Form
    {
        private delegate void UpdateRadioButtonDelegate(ref RadioButton aRadioButton, bool aChecked);
        private delegate void UpdateTextBoxDelegate(ref TextBox aTextBox, string aNewText);

#region Constructors
        /// <summary>
        /// Constructors
        /// </summary>
        public SetupForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="aSetup">Setup classs contained the setup informations.</param>
        public SetupForm(ref Setup aSetup)
        {
            InitializeComponent();
            mSetup = aSetup;
            
            // Role
            switch (mSetup.Role)
            {
                case Setup.cRoleCtrlData:
                    UpdateRadioButton(ref controlDataReceiverRadioButton, true);
                    break;

                case Setup.cRoleData:
                    UpdateRadioButton(ref dataReceiverRadioButton, true);
                    break;

                default:
                    break;
            }

            // Destination
            switch (mSetup.Destination)
            {
                case Setup.cRoleCtrlData:
                    UpdateRadioButton(ref controlDataReceiverRadioButton, true);
                    break;

                case Setup.cRoleData:
                    UpdateRadioButton(ref dataReceiverRadioButton, true);
                    break;

                default:
                    break;
            }

            switch (mSetup.Destination)
            {
                case Setup.cDestinationUnicastAuto:
                    UpdateTextBox(ref unicastSpecificPortTextBox, "");
                    UpdateTextBox(ref multicastIPAddressTextBox, " 0 . 0 . 0 . 0 ");
                    UpdateTextBox(ref multicastPortTextBox, "");
                    break;
                case Setup.cDestinationUnicastSpecific:
                    UpdateTextBox(ref unicastSpecificPortTextBox, mSetup.Port.ToString());
                    UpdateTextBox(ref multicastIPAddressTextBox, " 0 . 0 . 0 . 0 ");
                    UpdateTextBox(ref multicastPortTextBox, "");
                    break;
                case Setup.cDestinationMulticast:   
                    UpdateTextBox(ref unicastSpecificPortTextBox, "");
                    multicastIPAddressTextBox.Text = mSetup.IPAddress;
                    UpdateTextBox(ref multicastIPAddressTextBox, mSetup.IPAddress);
                    UpdateTextBox(ref multicastPortTextBox, mSetup.Port.ToString());
                    break;

                default:
                    break;
            }
            
        }
#endregion

#region Data member
        Setup mSetup = null;
#endregion

        

#region Utilities methods
        /// <summary>
        /// Update radio button.
        /// </summary>
        /// <param name="aRadioButton">Radio button to be updated.</param>
        /// <param name="aChecked">To check or uncheck radio button.</param>
        private void UpdateRadioButton(ref RadioButton aRadioButton, bool aChecked)
        {
            if (aRadioButton.InvokeRequired == true)
            {
                UpdateRadioButtonDelegate lUpdateRadioButton = new UpdateRadioButtonDelegate(UpdateRadioButton);
                aRadioButton.Invoke(lUpdateRadioButton, aChecked);
            }
            else
            {
                aRadioButton.Checked = aChecked;
            }
        }

        /// <summary>
        /// To update the text box.
        /// </summary>
        /// <param name="aTextBox">Text box to be updated.</param>
        /// <param name="aNewText">New text to update the text box's text.</param>
        private void UpdateTextBox(ref TextBox aTextBox, string aNewText)
        {
            if (aTextBox.InvokeRequired == true)
            {
                UpdateTextBoxDelegate lUpdateTextBox = new UpdateTextBoxDelegate(UpdateTextBox);
                aTextBox.Invoke(lUpdateTextBox, aNewText);
            }
            else
            {
                aTextBox.Text = aNewText;
            }
        }

        /// <summary>
        /// Enables the GUI controls.
        /// </summary>
        private void EnableInterface()
        {
            if (unicastAutomaticRadioButton.Checked == true)
            {
                unicastSpecificPortTextBox.Enabled = false;
                multicastPortTextBox.Enabled = false;
                multicastIPAddressTextBox.Enabled = false;
            }
            else if (unicastSpecificRadioButton.Checked == true)
            {
                unicastSpecificPortTextBox.Enabled = true;
                multicastPortTextBox.Enabled = false;
                multicastIPAddressTextBox.Enabled = false;
            }
            else if (multicastRadioButton.Checked == true)
            {
                unicastSpecificPortTextBox.Enabled = false;
                multicastPortTextBox.Enabled = true;
                multicastIPAddressTextBox.Enabled = true;
            }
        }
#endregion


#region Windows events
        private void okButton_Click(object sender, EventArgs e)
        {            
            // Invalidate the results
            mSetup.Reset();

            if (controlDataReceiverRadioButton.Checked == true)
            {
                mSetup.Role = Setup.cRoleCtrlData;
            }
            else if (dataReceiverRadioButton.Checked == true)
            {
                mSetup.Role = Setup.cRoleData;
            }
           
            if (unicastAutomaticRadioButton.Checked == true)
            {
               mSetup.Destination = Setup.cDestinationUnicastAuto;
            }
            else if (unicastSpecificRadioButton.Checked == true)
            {
                mSetup.Destination = Setup.cDestinationUnicastSpecific;
                mSetup.Port = Convert.ToUInt16(unicastSpecificPortTextBox.Text);
            }
            else if (multicastRadioButton.Checked == true)
            {
                try
                {
                    PvNetworkUtils.IPStrToDWORD(multicastIPAddressTextBox.Text);
                }
                catch (Exception)
                {
                    MessageBox.Show("Invalid multicast IP address!", "TransmitTiledImages");
                    return;
                }
                mSetup.Destination = Setup.cDestinationMulticast;
                mSetup.IPAddress = multicastIPAddressTextBox.Text;
                mSetup.Port = Convert.ToUInt16(multicastPortTextBox.Text);
            }
            DialogResult = DialogResult.OK;
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void unicastAutomaticRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            EnableInterface();
        }

        private void unicastSpecificRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            EnableInterface();
        }

        private void multicastRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            EnableInterface();
        }

        private void SetupForm_Load(object sender, EventArgs e)
        {
            // Role
            switch (mSetup.Role)
            {
                case Setup.cRoleCtrlData:
                    controlDataReceiverRadioButton.Checked = true;
                    break;

                case Setup.cRoleData:
                    dataReceiverRadioButton.Checked = true;
                    break;

                default:
                    break;
            }

            // Destination
            switch (mSetup.Destination)
            {
                case Setup.cRoleCtrlData:
                    controlDataReceiverRadioButton.Checked = true;
                    break;

                case Setup.cRoleData:
                    dataReceiverRadioButton.Checked = true;
                    break;

                default:
                    break;
            }

            switch (mSetup.Destination)
            {
                case Setup.cDestinationUnicastAuto:
                    unicastSpecificPortTextBox.Text = "";
                    multicastIPAddressTextBox.Text = " 0 . 0 . 0 . 0 ";
                    multicastPortTextBox.Text = "";
                    break;
                case Setup.cDestinationUnicastSpecific:
                    unicastSpecificPortTextBox.Text = mSetup.Port.ToString();
                    multicastIPAddressTextBox.Text = " 0 . 0 . 0 . 0 ";
                    multicastPortTextBox.Text = "";
                    break;
                case Setup.cDestinationMulticast:
                    unicastSpecificPortTextBox.Text = "";
                    multicastIPAddressTextBox.Text = mSetup.IPAddress;
                    multicastPortTextBox.Text = mSetup.Port.ToString();
                    break;

                default:
                    break;
            }
        }
#endregion       
    }
}
