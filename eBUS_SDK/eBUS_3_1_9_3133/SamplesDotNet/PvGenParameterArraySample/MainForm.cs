// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PvDotNet;
using PvGUIDotNet;


namespace PvGenParameterArraySample
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
        }

        PvDevice mDevice = new PvDevice();

        /// <summary>
        /// Form Load event handler. Connects the device and populates the tree with
        /// the PvGenParameter objects contained in the GenICam interface of the device.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            // Show device finder
            PvDeviceFinderForm lForm = new PvDeviceFinderForm();
            if (lForm.ShowDialog() != DialogResult.OK)
            {
                Close();
                return;
            }

            try
            {
                // Connect device, use device finder selection
                mDevice.Connect(lForm.Selected);
            }
            catch (PvException ex)
            {
                MessageBox.Show(ex.Message, "Error",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);

                Close();
                return;
            }

            // Build the tree
            BuildTree();
        }

        /// <summary>
        /// Form Closign event handler. Disconnects the device.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (mDevice.IsConnected)
            {
                mDevice.Disconnect();
            }
        }

        /// <summary>
        /// Returns and creates if needed the nodes/node corresponding to the
        /// category of a parameter.
        /// </summary>
        /// <param name="aParameter"></param>
        /// <returns></returns>
        private TreeNode GetCategory(PvGenParameter aParameter)
        {
            // We don't want to show the "Root\\" level all categories start with, remove it
            string lCategory = aParameter.Category;
            if (lCategory.IndexOf("Root\\") == 0)
            {
                lCategory = lCategory.Substring("Root\\".Length);
            }

            // Break categories in a list of strings. They are separated by backslashes
            // as in Category\\Example\\Test
            List<String> lCategories = new List<string>();
            int lIndex = lCategory.IndexOf("\\");
            while (lIndex > 0)
            {
                lCategories.Add(lCategory.Substring(0, lIndex));
                lCategory = lCategory.Substring(lIndex + 1);

                lIndex = lCategory.IndexOf("\\");
            }
            lCategories.Add(lCategory);

            // Get (or create as needed) the category node requested
            TreeNode lCurrent = null;
            foreach (string lC in lCategories)
            {
                if (lCurrent == null)
                {
                    // Current is null, root level. Use treeView for Nodes container
                    if (!treeView.Nodes.ContainsKey(lC))
                    {
                        // Not found, create
                        TreeNode lNode = treeView.Nodes.Add(lC, lC);
                        lNode.ImageIndex = 0;
                        lNode.SelectedImageIndex = 1;
                    }
                    // Set current category for next iteration
                    lCurrent = treeView.Nodes[lC];
                }
                else
                {
                    // Use current
                    if (!lCurrent.Nodes.ContainsKey(lC))
                    {
                        // Not found, create
                        TreeNode lNode = lCurrent.Nodes.Add(lC, lC);
                        lNode.ImageIndex = 0;
                        lNode.SelectedImageIndex = 1;
                    }
                    // Set current category for next iteration
                    lCurrent = lCurrent.Nodes[lC];
                }
            }

            // Last current is the last category at the end of the category
            // path, this is what we need to return
            return lCurrent;
        }

        /// <summary>
        /// Builds the GenICam device interface tree.
        /// </summary>
        private void BuildTree()
        {
            // Select GenICam interface of the device, go through all parameters
            PvGenParameterArray lArray = mDevice.GenParameters;
            foreach (PvGenParameter lP in lArray)
            {
                // Create parameter node
                TreeNode lNode = new TreeNode(lP.Name);
                lNode.ImageIndex = 2;
                lNode.SelectedImageIndex = 3;

                // Set tag to parameter, used to reference parameter when sleected
                lNode.Tag = lP;

                // Get category
                TreeNode lCategory = GetCategory(lP);

                // Add parameter node to category
                lCategory.Nodes.Add(lNode);
            }

            // Expand all
            treeView.ExpandAll();

            // Look for first leaf node, select
            TreeNode lCurrent = treeView.Nodes[0];
            for (;;)
            {
                if (lCurrent.Nodes.Count <= 0)
                {
                    treeView.SelectedNode = lCurrent;
                    break;
                }

                lCurrent = lCurrent.Nodes[0];
            }

            // Make sure the first node (leaf or not) is visible (scroll up)
            treeView.Nodes[0].EnsureVisible();
        }

        /// <summary>
        /// AfterSelect event handler for the tree. Updates the parameter data
        /// if a parameter is selected.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // Dynamic cast of tag in PvGenParameter
            PvGenParameter lP = treeView.SelectedNode.Tag as PvGenParameter;
            if (lP == null)
            {
                // Not a PvGenParameter, clear list view and return
                listView1.Items.Clear();
                return;
            }

            // List where all name/value attribute pairs are added
            List<ListViewItem> lList = new List<ListViewItem>();

            // Common properties for all parameters
            FillParameter(lList, lP);

            // Integer
            PvGenInteger lPI = lP as PvGenInteger;
            if (lPI != null)
            {
                FillInteger(lList, lPI);
            }

            // Boolean
            PvGenBoolean lPB = lP as PvGenBoolean;
            if (lPB != null)
            {
                FillBoolean(lList, lPB);
            }

            // Enumeration
            PvGenEnum lPE = lP as PvGenEnum;
            if (lPE != null)
            {
                FillEnum(lList, lPE);
            }
            
            // String
            PvGenString lPS = lP as PvGenString;
            if (lPS != null)
            {
                FillString(lList, lPS);
            }

            // Float
            PvGenFloat lPF = lP as PvGenFloat;
            if (lPF != null)
            {
                FillFloat(lList, lPF);
            }

            // Register
            PvGenRegister lPR = lP as PvGenRegister;
            if (lPR != null)
            {
                FillRegister(lList, lPR);
            }

            // One-shot update of the list of attributes to the list view
            listView1.BeginUpdate();
            listView1.Items.Clear();
            listView1.Items.AddRange(lList.ToArray());
            listView1.EndUpdate();
        }

        /// <summary>
        /// Add common parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillParameter(List<ListViewItem> aList, PvGenParameter aParameter)
        {
            ListViewItem lLVI = new ListViewItem("Name");
            lLVI.SubItems.Add(aParameter.Name);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("DisplayName");
            lLVI.SubItems.Add(aParameter.DisplayName);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Type");
            lLVI.SubItems.Add(aParameter.Type.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Description");
            lLVI.SubItems.Add(aParameter.Description);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("ToolTip");
            lLVI.SubItems.Add(aParameter.ToolTip);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Category");
            lLVI.SubItems.Add(aParameter.Category);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Visibility");
            lLVI.SubItems.Add(aParameter.Visibility.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            // Try catch use as failure has to be expected when reading value
            lLVI = new ListViewItem("Value");
            string lValue = "";
            if (aParameter.IsReadable)
            {
                try
                {
                    lValue = aParameter.ToString();
                }
                catch (PvException ex)
                {
                    // On failure display error message
                    lValue = ex.Message;
                }
            }
            else
            {
                lValue = "{Not readable}";
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsAvailable");
            lLVI.SubItems.Add(aParameter.IsAvailable.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsPersistent");
            lLVI.SubItems.Add(aParameter.IsPersistent.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsReadable");
            lLVI.SubItems.Add(aParameter.IsReadable.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsStreamable");
            lLVI.SubItems.Add(aParameter.IsStreamable.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsValueCached");
            lLVI.SubItems.Add(aParameter.IsValueCached.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsWritable");
            lLVI.SubItems.Add(aParameter.IsWritable.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("IsSelector");
            lLVI.SubItems.Add(aParameter.IsSelector.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            // Display selected parameters on the same line
            lLVI = new ListViewItem("SelectedParameters");
            string lSelectees = "";
            foreach (PvGenParameter lS in aParameter.SelectedParameters)
            {
                lSelectees += lS.Name + " ";
            }
            lLVI.SubItems.Add(lSelectees);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            // Display selectors on the same line
            lLVI = new ListViewItem("SelectingParameters");
            string lSelectors = "";
            foreach (PvGenParameter lS in aParameter.SelectingParameters)
            {
                lSelectors += lS.Name + " ";
            }
            lLVI.SubItems.Add(lSelectors);
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);

            lLVI = new ListViewItem("NameSpace");
            lLVI.SubItems.Add(aParameter.NameSpace.ToString());
            lLVI.Group = listView1.Groups[0];
            aList.Add(lLVI);
        }

        /// <summary>
        /// Add integer parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillInteger(List<ListViewItem> aList, PvGenInteger aParameter)
        {
            // Try catch use as failure has to be expected when reading value
            ListViewItem lLVI = new ListViewItem("Value");
            string lValue = "";
            try
            {
                lValue = aParameter.Value.ToString();
            }
            catch (PvException ex)
            {
                // Display error message
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Max");
            lLVI.SubItems.Add(aParameter.Max.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Min");
            lLVI.SubItems.Add(aParameter.Min.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Increment");
            lLVI.SubItems.Add(aParameter.Increment.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Representation");
            lLVI.SubItems.Add(aParameter.Representation.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);
        }

        /// <summary>
        /// Add Boolean parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillBoolean(List<ListViewItem> aList, PvGenBoolean aParameter)
        {
            // Try catch use as failure has to be expected when reading value
            ListViewItem lLVI = new ListViewItem("Value");
            string lValue = "";
            try
            {
                // Display error message
                lValue = aParameter.Value.ToString();
            }
            catch (PvException ex)
            {
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);
        }

        /// <summary>
        /// Add Enum parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillEnum(List<ListViewItem> aList, PvGenEnum aParameter)
        {
            // Try catch use as failure has to be expected when reading value
            ListViewItem lLVI = new ListViewItem("ValueInt");
            string lValue = "";
            try
            {
                lValue = aParameter.ValueInt.ToString();
            }
            catch (PvException ex)
            {
                // Display error message
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            // Try catch use as failure has to be expected when reading value
            lLVI = new ListViewItem("ValueString");
            lValue = "";
            try
            {
                lValue = aParameter.ValueString;
            }
            catch (PvException ex)
            {
                // Display error message
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("EntriesCount");
            lLVI.SubItems.Add(aParameter.EntriesCount.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            // Display enum entries as string(int)
            int lIndex = 0;
            foreach (PvGenEnumEntry lEE in aParameter)
            {
                lLVI = new ListViewItem("Entry[" + lIndex.ToString() + "]");
                lLVI.SubItems.Add(lEE.ValueString + "(" + lEE.ValueInt.ToString() + ") " +
                    (lEE.IsAvailable ? "" : "NA"));
                lLVI.Group = listView1.Groups[2];
                aList.Add(lLVI);

                lIndex++;
            }
        }

        /// <summary>
        /// Add String parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillString(List<ListViewItem> aList, PvGenString aParameter)
        {
            // Try catch use as failure has to be expected when reading value
            ListViewItem lLVI = new ListViewItem("Value");
            string lValue = "";
            try
            {
                lValue = aParameter.Value;
            }
            catch (PvException ex)
            {
                // Display error message
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("MaxLength");
            lLVI.SubItems.Add(aParameter.MaxLength.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);
        }

        /// <summary>
        /// Add Float parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillFloat(List<ListViewItem> aList, PvGenFloat aParameter)
        {
            // Try catch use as failure has to be expected when reading value
            ListViewItem lLVI = new ListViewItem("Value");
            string lValue = "";
            try
            {
                lValue = aParameter.Value.ToString();
            }
            catch (PvException ex)
            {
                // Display error message
                lValue = ex.Message;
            }
            lLVI.SubItems.Add(lValue);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Max");
            lLVI.SubItems.Add(aParameter.Max.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Min");
            lLVI.SubItems.Add(aParameter.Min.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Unit");
            lLVI.SubItems.Add(aParameter.Unit);
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);

            lLVI = new ListViewItem("Representation");
            lLVI.SubItems.Add(aParameter.Representation.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);
        }

        /// <summary>
        /// Add Register parameter attributes/values to a List used to populate
        /// the list view.
        /// </summary>
        /// <param name="aList"></param>
        /// <param name="aParameter"></param>
        private void FillRegister(List<ListViewItem> aList, PvGenRegister aParameter)
        {
            ListViewItem lLVI = new ListViewItem("Length");
            lLVI.SubItems.Add(aParameter.Length.ToString());
            lLVI.Group = listView1.Groups[1];
            aList.Add(lLVI);
        }
    }
}

