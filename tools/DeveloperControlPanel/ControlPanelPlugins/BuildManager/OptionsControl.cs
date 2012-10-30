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
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace CoS
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
