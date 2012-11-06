/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Teardrop
{
    public partial class OptionsControl : DeveloperControlPanel.Plugin.OptionsControl
    {
        Options m_opts = null;

        public OptionsControl(Options opts)
        {
            InitializeComponent();
            m_opts = opts;

            txtDataRoot.TextChanged += new EventHandler(txtDataRoot_TextChanged);
            txtOutputRoot.TextChanged += new EventHandler(txtOutputRoot_TextChanged);
            txtToolDir.TextChanged += new EventHandler(txtToolDir_TextChanged);

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

        public override void OnApplyOptions()
        {
            m_opts.m_dataPath = txtDataRoot.Text;
            m_opts.m_outputPath = txtOutputRoot.Text;
            m_opts.m_toolPath = txtToolDir.Text;
            m_opts.save();
        }

        private bool verifyPath(string p)
        {
            return System.IO.Directory.Exists(p);
        }

        public override void OnCancelOptions()
        {
        }

        public override bool BeforeApplyOptions()
        {
            return true;
        }

        public override bool BeforeCloseOptions()
        {
            if (!verifyPath(txtDataRoot.Text))
            {
                MessageBox.Show("You must provide a valid source data root path");
                return false;
            }

            if (!verifyPath(txtOutputRoot.Text))
            {
                MessageBox.Show("You must provide a valid output data root path");
                return false;
            }

            if (!verifyPath(txtToolDir.Text))
            {
                MessageBox.Show("You must provide a valid tool bin path");
                return false;
            }

            return true;
        }

        public override void OnCloseOptions()
        {
            OnApplyOptions();
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

        void setApplyState()
        {
            m_bDirty = !(
                m_opts.m_dataPath == txtDataRoot.Text
            && m_opts.m_outputPath == txtOutputRoot.Text
            && m_opts.m_toolPath == txtToolDir.Text);

            RaiseOptionsChanged(this);
        }
    }
}
