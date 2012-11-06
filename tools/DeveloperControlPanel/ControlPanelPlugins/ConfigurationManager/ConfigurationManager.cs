/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DeveloperControlPanel.Plugin;
using Microsoft.Win32;
using System.Xml;
using System.Xml.XPath;
using ConfigurationManager;

namespace Teardrop
{
    public class ConfigurationManager : Plugin
    {
        System.Collections.Hashtable m_config = new System.Collections.Hashtable();

        #region Plugin Implementation
        OptionsControl m_optionsControl = null;
        PluginContext m_context = null;
        OptionData m_optionData = null;
        string m_name = "ConfigurationManager";
        string m_displayName = "Configuration Manager";

        public override bool HasUI
        {
            get
            {
                return false;
            }
        }

        public override string Name
        {
            get
            {
                return m_name;
            }
        }

        public override string DisplayName
        {
            get
            {
                return m_displayName;
            }
        }

        public override PluginContext Context
        {
            set
            {
                m_context = value;
            }
            get
            {
                return m_context;
            }
        }

        public override OptionsControl OptionsContent
        {
            get
            {
                return m_optionsControl;
            }
        }

        public override void MenuAdjust(ref ToolStripMenuItem toolStripMenuItem, ref ContextMenuStrip contextMenuStrip)
        {
        }

        public override bool Startup()
        {
            m_optionData = new OptionData();
            Options opts = new Options(m_optionData);
            m_optionsControl = opts;
            opts.OptionsApply += new EventHandler<OptionsApplyArgs>(OnOptionsApply);

            // hook us into shell events
            m_context.Shell.ConfigDataRequested += new EventHandler<EventArgs>(Shell_ConfigDataRequested);

            // fill in our config data
            foreach (System.Reflection.PropertyInfo i in m_optionData.GetType().GetProperties())
            {
                m_config[i.Name] = i.GetValue(m_optionData, null);
            }
            return true;
        }

        /// <summary>
        /// called when the user applies options in the options control 
        /// (usually when they change client spec)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void OnOptionsApply(object sender, OptionsApplyArgs e)
        {
            if (m_optionData == null)
                return;

            // first, need to update the registry with the current info
            m_optionData.Save();

            // and then spam out that config data changed
            m_context.Shell.RaiseConfigDataChanged(m_config);
        }

        public override void Shutdown()
        {

        }

        public override void OnShowTool(object sender, EventArgs e)
        {
        }

        #endregion

        public ConfigurationManager()
        {
        }

        /// <summary>
        /// Raised by the shell when someone wants config data
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Shell_ConfigDataRequested(object sender, EventArgs e)
        {
            // for now just return our internal data
            m_context.Shell.RaiseConfigDataChanged(m_config);
        }
    }
}
