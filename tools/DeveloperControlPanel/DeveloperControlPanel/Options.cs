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
using System.Collections;

namespace DeveloperControlPanel
{
    public partial class Options : Form
    {
        Hashtable m_controls = new Hashtable();

        public enum RootNodes
        {
            General,
            Plugins,
        }

        public Options()
        {
            InitializeComponent();
            cmdApply.Enabled = false;
        }

        public TreeView Navigation
        {
            get
            {
                return tvwOptions;
            }
        }

        public Panel Contents
        {
            get
            {
                return paneOptions;
            }
        }

        public TreeNode AddNode(RootNodes rootNode, string nodeName)
        {
            TreeNode[] nodes = tvwOptions.Nodes.Find("node"+rootNode.ToString(), false);
            if (nodes.Length == 0)
            {
                return null;
            }

            TreeNode node = new TreeNode();
            node.Name = nodeName;
            nodes[0].Nodes.Add(node);

            return node;
        }

        public TreeNode FindNodeByName(RootNodes rootNode, string nodeName)
        {
            TreeNode[] nodes = tvwOptions.Nodes.Find(rootNode.ToString(), false);
            if (nodes.Length == 0)
            {
                return null;
            }

            TreeNode node = nodes[0];
            nodes = node.Nodes.Find(nodeName, false);
            if (nodes.Length > 0)
            {
                return nodes[0];
            }

            return null;
        }

        private void tvwOptions_AfterSelect(object sender, TreeViewEventArgs e)
        {
            paneOptions.Controls.Clear();

            if (e.Node == null)
                return;

            // grab the Plugin instance from the Tag property of the node, and
            // get the UserControl instance to put in the right pane
            Plugin.Plugin plugin = e.Node.Tag as Plugin.Plugin;
            if (plugin == null)
            {
                return;
            }

            if (plugin.OptionsContent == null)
            {
                return;
            }

            paneOptions.Controls.Add(plugin.OptionsContent);
            plugin.OptionsContent.Dock = DockStyle.Fill;
            if (!m_controls.Contains(plugin.Name))
            {
                // only add the callback once 
                DeveloperControlPanel.Plugin.OptionsControl.OptionsChangedHandler h = 
                    new DeveloperControlPanel.Plugin.OptionsControl.OptionsChangedHandler(OnOptionsChanged);

                plugin.OptionsContent.OnOptionsChanged += h;
                    
            }
            m_controls[plugin.Name] = plugin.OptionsContent;
        }

        private void OnOptionsChanged(object sender, EventArgs args)
        {
            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    if (ctl.Dirty)
                    {
                        cmdApply.Enabled = true;
                        return;
                    }
                }
            }

            cmdApply.Enabled = false;
        }

        private void cmdApply_Click(object sender, EventArgs e)
        {
            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    if (!ctl.BeforeApplyOptions())
                    {
                        return;
                    }
                }
            }

            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    ctl.OnApplyOptions();
                }
            }

            cmdApply.Enabled = false;
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    ctl.OnCancelOptions();
                }
            }

            Close();
        }

        private void cmdClose_Click(object sender, EventArgs e)
        {
            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    if (!ctl.BeforeCloseOptions())
                    {
                        return;
                    }
                }
            }

            foreach (DictionaryEntry ent in m_controls)
            {
                Plugin.OptionsControl ctl = ent.Value as Plugin.OptionsControl;
                if (ctl != null)
                {
                    ctl.OnCloseOptions();
                }
            }

            Close();
        }
    }
}