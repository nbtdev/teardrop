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
using System.IO;
using System.Reflection;
using DeveloperControlPanel.Plugin;

namespace DeveloperControlPanel
{
    public partial class Main : Form, DeveloperControlPanel.Plugin.IADCP
    {
        struct PluginEntry
        {
            public Assembly assembly;
            public string filename;
            public Plugin.Plugin plugin;
            public bool started;
            public bool loaded;
        }

        class PluginTable : Dictionary<string,PluginEntry> { }

        class IconSet
        {
            public Icon m_newlyLoaded = null;
            public Icon m_building = null;
            public Icon m_success = null;
            public Icon m_failure = null;
            public IconSet()
            {
                try
                {
                    m_newlyLoaded = new Icon("icons\\Cos.ico");
                    m_building = new Icon("icons\\Cos_Building.ico");
                    m_success = new Icon("icons\\Cos_Success.ico");
                    m_failure = new Icon("icons\\Cos_Failure.ico");
                }
                catch (Exception e)
                {
                    MessageBox.Show("Error: Not all icons successfully loaded for the Developer Control Panel. \n(" + e.Message + ")");
                }
            }
        }

        PluginTable m_plugins = new PluginTable();
        Options m_options = new Options();
        bool m_loading = false;
        bool m_configDataRequest = false;
        string m_outputRootPath = string.Empty;
        IconSet m_iconSet = null;

        public Main()
        {
            InitializeComponent();
            loadPlugins();
            m_iconSet = new IconSet();
            this.WindowState = FormWindowState.Minimized;
            this.ShowInTaskbar = false;
        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
            Show();
            WindowState = FormWindowState.Normal;
        }

