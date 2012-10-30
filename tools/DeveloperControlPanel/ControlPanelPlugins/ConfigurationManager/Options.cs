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
using ConfigurationManager;

namespace CoS
{
    public partial class Options : DeveloperControlPanel.Plugin.OptionsControl
    {
        OptionData m_optionData;
        public event EventHandler<OptionsApplyArgs> OptionsApply;

        public Options(OptionData optionData)
        {
            init();
            m_optionData = optionData;
            txtDataDir.Text = optionData.DataDir;
            txtOutputDir.Text = optionData.OutputDir;
            txtToolsDir.Text = optionData.ToolDir;
        }

        void init()
        {
            InitializeComponent();
        }

        //void onClientSpecChanged(object sender, EventArgs e)
        //{
        //    m_optionData.m_clientSpec.Value = txtClientSpec.Text;
        //    m_bDirty = m_optionData.m_clientSpec.isDirty();
        //    RaiseOptionsChanged(this);
        //}

        //void onMasterServerChanged(object sender, EventArgs e)
        //{
        //    m_optionData.m_masterServer.Value = txtMasterServer.Text;
        //    m_bDirty = m_optionData.m_masterServer.isDirty();
        //    RaiseOptionsChanged(this);
        //}

        public override void OnApplyOptions()
        {
            //m_optionData.m_masterServer.Value = txtMasterServer.Text;
            //m_optionData.m_clientSpec.Value = txtClientSpec.Text;

            // fire off an Apply event
            if (OptionsApply != null)
            {
                OptionsApply(this, new OptionsApplyArgs(m_optionData));
            }
        }

        public override bool BeforeApplyOptions()
        {
            return true;
        }

        public override bool BeforeCloseOptions()
        {
//            return !m_optionData.m_clientSpec.isDirty() && !m_optionData.m_clientSpec.isDirty();
            return true;
        }

        public override void OnCloseOptions()
        {
        }

        public override void OnCancelOptions()
        {
        }

        private void cmdEditOutputDir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.SelectedPath = txtOutputDir.Text;
            DialogResult r = dlg.ShowDialog(this);
            if (r == DialogResult.OK)
            {
                txtOutputDir.Text = dlg.SelectedPath;
            }
        }

        private void cmdEditDataDir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.SelectedPath = txtDataDir.Text;
            DialogResult r = dlg.ShowDialog(this);
            if (r == DialogResult.OK)
            {
                txtDataDir.Text = dlg.SelectedPath;
            }
        }

        private void cmdEditToolDir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.SelectedPath = txtToolsDir.Text;
            DialogResult r = dlg.ShowDialog(this);
            if (r == DialogResult.OK)
            {
                txtToolsDir.Text = dlg.SelectedPath;
            }
        }
    }
}
