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
using ConfigurationManager;

namespace Teardrop
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
