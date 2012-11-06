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

namespace Teardrop
{
    public class FileSystemWatcher : Plugin
    {
        System.IO.FileSystemWatcher m_fsw = new System.IO.FileSystemWatcher();
        System.IO.FileSystemWatcher m_extfsw = new System.IO.FileSystemWatcher();
        System.Timers.Timer m_timer = new System.Timers.Timer();
        bool m_timerRunning = false;
        System.Collections.Hashtable m_pendingFiles = new System.Collections.Hashtable();
        System.Collections.ArrayList m_readyFiles = new System.Collections.ArrayList();
        object m_timerMutex = new object();

        class FileEntry
        {
            public string path = string.Empty;
            public DateTime lastActionTime;
        }

        #region Plugin Implementation
        OptionsControl m_optionsControl = null;

        PluginContext m_context = null;
        string m_name = "FileSystemWatcher";
        string m_displayName = "FileSystemWatcher";

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
            if (m_context == null)
            {
                return false;
            }

            // initialize the timer
            m_timer.Elapsed += new System.Timers.ElapsedEventHandler(OnTimerElapsed);
            m_timer.Interval = 2000; // in ms
            m_timer.Enabled = true;

            // we really only need to know about config data changes
            m_context.Shell.ConfigDataChanged += new EventHandler<ConfigDataChangedArgs>(Shell_ConfigDataChanged);

            // request a config update from the shell
            m_context.Shell.RequestConfigData();

            // this one watches for writes to the special file used to communicate
            // between the Explorer extension and the ADCP
            m_extfsw.NotifyFilter = System.IO.NotifyFilters.LastWrite;
            m_extfsw.IncludeSubdirectories = false;

            // C:\Documents And Settings\<username>\Local Settings\Application Data\ADCP
            string adcpPath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)
                + System.IO.Path.DirectorySeparatorChar
                + "DCP";

            System.IO.Directory.CreateDirectory(adcpPath);
            m_extfsw.Path = adcpPath;
            m_extfsw.EnableRaisingEvents = false;

            // we want to treat this special file like any other at this stage -- it 
            // will need to have the same 2-second timeout as the rest of the normal
            // data files
            m_extfsw.Changed += new System.IO.FileSystemEventHandler(OnFilesChanged);

            // this one watches the data directory
            m_fsw.NotifyFilter = System.IO.NotifyFilters.LastWrite;
            m_fsw.IncludeSubdirectories = true;
            m_fsw.Changed += new System.IO.FileSystemEventHandler(OnFilesChanged);

            // this is in non-paged memory, but in the Year Of Our Lord 2009, physical memory is cheap...
            // it defaults to 4K (one page), which corresponds to roughly 80 files. 
            m_fsw.InternalBufferSize = 400 * 1024; // 400kB, ~8000 files, enough for any SCM checkout

            // we need to wait for the config data update before we can start watching anything
            m_fsw.EnableRaisingEvents = false;

