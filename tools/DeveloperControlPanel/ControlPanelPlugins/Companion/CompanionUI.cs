/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CoS
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