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
    public partial class Main : Form
    {
        bool m_bIsPaused = false;

        public delegate void PauseButtonClicked(bool newState);
        public event PauseButtonClicked OnPauseButtonClicked;

        public FolderTreeView Results
        {
            get
            {
                return tvwResults;
            }
        }

        public LogWriter OutputLog
        {
            get
            {
                return txtOutput;
            }
        }

        public Main()
        {
            InitializeComponent();
        }

        public void OnJobCreated(JobStatus job)
        {
            FolderTreeNode node = tvwResults.CreateFileNode(job.Path);
            node.Tag = job;
            job.Tag = node;

            // This is a bit of a hack just to optimize the speed at which this happens...
            node.ForeColor = Color.Gray;
            
            while( node.Parent != null )
            {
                node = (FolderTreeNode)node.Parent;
                JobStatus status = node.Tag as CoS.JobStatus;
                if (status != null && status.Status == JobStatus.State.RUNNING)
                    continue;

                node.ForeColor = Color.Gray;
            }
        }

        public void OnJobStatusChanged(JobStatus job)
        {
            FolderTreeNode node = job.Tag as FolderTreeNode;
            if (job.Status != JobStatus.State.UNKNOWN_PATH && job.Status != JobStatus.State.UNKNOWN_TYPE)
            {
                updateNodeColor(node, job.Status);
                updateParentHierarchy(job);
            }
        }

        void updateNodeColor(FolderTreeNode node, JobStatus.State jobState)
        {
            if (node == null)
            {
                return;
            }

            switch (jobState)
            {
                case JobStatus.State.PENDING:
                    node.ForeColor = Color.Gray;
                    break;
                case JobStatus.State.RUNNING:
                    node.ForeColor = Color.Orange;
                    break;
                case JobStatus.State.FAILED:
                    node.ForeColor = Color.Red;
                    break;
                case JobStatus.State.SUCCEEDED:
                    node.ForeColor = Color.Green;
                    break;
            }
        }

        JobStatus.State getStatusFromAllBelow(FolderTreeNode currentNode)
        {
            if (currentNode == null)
                return JobStatus.State.RUNNING;
            int countPending = 0;
            int countRunning = 0;
            foreach( FolderTreeNode node in currentNode.Nodes )
            {
                JobStatus.State myStatus;
                if (node.Nodes.Count > 0)
                {
                    myStatus = getStatusFromAllBelow(node);
                }
                else
                {
                    JobStatus status = node.Tag as CoS.JobStatus;
                    if (status == null)
                        return JobStatus.State.RUNNING;

                    myStatus = status.Status;
                }
                if (myStatus == JobStatus.State.FAILED)
                    return JobStatus.State.FAILED;
                if( myStatus == JobStatus.State.PENDING )
                    countPending += 1;
                if( myStatus == JobStatus.State.RUNNING )
                    countRunning += 1;
            }
            if( countRunning > 0 )
                return JobStatus.State.RUNNING;
            if( countPending > 0 )
                return JobStatus.State.PENDING;
            return JobStatus.State.SUCCEEDED;
        }

        void updateParentHierarchy(JobStatus job)
        {
            FolderTreeNode currentNode = job.Tag as FolderTreeNode;
            while( currentNode != null && currentNode.Parent != null )
            {
                currentNode = (FolderTreeNode)currentNode.Parent;
                if (job.Status == JobStatus.State.SUCCEEDED)
                {
                    updateNodeColor(currentNode, getStatusFromAllBelow(currentNode));
                }
                else
                {
                    updateNodeColor(currentNode, job.Status);
                }
            }
        }

        public void ShowMessageBox(string message, string caption)
        {
            MessageBox.Show(this, message, caption);
        }

        public bool ShowOptionsForm(Options opts)
        {
            dlgOptions dlg = new dlgOptions(opts);
            return (dlg.ShowDialog(this) == DialogResult.OK);
        }

        /// <summary>
        /// Clears all open log windows
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void clearLogsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //txtError.Clear();
            txtOutput.Clear();
        }

        /// <summary>
        /// Rebuild all defined targets (from main form)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void allToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// the user would like to suspend operation for a bit
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdPause_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            if (item == null)
            {
                return;
            }

            // flip the polarity on our flag and set the text to match
            m_bIsPaused = !m_bIsPaused;

            if (m_bIsPaused)
            {
                cmdPause.Text = "Resume";
                cmdPause.Text = "Resume";
            }
            else
            {
                cmdPause.Text = "Pause";
                cmdPause.Text = "Pause";
            }

            // notify listeners
            if (OnPauseButtonClicked != null)
            {
                OnPauseButtonClicked(m_bIsPaused);
            }
        }

        private void pauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cmdPause_Click(sender, e);
        }

        private void tvwResults_AfterSelect(object sender, TreeViewEventArgs e)
        {
            txtOutput.Clear();

            if (e.Node == null)
            {
                return;
            }

            if (e.Action == TreeViewAction.ByKeyboard ||
                e.Action == TreeViewAction.ByMouse)
            {
                JobStatus status = e.Node.Tag as JobStatus;
                if (status != null)
                {
                    txtOutput.AppendMessage(status.Output);
                }
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                Hide();
                e.Cancel = true;
            }
        }
    }
}