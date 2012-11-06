/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Teardrop
{
    public partial class CompanionUI : Form
    {
        public event EventHandler<EventArgs> Reconnect;

        public CompanionUI()
        {
            InitializeComponent();
            FormClosing += new FormClosingEventHandler(OnFormClosing);
        }

        void OnFormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                Hide();
                e.Cancel = true;
            }
        }

        // temp for now
        public string FPS
        {
            get
            {
                return txtFPS.Text;
            }
            set
            {
                txtFPS.Text = value;
            }
        }
        public string Tris
        {
            get
            {
                return txtTris.Text;
            }
            set
            {
                txtTris.Text = value;
            }
        }
        public string Verts
        {
            get
            {
                return txtVerts.Text;
            }
            set
            {
                txtVerts.Text = value;
            }
        }
        public string Batches
        {
            get
            {
                return txtBatches.Text;
            }
            set
            {
                txtBatches.Text = value;
            }
        }

        public void beginUpdate()
        {
            lvwMemory.BeginUpdate();
        }

        public void endUpdate()
        {
            lvwMemory.EndUpdate();
        }

        private void reconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Reconnect != null)
            {
                Reconnect(this, new EventArgs());
            }
        }

        public void setAllocatorStats(string name, uint usage, uint highWater, uint hardLimit, uint softLimit)
        {
            ListViewItem item = lvwMemory.Items[name];
            if (item == null)
            {
                item = new ListViewItem();
                item.Name = name;
                item.Text = name;

                item.SubItems.Add(new ListViewItem.ListViewSubItem());
                item.SubItems.Add(new ListViewItem.ListViewSubItem());
                item.SubItems.Add(new ListViewItem.ListViewSubItem());

                lvwMemory.Items.Add(item);
            }

            item.SubItems[1].Text = usage.ToString();
            item.SubItems[2].Text = highWater.ToString();

            // this will be a user-drawn bar graph at some point
            item.SubItems[3].Text = hardLimit.ToString();
        }

        public void Alert(string message)
        {
            MessageBox.Show(message, "Alert");
        }
    }
}