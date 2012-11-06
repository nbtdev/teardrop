/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using DeveloperControlPanel;
using DeveloperControlPanel.Plugin;
using System.IO;

namespace Teardrop
{
    public class BuildManagerPlugin : Plugin
    {
        BuildManager m_buildManager = null;
        Options m_options = null;
        Log m_outputLog = null;
        Main m_mainForm = null;
        int m_pendingBuilds = 0;
        bool m_overallBuildStatus = true;
        System.Collections.ArrayList m_builtFiles = null;
        object m_syncObj = new object();

        string m_dataPath = string.Empty;
        string m_outputPath = string.Empty;
        string m_toolPath = string.Empty;

        public BuildManagerPlugin()
        {
        }

        #region Developer Control Panel Plugin Implementation
        OptionsControl m_optionsControl = null;

        PluginContext m_context = null;
        string m_name = "BuildManager";
        string m_displayName = "Build Manager";

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

        public override bool HasUI
        {
            get 
            {
                return true;
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

        public override DeveloperControlPanel.Plugin.OptionsControl OptionsContent
        {
            get 
            {
                return m_optionsControl;
            }
        }

        public override void MenuAdjust(ref ToolStripMenuItem toolStripMenuItem, ref ContextMenuStrip contextMenuStrip)
        {
            ToolStripMenuItem menuItem = new ToolStripMenuItem();
            menuItem.Name = Name;
            menuItem.Text = DisplayName;
            menuItem.Tag = this;
            menuItem.Click += new EventHandler(OnShowTool);
            toolStripMenuItem.DropDown.Items.Insert(0,menuItem);

            ToolStripMenuItem itemCopy = new ToolStripMenuItem();
            itemCopy.Name = menuItem.Name;
            itemCopy.Text = menuItem.Text;
            itemCopy.Tag = menuItem.Tag;
            itemCopy.Click += new EventHandler(OnShowTool);
            contextMenuStrip.Items.Insert(0,itemCopy);
        }

        public override bool Startup()
        {
            m_mainForm = new Main();
            m_mainForm.Hide();
            
            m_mainForm.Results.RootFolder = m_dataPath;
            m_outputLog = new Log(m_mainForm.OutputLog);
            m_buildManager = new BuildManager(m_dataPath, m_outputPath, m_toolPath);

            // listen for "files ready to build" events
            m_context.Shell.WatchedFilesChanged += new EventHandler<FilesChangedArgs>(Shell_WatchedFilesChanged);

            m_context.Shell.ConfigDataChanged += new EventHandler<ConfigDataChangedArgs>(Shell_ConfigDataChanged);

            // hook into the form treeview's "Build" event
            m_mainForm.Results.Build += new EventHandler<BuildEventArgs>(ForceBuild);

            // we also want to know if the user clicked on a post-build notify balloon so that we can make sure we are visible
            m_context.Shell.BuildNotifyBalloonClicked += new EventHandler<EventArgs>(Shell_BuildNotifyBalloonClicked);

            m_context.m_bDirtyMenu = true;

            return true;
        }

        public override void Shutdown()
        {
            
        }

        public override void OnShowTool(object sender, EventArgs e)
        {
            m_mainForm.Show();
            m_mainForm.WindowState = FormWindowState.Normal;
        }
        #endregion

        void Shell_BuildNotifyBalloonClicked(object sender, EventArgs e)
        {
            OnShowTool(sender, e);
        }

        void Shell_ConfigDataChanged(object sender, ConfigDataChangedArgs e)
        {
            if (e.Data["DataDir"] == null
                || e.Data["OutputDir"] == null
                || e.Data["ToolDir"] == null)
            {
                return;
            }

            m_dataPath = e.Data["DataDir"].ToString();
            m_outputPath = e.Data["OutputDir"].ToString();
            m_toolPath = e.Data["ToolDir"].ToString();

            m_buildManager.DataRoot = m_dataPath;
            m_mainForm.Results.RootFolder = m_dataPath;
            m_buildManager.OutputRoot = m_outputPath;
            m_buildManager.ToolRoot = m_toolPath;
        }

        void buildFiles(string[] files)
        {
            if (files.Length > 0)
            {
                // alert the system to a build-started condition
                lock (m_syncObj)
                {
                    m_pendingBuilds = 0;
                    m_overallBuildStatus = true;
                    m_builtFiles = new System.Collections.ArrayList();

                    m_context.Shell.RaiseBuildStarted();

                    if (files.Length == 1 && m_dataPath.StartsWith(files[0]) )
                    {
                        // This is the case where we've clicked a directory ABOVE the
                        // data path, so we want to just build the directory below.
                        files[0] = m_dataPath;
                    }

                    foreach (string s in files)
                    {

                        // check to see if the job is rooted in our data path
                        if (!s.StartsWith(m_dataPath))
                        {
                            continue;
                        }

                        // check to see if the job contains a directory name (in
                        // which case we need to do a recursive build...ugh)
                        System.IO.FileInfo fi = new FileInfo(s);
                        if ((fi.Attributes & FileAttributes.Directory) == FileAttributes.Directory)
                        {
                            System.Collections.ArrayList recursiveFiles = new System.Collections.ArrayList();
                            getFilesRecursive(s, recursiveFiles);

                            foreach (string file in recursiveFiles)
                            {
                                createJob(file);
                            }

                            continue;
                        }

                        // otherwise, it's a single file in our source tree, build it
                        createJob(s);
                    }
                }
            }
        }

        private void getFilesRecursive(string path, System.Collections.ArrayList files)
        {
            // we need to ignore .svn directories as well as files that end in .tmp
            string[] dirs = System.IO.Directory.GetDirectories(path);
            foreach (string dir in dirs)
            {
                if (dir.EndsWith(".svn"))
                {
                    continue;
                }

                // else, dive down into this directory and get its files
                getFilesRecursive(dir, files);
            }

            // and then, get ths files in this directory
            string[] dirFiles = System.IO.Directory.GetFiles(path, "*.*", SearchOption.TopDirectoryOnly);
            foreach (string dirFile in dirFiles)
            {
                if (dirFile.EndsWith(".tmp"))
                {
                    // ignore
                    continue;
                }

                files.Add(dirFile);
            }
        }

        private bool createJob(string sourceFile)
        {
            JobStatus job = new JobStatus();
            job.Status = JobStatus.State.PENDING;
            job.Path = sourceFile;

            // this is how the folder tree node gets its color updated
            job.JobStateChanged += new JobStateChangedHandler(m_mainForm.OnJobStatusChanged);

            // this is used internally so that we can track completed and in-flight builds
            // and compare against the total number, so that we properly can raise the 
            // "build completed" event 
            job.JobStateChanged += new JobStateChangedHandler(JobStateChanged);

            if (!m_buildManager.buildAsset(job))
            {
                // this happens when the code encounters a file type it
                // doesn't understand, so we need to pretend the build
                // succeeded, in effect, ignoring it
                //job.Status = JobStatus.State.UNKNOWN_TYPE;
                return false;
            }
            else
            {
                // this is where the job is associated with a folder tree node
                m_mainForm.OnJobCreated(job);
                lock (m_syncObj)
                {
                    ++m_pendingBuilds;
                }
                return true;
            }
        }

        void Shell_WatchedFilesChanged(object sender, FilesChangedArgs e)
        {
            buildFiles(e.Pathnames);
        }

        void ForceBuild(object sender, BuildEventArgs e)
        {
            buildFiles(e.Files);
        }

        void JobStateChanged(JobStatus job)
        {
            if (job.Status != JobStatus.State.PENDING &&
                job.Status != JobStatus.State.RUNNING)
            {
                // then the job finished in some manner
                lock (m_syncObj)
                {
                    m_overallBuildStatus &= (job.Status != JobStatus.State.FAILED);

                    if (job.Status == JobStatus.State.SUCCEEDED)
                    {
                        m_builtFiles.Add(job.OutputPath);
                    }

                    --m_pendingBuilds;

                    if (m_pendingBuilds == 0)
                    {
                        string[] files = new string[m_builtFiles.Count];
                        int i = 0;
                        foreach (string s in m_builtFiles)
                        {
                            files[i++] = s;
                        }

                        m_context.Shell.RaiseBuildCompleted(m_overallBuildStatus, files);
                    }
                }
#if DEBUG
                System.Console.WriteLine(String.Format("Pending builds: {0}", m_pendingBuilds));
#endif
            }
        }
    }
}