        private void Main_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == WindowState)
            {
                Hide();
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                WindowState = FormWindowState.Minimized;
                Hide();
                e.Cancel = true;
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DialogResult.Yes == MessageBox.Show(this, "Are you sure you want to close the Developer Control Panel?" + Environment.NewLine +
                "This will prevent automatic data builds from occurring.", "Confirm", MessageBoxButtons.YesNo))
            {
                Application.Exit();
            }
        }

        void populateMenu()
        {
            bool anyDirty = false;
            foreach (KeyValuePair<string, PluginEntry> pair in m_plugins)
                if (pair.Value.plugin.Context.m_bDirtyMenu)
                    anyDirty = true;

            if (anyDirty)
            {
                toolsToolStripMenuItem.DropDown.Items.Clear();
                contextMenuStrip1.Items.Clear();
                foreach (KeyValuePair<string, PluginEntry> pair in m_plugins)
                {
                    if (pair.Value.loaded && pair.Value.plugin.HasUI)
                    {
                        pair.Value.plugin.MenuAdjust(ref toolsToolStripMenuItem, ref contextMenuStrip1); //menu_trayTools);
                    }
                    pair.Value.plugin.Context.m_bDirtyMenu = false;
                }

                this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
                this.toolStripSeparator2,
                this.optionsToolStripMenuItem});

                this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
                this.toolStripSeparator3,
                this.optionsToolStripMenuItem1,
                this.helpToolStripMenuItem1,
                this.toolStripSeparator1,
                this.exitToolStripMenuItem1});
            }
        }

        void loadPlugins()
        {
            m_loading = true;

            string root = Environment.CurrentDirectory;
            string pluginPath = root + Path.DirectorySeparatorChar + "plugins";

            string[] plugins = null;

            try
            {
                plugins = Directory.GetFiles(pluginPath, "*.dll");
            }
            catch
            {
                m_loading = false;
                return;
            }

            foreach (string assemblyFilename in plugins)
            {
                PluginEntry ent = new PluginEntry();

                ent.loaded = false;
                ent.started = false;
                ent.filename = assemblyFilename;
                ent.assembly = Assembly.LoadFile(assemblyFilename);

                if (ent.assembly != null)
                {
                    ent.loaded = true;
                }
                else
                {
                    continue;
                }

                // start up the plugin
                Type[] types = null;
                try
                {
                    types = ent.assembly.GetTypes();
                }
                catch (ReflectionTypeLoadException ex)
                {
                    string excepts = string.Empty;
                    foreach (Exception exc in ex.LoaderExceptions)
                    {
                        excepts += exc.ToString();
                    }

                    MessageBox.Show(ex.Message + excepts);
                    continue;
                }

                foreach (Type type in types)
                {
                    if (type.BaseType == typeof(Plugin.Plugin))
                    {
                        // make a context block for this plugin to use
                        Plugin.PluginContext ctx = new DeveloperControlPanel.Plugin.PluginContext(this);
                        ctx.OptionsArea = m_options.Contents;

                        // create an instance of this type
                        ent.plugin = ent.assembly.CreateInstance(type.FullName) as Plugin.Plugin;
                        ent.plugin.Context = ctx;

                        // make a node in the options treeview, if one does not already exist
                        TreeNode node = m_options.FindNodeByName(Options.RootNodes.Plugins, ent.plugin.Name);
                        if (node == null)
                        {
                            node = m_options.AddNode(Options.RootNodes.Plugins, ent.plugin.Name);
                            node.Text = ent.plugin.DisplayName;
                        }

                        node.Tag = ent.plugin;
                        ent.started = ent.plugin.Startup();

                        // only one plugin per assembly, for now
                        break;
                    }
                }
                m_plugins[assemblyFilename] = ent;
            }

            m_loading = false;

            if (m_configDataRequest)
            {
                if (ConfigDataRequested != null)
                {
                    ConfigDataRequested(this, new EventArgs());
                }

                m_configDataRequest = false;
            }
            populateMenu();
        }

        private void toolsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            populateMenu();
        }

        private void contextMenuStrip1_Click(object sender, EventArgs e)
        {
            populateMenu();
        }

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_options.ShowDialog();
        }

        private void contentsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Not Implemented Yet");
        }

        private void indexToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Not Implemented Yet");
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dlgAbout dlg = new dlgAbout();
            dlg.ShowDialog(this);
        }

        private void notifyIcon1_BalloonTipClicked(object sender, EventArgs e)
        {
            if (BuildNotifyBalloonClicked != null)
            {
                BuildNotifyBalloonClicked(this, new EventArgs());
            }
        }

        #region IDCP Implementation
        /// <summary>
        /// Called by plugins to request config data. This method does not 
        /// immediately result in a response; if the DCP is in the process
        /// of loading all of its plugins, the config data request will be
        /// deferred until all plugins are loaded, to prevent spamming the 
        /// message bus.
        /// </summary>
        public void RequestConfigData()
        {
            m_configDataRequest = true;
            if (!m_loading)
            {
                if (ConfigDataRequested != null)
                {
                    ConfigDataRequested(this, new EventArgs());
                }
            }
        }

        /// <summary>
        /// Raised by the DCP to request configuration data 
        /// </summary>
        public event EventHandler<EventArgs> ConfigDataRequested;

        /// <summary>
        /// Called by plugins to raise the ConfigDataChanged event
        /// </summary>
        /// <param name="dict">The name-value pairs containing the config data</param>
        public void RaiseConfigDataChanged(System.Collections.Hashtable dict)
        {
            if (ConfigDataChanged != null)
            {
                ConfigDataChanged(this, new ConfigDataChangedArgs(dict));
            }

            // keep the output directory around for ourselves
            m_outputRootPath = dict["OutputDir"] as string;
        }

        /// <summary>
        /// Raised by the DCP when configuration data has changed
        /// </summary>
        public event EventHandler<ConfigDataChangedArgs> ConfigDataChanged;

        // we need this because the files-changed event is likely to 
        // come in from another thread
        delegate void WatchedFilesChangedDelegate(string[] files);
        void RaiseWatchedFilesChangedImpl(string[] files)
        {
            if (WatchedFilesChanged != null )
            {
                WatchedFilesChanged(this, new FilesChangedArgs(files));
                SetIconStatus(IconStatus.FileChangeDetected);
            }
            else
            {
                MessageBox.Show("Unable to Build.", "Build Manager", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification);
                SetIconStatus(IconStatus.BuildFailure);
            }
        }

        /// <summary>
        /// Called by file-watching plugins to alert other plugins that 
        /// watched files have changed
        /// </summary>
        /// <param name="files">Array of the changed files, full pathnames</param>
        public void RaiseWatchedFilesChanged(string[] files)
        {
            if (InvokeRequired)
            {
                object[] args = new object[1];
                args[0] = files;
                Invoke(new WatchedFilesChangedDelegate(RaiseWatchedFilesChangedImpl), args);
            }
            else
            {
                RaiseWatchedFilesChangedImpl(files);
            }
        }

        /// <summary>
        /// Raised in response to a request for configuration data by a plugin
        /// </summary>
        public event EventHandler<FilesChangedArgs> WatchedFilesChanged;

        // we need this because the build-started event can 
        // come in from another thread
        delegate void RaiseBuildStartedDelegate();
        void RaiseBuildStartedImpl()
        {
            if (BuildStarted != null)
            {
                BuildStarted(this, new EventArgs());
            }
        }

        /// <summary>
        /// Called by build-management plugins to indicate a build has begun
        /// </summary>
        public void RaiseBuildStarted()
        {
            if (InvokeRequired)
            {
                Invoke(new RaiseBuildStartedDelegate(RaiseBuildStartedImpl));
            }
            else
            {
                RaiseBuildStartedImpl();
            }
        }

        /// <summary>
        /// Raised when a build-manager plugin begins a build
        /// </summary>
        public event EventHandler<EventArgs> BuildStarted;

        // we need this because the build-started event can 
        // come in from another thread
        delegate void RaiseBuildCompletedDelegate(bool status, string[] files);
        void RaiseBuildCompletedImpl(bool status, string [] files)
        {
            if (BuildCompleted != null)
            {
                BuildCompleted(this, new BuildCompletedArgs(status, files));
            }

            StringBuilder sb = new StringBuilder();
            sb.Append(Environment.NewLine);

            foreach (string file in files)
            {
                string f = file.Replace("\"", "");
                sb.Append(f.Substring(m_outputRootPath.Length + 1) + Environment.NewLine);
            }

            // set the tray icon accordingly
            string title = string.Empty;

            if (status)
            {
                title = "Build Succeeded: "+files.Length+" file"+(files.Length!=1?"s":"")+"";
            }
            else
            {
                title = "Build Failed";
            }

            notifyIcon1.BalloonTipTitle = title;
            notifyIcon1.BalloonTipText = sb.ToString();
            notifyIcon1.ShowBalloonTip(1000);
            SetIconStatus(status ? IconStatus.BuildSuccess : IconStatus.BuildFailure);

        }

        /// <summary>
        /// Called by build-management plugins to indicate a build has completed
        /// </summary>
        /// <param name="status"><b>true</b> if the build succeeded, <b>false</b> otherwise</param>
        public void RaiseBuildCompleted(bool status, string[] files)
        {
            if (InvokeRequired)
            {
                object[] args = new object[2];
                args[0] = status;
                args[1] = files;
                Invoke(new RaiseBuildCompletedDelegate(RaiseBuildCompletedImpl), args);
            }
            else
            {
                RaiseBuildCompletedImpl(status, files);
            }
        }

        /// <summary>
        /// Raised when a build-manager plugin has completed a build
        /// </summary>
        public event EventHandler<BuildCompletedArgs> BuildCompleted;

        /// <summary>
        /// Called by a data publisher to indicate completion of data publication to a data server
        /// </summary>
        public void RaiseDataPublished()
        {
            if (DataPublished != null)
            {
                DataPublished(this, new EventArgs());
            }
        }

        /// <summary>
        /// Called by plugins to change the tool tray icon, to show status
        /// </summary>
        /// <param name="iconStatus">The new status for the icon to show</param>
        public void SetIconStatus(DeveloperControlPanel.Plugin.IconStatus iconStatus)
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));

            switch (iconStatus)
            {
                case IconStatus.NewlyLoaded:
                    notifyIcon1.Icon = m_iconSet.m_newlyLoaded;
                    break;
                case IconStatus.FileChangeDetected:
                case IconStatus.Building:
                    notifyIcon1.Icon = m_iconSet.m_building;
                    break;
                case IconStatus.BuildSuccess:
                    notifyIcon1.Icon = m_iconSet.m_success;
                    break;
                case IconStatus.BuildFailure:
                    notifyIcon1.Icon = m_iconSet.m_failure;
                    break;
            };
        }

        /// <summary>
        /// Raised when a data publisher has completed pushing data to a data server
        /// </summary>
        public event EventHandler<EventArgs> DataPublished;

        /// <summary>
        /// Raised when the user has clicked on the notify icon balloon 
        /// shown at the end of a build (so that interested plugins may
        /// respond if they so choose)
        /// </summary>
        public event EventHandler<EventArgs> BuildNotifyBalloonClicked;

        #endregion
    }
}