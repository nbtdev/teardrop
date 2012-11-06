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
    public partial class dlgOptions : Form
    {
        Options m_opts = null;

        public dlgOptions(Options opts)
        {
            InitializeComponent();
            m_opts = opts;
            cmdApply.Enabled = false;

            txtDataRoot.Text = m_opts.m_dataPath;
            txtOutputRoot.Text = m_opts.m_outputPath;
            txtToolDir.Text = m_opts.m_toolPath;
        }

        private void cmdBrowseData_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                txtDataRoot.Text = dlg.SelectedPath;
                setApplyState();
            }
        }

        private void cmdBrowseOutput_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                txtOutputRoot.Text = dlg.SelectedPath;
                setApplyState();
            }
        }

        private void cmdBrowseToolDir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                txtToolDir.Text = dlg.SelectedPath;
                setApplyState();
            }
        }

        private void cmdApply_Click(object sender, EventArgs e)
        {
            m_opts.m_dataPath = txtDataRoot.Text;
            m_opts.m_outputPath = txtOutputRoot.Text;
            m_opts.m_toolPath = txtToolDir.Text;
            m_opts.save();
            cmdApply.Enabled = false;
        }

        private bool verifyPath(string p)
        {
            return System.IO.Directory.Exists(p);
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void cmdClose_Click(object sender, EventArgs e)
        {
            if (!verifyPath(txtDataRoot.Text))
            {
                MessageBox.Show("You must provide a valid source data root path");
                return;
            }

            if (!verifyPath(txtOutputRoot.Text))
            {
                MessageBox.Show("You must provide a valid output data root path");
                return;
            }

            if (!verifyPath(txtToolDir.Text))
            {
                MessageBox.Show("You must provide a valid tool bin path");
                return;
            }

            cmdApply_Click(sender, e);
            this.DialogResult = DialogResult.OK;
            Close();
        }

        private void setApplyState()
        {
            cmdApply.Enabled =
                !(txtDataRoot.Text == m_opts.m_dataPath
                && txtOutputRoot.Text == m_opts.m_outputPath
                && txtToolDir.Text == m_opts.m_toolPath);
        }

        private void txtDataRoot_TextChanged(object sender, EventArgs e)
        {
            setApplyState();
        }

        private void txtOutputRoot_TextChanged(object sender, EventArgs e)
        {
            setApplyState();
        }

        private void txtToolDir_TextChanged(object sender, EventArgs e)
        {
            setApplyState();
        }
    }
}