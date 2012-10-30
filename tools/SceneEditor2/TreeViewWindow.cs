using System;
using System.Collections.Generic;
using System.Text;
using WeifenLuo.WinFormsUI.Docking;

namespace SceneEditor
{
    public class TreeViewWindow : DockContent
    {
        private CoS.Controls.TreeView treeView1;

        public TreeViewWindow()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.treeView1 = new CoS.Controls.TreeView();
            this.SuspendLayout();
            // 
            // treeView1
            // 
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(292, 266);
            this.treeView1.TabIndex = 0;
            // 
            // TreeViewWindow
            // 
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.treeView1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "TreeViewWindow";
            this.ResumeLayout(false);

        }

        public CoS.Controls.TreeView TreeView
        {
            get
            {
                return treeView1;
            }
        }
    }
}
