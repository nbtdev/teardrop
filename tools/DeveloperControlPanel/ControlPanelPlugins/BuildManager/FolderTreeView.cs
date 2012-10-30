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
using System.Runtime.InteropServices;
using System.Drawing;

namespace CoS
{
    public class BuildEventArgs : EventArgs
    {
        string[] m_files = null;

        public BuildEventArgs(string[] files)
        {
            m_files = files;
        }

        public string[] Files
        {
            get
            {
                return m_files;
            }
        }
    }

    public class FolderTreeView : TreeView
    {
        private ContextMenuStrip contextMenuStrip1;
        private System.ComponentModel.IContainer components;
        private ToolStripMenuItem mnuView;
        private ToolStripMenuItem mnuBuild;
        string m_root = string.Empty;

        [DllImport("Shell32", CharSet = CharSet.Auto)]
        internal extern static int ExtractIconEx(
            [MarshalAs(UnmanagedType.LPTStr)] 
            string lpszFile,
            int nIconIndex,
            IntPtr[] phIconLarge,
            IntPtr[] phIconSmall,
            int nIcons);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        extern static bool DestroyIcon(IntPtr handle);

        public event EventHandler<BuildEventArgs> Build;

        public FolderTreeView()
            : base()
        {
            InitializeComponent();
            mnuView.Click += new EventHandler(OnViewItem);
            mnuBuild.Click += new EventHandler(OnBuildItem);
            BeforeSelect += new TreeViewCancelEventHandler(OnBeforeSelect);
            NodeMouseClick += new TreeNodeMouseClickEventHandler(OnNodeMouseClick);

            string shell32 = "C:\\Windows\\System32\\Shell32.dll";
            int IconCount = ExtractIconEx(shell32, -1, null, null, 0);

            IntPtr[] IconPtr = new IntPtr[IconCount];
            Icon TempIcon;

            ExtractIconEx(shell32, 0, null, IconPtr, IconCount);

            Icon[] IconList = new Icon[IconCount];

            //gets the icons in a list.
            ImageList imgList = new ImageList();
            for (int i = 0; i < IconCount; i++)
            {
                TempIcon = (Icon)Icon.FromHandle(IconPtr[i]);
                Icon ManagedIcon = (Icon)TempIcon.Clone();
                DestroyIcon(TempIcon.Handle);
                imgList.Images.Add(ManagedIcon);
            }

            ImageList = imgList;
        }

