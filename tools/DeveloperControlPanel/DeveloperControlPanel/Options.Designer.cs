/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

namespace DeveloperControlPanel
{
    partial class Options
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("General");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Plugins");
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Options));
            this.panes = new System.Windows.Forms.SplitContainer();
            this.tvwOptions = new System.Windows.Forms.TreeView();
            this.paneOptions = new System.Windows.Forms.Panel();
            this.cmdClose = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdApply = new System.Windows.Forms.Button();
            this.panes.Panel1.SuspendLayout();
            this.panes.Panel2.SuspendLayout();
            this.panes.SuspendLayout();
            this.SuspendLayout();
            // 
            // panes
            // 
            this.panes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panes.Location = new System.Drawing.Point(0, 0);
            this.panes.Name = "panes";
            // 
            // panes.Panel1
            // 
            this.panes.Panel1.Controls.Add(this.tvwOptions);
            // 
            // panes.Panel2
            // 
            this.panes.Panel2.Controls.Add(this.paneOptions);
            this.panes.Panel2.Controls.Add(this.cmdClose);
            this.panes.Panel2.Controls.Add(this.cmdCancel);
            this.panes.Panel2.Controls.Add(this.cmdApply);
            this.panes.Size = new System.Drawing.Size(872, 420);
            this.panes.SplitterDistance = 189;
            this.panes.TabIndex = 0;
            // 
            // tvwOptions
            // 
            this.tvwOptions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvwOptions.HideSelection = false;
            this.tvwOptions.Location = new System.Drawing.Point(0, 0);
            this.tvwOptions.Name = "tvwOptions";
            treeNode1.Name = "nodeGeneral";
            treeNode1.Text = "General";
            treeNode1.ToolTipText = "General Control Panel Options";
            treeNode2.Name = "nodePlugins";
            treeNode2.Text = "Plugins";
            treeNode2.ToolTipText = "Plugin Manager Settings";
            this.tvwOptions.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
            this.tvwOptions.Size = new System.Drawing.Size(189, 420);
            this.tvwOptions.TabIndex = 0;
            this.tvwOptions.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvwOptions_AfterSelect);
            // 
            // paneOptions
            // 
            this.paneOptions.Dock = System.Windows.Forms.DockStyle.Top;
            this.paneOptions.Location = new System.Drawing.Point(0, 0);
            this.paneOptions.Name = "paneOptions";
            this.paneOptions.Size = new System.Drawing.Size(679, 379);
            this.paneOptions.TabIndex = 24;
            // 
            // cmdClose
            // 
            this.cmdClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdClose.Location = new System.Drawing.Point(439, 394);
            this.cmdClose.Name = "cmdClose";
            this.cmdClose.Size = new System.Drawing.Size(75, 23);
            this.cmdClose.TabIndex = 23;
            this.cmdClose.Text = "C&lose";
            this.cmdClose.UseVisualStyleBackColor = true;
            this.cmdClose.Click += new System.EventHandler(this.cmdClose_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdCancel.Location = new System.Drawing.Point(520, 394);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(75, 23);
            this.cmdCancel.TabIndex = 22;
            this.cmdCancel.Text = "&Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // cmdApply
            // 
            this.cmdApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdApply.Location = new System.Drawing.Point(601, 394);
            this.cmdApply.Name = "cmdApply";
            this.cmdApply.Size = new System.Drawing.Size(75, 23);
            this.cmdApply.TabIndex = 21;
            this.cmdApply.Text = "&Apply";
            this.cmdApply.UseVisualStyleBackColor = true;
            this.cmdApply.Click += new System.EventHandler(this.cmdApply_Click);
            // 
            // Options
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(872, 420);
            this.Controls.Add(this.panes);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Options";
            this.Text = "Options";
            this.panes.Panel1.ResumeLayout(false);
            this.panes.Panel2.ResumeLayout(false);
            this.panes.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer panes;
        private System.Windows.Forms.TreeView tvwOptions;
        private System.Windows.Forms.Panel paneOptions;
        private System.Windows.Forms.Button cmdClose;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdApply;
    }
}