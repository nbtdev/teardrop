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
using System.Text;
using System.Windows.Forms;
using DeveloperControlPanel;
using DeveloperControlPanel.Plugin;

namespace SamplePlugin
{
    /// <summary>
    /// Plugins need to live in the "plugins" directory -- you'll find this 
    /// directory in the same directory as the Arcadia Developer Control Panel
    /// application executable. It is recommended that you either build to, or
    /// copy your assembly DLL to, this directory, during plugin development.
    /// 
    /// Debugging plugins can occur in one of two ways -- you can either attach
    /// to a running Control Panel application and dynamically load and unload
    /// your plugin, or (more commonly), simply start up the Control Panel as
    /// your assembly DLL's executable host application. 
    /// 
    /// It is also recommended that you simply include the PluginInterface project
    /// in your solution, and "Add Reference" to that project, instead of trying
    /// to add a reference to an arbitrary DLL location on the filesystem; those
    /// locations are subject to change and you may end up referencing a stale
    /// assembly. Including the project in your solution avoids that since you 
    /// then always have the current version of the interface definition.
    /// </summary>
    public partial class SamplePlugin : Plugin
    {
        #region Plugin Interface Implementation
        /// <summary>
        /// This field stores the value associated with the Context
        /// property required by the Plugin interface. The PluginContext
        /// provides access to all of the shell UI components that a 
        /// plugin should need -- the Options dialog contents pane,
        /// both shell and System Tray plugin-specific menus, and
        /// so on.
        /// </summary>
        PluginContext m_context;
        public override PluginContext Context
        {
            get
            {
                return m_context;
            }
            set
            {
                m_context = value;
            }
        }

        /// <summary>
        /// Return "true" if your plugin has a form of its own, "false" otherwise.
        /// If you return "true", an entry for your plugin will appear in the "Tools"
        /// menu in the ADCP main form, as well as the System Tray Tools menu.
        /// </summary>
        public override bool HasUI
        {
            get
            {
                return false;
            }
        }

        /// <summary>
        /// The DisplayName property provides the text that will appear for this plugin's
        /// entry in the Options dialog "Plugins" node in the options treeview.
        /// </summary>
        String m_displayName = "SamplePlugin";
        public override string DisplayName
        {
            get 
            {
                return m_displayName;
            }
        }

        /// <summary>
        /// This field/property provide the unique name for the plugin's 
        /// node in the Options dialog's Plugins treeview node.
        /// </summary>
        String m_name = "SamplePlugin";
        public override string Name
        {
            get 
            {
                return m_name;
            }
        }

        /// <summary>
        /// This method is called by the Arcadia Developer Control Panel
        /// framework when the menu is opened.  Use it to adjust the menu
        /// in any way you see fit, including making context-sensitive
        /// adjustments.
        /// 
        /// A typical piece of code might look like this:
        /// ToolStripMenuItem menuItem = new ToolStripMenuItem();
        /// menuItem.Name = Name;
        /// menuItem.Text = DisplayName;
        /// menuItem.Tag = this;
        /// menuItem.Click += new EventHandler(OnShowTool);
        /// toolStripMenuItem.DropDown.Items.Insert(0,menuItem);
        /// 
        /// ToolStripMenuItem itemCopy = new ToolStripMenuItem();
        /// itemCopy.Name = menuItem.Name;
        /// itemCopy.Text = menuItem.Text;
        /// itemCopy.Tag = menuItem.Tag;
        /// itemCopy.Click += new EventHandler(OnShowTool);
        /// contextMenuStrip.Items.Insert(0,itemCopy);
        /// 
        /// </summary>
        public override void MenuAdjust(ref ToolStripMenuItem toolStripMenuItem, ref ContextMenuStrip contextMenuStrip)
        {
        }

        /// <summary>
        /// This method is called by the Arcadia Developer Control Panel
        /// framework when a plugin assembly has successfully been loaded,
        /// its plugin instance created, and the context value set on the 
        /// instance. In other words, once this method is called you can 
        /// count on the Context property being set to a "good" value.
        /// 
        /// If the plugin did not start properly, it will still have an entry
        /// in the Options dialog, so you may want to provide a UI means for the 
        /// user to restart or retry your startup process.
        /// </summary>
        /// <returns>true if the plugin successfully started, false otherwise.</returns>
        public override bool Startup()
        {
            /* 
             * do any plugin startup code you need here -- fixing up various
             * UI elements, and so on...
            */
            //m_context.m_bDirtyMenu = true;
            // Set this if you need to make menu items.
            return true;
        }

        /// <summary>
        /// This method is called by the framework when it is shutting down -- either just
        /// your plugin, or the shell itself. Regardless, once this method is called, you 
        /// must consider your plugin to be deactived and/or dead -- so treat this as your
        /// plugin's "destructor"; the next step after this method returns is usually to
        /// unload your plugin's assembly.
        /// </summary>
        public override void Shutdown()
        {
            /*
             * Do any code you need to clean up after yourself
             * 
             */
        }

        /// <summary>
        /// You will need to provide a UserControl for your plugin's Options
        /// pane contents -- when your plugin is selected in the shell's Options
        /// dialog treeview, the framework will ask your plugin instance for your
        /// options content view, by requesting the value of this property.
        /// </summary>
        public override DeveloperControlPanel.Plugin.OptionsControl OptionsContent
        {
            get
            {
                return new OptionsControl();
            }
        }

        /// <summary>
        /// This method is called by the framework when the user selects your
        /// plugin from either the system tray context menu, or the Tools menu
        /// in the Control Panel interface. Your plugin can optionally show an 
        /// interface, if applicable, or simply ignore this message.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public override void OnShowTool(object sender, EventArgs e)
        {
            SamplePluginForm frm = new SamplePluginForm();
            frm.Show(); // show in modeless state -- IMPORTANT!!!
        }
        #endregion // Plugin Interface Implementation
    }
}
