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
    public abstract class Plugin
    {
        public abstract string Name { get; }
        public abstract string DisplayName { get; }
        public abstract PluginContext Context { set; get; }
        public abstract OptionsControl OptionsContent { get; }
        public abstract bool Startup();
        public abstract void Shutdown();
        public abstract void MenuAdjust( ref ToolStripMenuItem toolStripMenuItem, ref ContextMenuStrip contextMenuStrip );
        public abstract void OnShowTool(object sender, EventArgs e);
        public abstract bool HasUI { get; }
    }
}