            return true;
        }

        public override void Shutdown()
        {

        }

        public override void OnShowTool(object sender, EventArgs e)
        {
        }

        #endregion

        public FileSystemWatcher()
        {
            System.Collections.Hashtable.Synchronized(m_pendingFiles);
        }

        /// <summary>
        /// Handle config data updates. Usually this will result in the path
        /// to the watched directory being changed.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Shell_ConfigDataChanged(object sender, ConfigDataChangedArgs e)
        {
            string path = e.Data["DataDir"] as string;

            if (path == null)
            {
                return;
            }

            if (path != m_fsw.Path)
            {
                try
                {
                    m_fsw.Path = path;
                    m_fsw.EnableRaisingEvents = true;
                    m_extfsw.EnableRaisingEvents = true;
                }
                catch (Exception)
                {
                    MessageBox.Show("File Watcher: Invalid path [" + path + "]");
                    m_fsw.EnableRaisingEvents = false;
                    m_extfsw.EnableRaisingEvents = false;
                }
            }
        }

        /// <summary>
        /// Raised by the System.IO.FileSystemWatcher when files changed in the
        /// watched directory tree
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void OnFilesChanged(object sender, System.IO.FileSystemEventArgs e)
        {
            // ignore .svn working directories, and also SVN temporary files (yer killin me, SVN)
            if (e.FullPath.Contains(".svn") || System.IO.Path.GetExtension(e.FullPath) == ".tmp")
            {
                return;
            }

            m_timer.Start();

            lock (m_pendingFiles.SyncRoot)
            {
                FileEntry ent = m_pendingFiles[e.FullPath] as FileEntry;
                if (ent == null)
                {
                    ent = new FileEntry();
                    ent.path = e.FullPath;
                    m_pendingFiles[e.FullPath] = ent;
                }

                // update the file entry -- these will be processed on the next timer tick
                ent.lastActionTime = DateTime.Now;

                if (!m_timerRunning)
                {
                    m_context.Shell.SetIconStatus(IconStatus.FileChangeDetected);
                    m_timerRunning = true;
                }
            }
        }

        void OnTimerElapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            // go through the list of newly arrived files and execute those for
            // which no activity has occurred since the last timer interval
            System.Collections.ArrayList tmp = new System.Collections.ArrayList();
            DateTime now = DateTime.Now;

            lock (m_pendingFiles.SyncRoot)
            {
                foreach (System.Collections.DictionaryEntry de in m_pendingFiles)
                {
                    FileEntry ent = de.Value as FileEntry;
                    if (ent.lastActionTime.AddSeconds(2) < now)
                    {
                        tmp.Add(ent.path);

                        // if this is the special adcp.txt file, take its contents
                        // and add them to the "ready files" list
                        if (ent.path.EndsWith("DCP" + System.IO.Path.DirectorySeparatorChar + "dcp.txt"))
                        {
                            System.IO.StreamReader r = new System.IO.StreamReader(ent.path);

                            string file = r.ReadLine();
                            while (file != null)
                            {
                                m_readyFiles.Add(file);
                                file = r.ReadLine();
                            }

                            r.Close();
                        }
                        else
                        {
                            // otherwise, it's a normal data file
                            System.Console.WriteLine("Processing " + ent.path);
                            m_readyFiles.Add(ent.path);
                        }
                    }
                }

                // clear out all of those that we processed
                foreach (string s in tmp)
                {
                    m_pendingFiles.Remove(s);
                }
            }

            // if there are no more pending files, send out a "files changed" event
            lock (m_timerMutex)
            {
                if (m_pendingFiles.Count == 0 && m_readyFiles.Count > 0)
                {
                    string[] paths = new string[m_readyFiles.Count];
                    int i = 0;
                    foreach (string s in m_readyFiles)
                    {
                        paths[i++] = s;
                    }
                    m_readyFiles.Clear();

                    m_context.Shell.RaiseWatchedFilesChanged(paths);
                }
                else
                {
                    if (m_timerRunning)
                    {
                        string[] paths = new string[0];
                        m_context.Shell.RaiseWatchedFilesChanged(paths);
                    }
                }

                m_timerRunning = false;
            }
        }

        private void OnFileCreated(object sender, System.IO.FileSystemEventArgs e)
        {
            // ignore .svn working directories, and also SVN temporary files (yer killin me, SVN)
            if (e.FullPath.Contains(".svn") || System.IO.Path.GetExtension(e.FullPath) == ".tmp")
            {
                return;
            }

        }

        private void OnFileDeleted(object sender, System.IO.FileSystemEventArgs e)
        {
            // ignore .svn working directories, and also SVN temporary files (yer killin me, SVN)
            if (e.FullPath.Contains(".svn") || System.IO.Path.GetExtension(e.FullPath) == ".tmp")
            {
                return;
            }


        }

        private void OnFileRenamed(object sender, System.IO.RenamedEventArgs e)
        {
            // ignore .svn working directories, and also SVN temporary files (yer killin me, SVN)
            if (e.FullPath.Contains(".svn") || System.IO.Path.GetExtension(e.FullPath) == ".tmp")
            {
                return;
            }


        }
    }
}
