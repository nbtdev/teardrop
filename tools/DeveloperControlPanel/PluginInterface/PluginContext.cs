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

namespace DeveloperControlPanel.Plugin
{
    public class PluginContext
    {
        ToolStripMenuItem m_menuTools;
        ToolStripMenuItem m_trayMenuTools;
        Panel m_optionsArea;

        public bool m_bDirtyMenu = false;

        public PluginContext(IADCP owner)
        {
            m_adcp = owner;
        }

        public ToolStripMenuItem ToolsMenu
        {
            get
            {
                return m_menuTools;
            }
            set
            {
                m_menuTools = value;
            }
        }

        public ToolStripMenuItem TrayToolsMenu
        {
            get
            {
                return m_trayMenuTools;
            }
            set
            {
                m_trayMenuTools = value;
            }
        }

        public Panel OptionsArea
        {
            get
            {
                return m_optionsArea;
            }
            set
            {
                m_optionsArea = value;
            }
        }

        IADCP m_adcp = null;
        public IADCP Shell
        {
            get
            {
                return m_adcp;
            }

            set
            {
                m_adcp = value;
            }
        }
    }
}