        void OnNodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.Node != null)
            {
                SelectedNode = e.Node;
                OnAfterSelect(new TreeViewEventArgs(e.Node, TreeViewAction.ByMouse));
            }
        }

        void OnBeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            // if the user has selected a folder node we need to
            // gray out the View menu
            JobStatus stat = e.Node.Tag as JobStatus;
            if (stat == null)
            {
                mnuView.Enabled = false;
            }
            else
            {
                mnuView.Enabled = true;
            }
        }

        /// <summary>
        /// Recursive build starting at the provided node
        /// </summary>
        /// <param name="node"></param>
        void buildFolderNode(FolderTreeNode node, System.Collections.ArrayList fileList)
        {
            foreach (FolderTreeNode fileNode in node.Nodes)
            {
                JobStatus s = fileNode.Tag as JobStatus;
                if (s != null)
                {
                    fileList.Add(s.Path);
                }
                else
                {
                    // recurse into this folder
                    buildFolderNode(fileNode, fileList);
                }
            }
        }

        /// <summary>
        /// Build the selected item. If the user has selected a folder instead of
        /// a single file, all files in that folder will be built
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void OnBuildItem(object sender, EventArgs e)
        {
            FolderTreeNode node = SelectedNode as FolderTreeNode;
            if (node == null)
            {
                return;
            }

            JobStatus status = node.Tag as JobStatus;
            System.Collections.ArrayList fileList = new System.Collections.ArrayList();
            string[] files = null;

            if (status == null) // then it's a folder
            {
                // invoke a recursive build of this folder
                buildFolderNode(node, fileList);
            }
            else
            {
                // invoke a build of this single file
                JobStatus s = node.Tag as JobStatus;
                if (s != null)
                {
                    fileList.Add(s.Path);
                }
            }

            if (fileList.Count > 0)
            {
                files = new string[fileList.Count];
                int i = 0;
                foreach (string f in fileList)
                {
                    files[i++] = f;
                }

                if (Build != null)
                {
                    Build(this, new BuildEventArgs(files));
                }
            }
        }

        void OnViewItem(object sender, EventArgs e)
        {
            FolderTreeNode node = SelectedNode as FolderTreeNode;
            if (node == null)
            {
                return;
            }

            JobStatus status = node.Tag as JobStatus;
            if (status == null)
            {
                return;
            }

            try
            {
                System.Diagnostics.Process.Start(status.Path);
            }
            catch
            {
                MessageBox.Show("Could not start or find a viewer for the selected file", "Viewer Not Found");
            }
        }

        public string RootFolder
        {
            get
            {
                return m_root;
            }
            set
            {
                m_root = value;
            }
        }

        public FolderTreeNode CreateFileNodeImpl(string path)
        {
            // dissect the path and make nodes to the new node as needed
            // first, remove the stem (based on value of m_root)
            path = path.Replace(m_root, "");
            string[] parts = path.Split(System.IO.Path.DirectorySeparatorChar);

            // the first "part" is an empty string, since the above operation 
            // left a dangling path sep char at the front; and we don't want 
            // the actual filename (yet) so we do up to numParts-1 of them.
            TreeNodeCollection currentNodeSet = Nodes;
            BeginUpdate();
            for (int i = 1; i < parts.Length - 1; ++i)
            {
                // find or create a folder node 
                string s = parts[i];
                TreeNode[] nodes = currentNodeSet.Find(s, false);

                FolderTreeNode node = null;
                if (nodes.Length == 0)
                {
                    // then create one
                    node = new FolderTreeNode();
                    node.Name = s;
                    currentNodeSet.Add(node);
                }
                else
                {
                    node = nodes[0] as FolderTreeNode;
                }

                node.ImageIndex = 3;
                node.SelectedImageIndex = 4;
                node.Text = s;
                currentNodeSet = node.Nodes;
            }

            // then do a leaf node for the file
            string name = parts[parts.Length-1];
            TreeNode[] leaves = currentNodeSet.Find(name, false);
            FolderTreeNode leaf = null;
            if (leaves.Length > 0)
            {
                leaf = leaves[0] as FolderTreeNode;
            }
            else
            {
                leaf = new FolderTreeNode();
                leaf.ImageIndex = 1;
                leaf.SelectedImageIndex = 1;
                leaf.Text = name;
                leaf.Name = name;
                currentNodeSet.Add(leaf);
            }

            EndUpdate();

            return leaf;
        }

        delegate object CreateFileNodeDelegate(string path);
        public FolderTreeNode CreateFileNode(string path)
        {
            if (InvokeRequired)
            {
                object[] args = new object[1];
                args[0] = path;
                return (FolderTreeNode)Invoke(new CreateFileNodeDelegate(CreateFileNodeImpl), args);
            }
            else
            {
                return CreateFileNodeImpl(path);
            }
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuView = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuBuild = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuView,
            this.mnuBuild});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(108, 48);
            // 
            // mnuView
            // 
            this.mnuView.Name = "mnuView";
            this.mnuView.Size = new System.Drawing.Size(107, 22);
            this.mnuView.Text = "&View";
            // 
            // mnuBuild
            // 
            this.mnuBuild.Name = "mnuBuild";
            this.mnuBuild.Size = new System.Drawing.Size(107, 22);
            this.mnuBuild.Text = "Build";
            // 
            // FolderTreeView
            // 
            this.ContextMenuStrip = this.contextMenuStrip1;
            this.LineColor = System.Drawing.Color.Black;
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }
    }
}
