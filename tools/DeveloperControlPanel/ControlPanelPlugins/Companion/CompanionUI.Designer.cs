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

namespace CoS
{
    partial class CompanionUI
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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reconnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPerformance = new System.Windows.Forms.TabPage();
            this.statusPerformance = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.txtFPS = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.txtTris = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.txtVerts = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.txtBatches = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabMemory = new System.Windows.Forms.TabPage();
            this.lvwMemory = new System.Windows.Forms.ListView();
            this.label1 = new System.Windows.Forms.Label();
            this.colAllocatorName = new System.Windows.Forms.ColumnHeader();
            this.colAllocatorUsage = new System.Windows.Forms.ColumnHeader();
            this.colAllocatorHigh = new System.Windows.Forms.ColumnHeader();
            this.colStatus = new System.Windows.Forms.ColumnHeader();
            this.menuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPerformance.SuspendLayout();
            this.statusPerformance.SuspendLayout();
            this.tabMemory.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(647, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.reconnectToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // reconnectToolStripMenuItem
            // 
            this.reconnectToolStripMenuItem.Name = "reconnectToolStripMenuItem";
            this.reconnectToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.reconnectToolStripMenuItem.Text = "&Reconnect";
            this.reconnectToolStripMenuItem.Click += new System.EventHandler(this.reconnectToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPerformance);
            this.tabControl1.Controls.Add(this.tabMemory);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 24);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(647, 354);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPerformance
            // 
            this.tabPerformance.Controls.Add(this.statusPerformance);
            this.tabPerformance.Location = new System.Drawing.Point(4, 22);
            this.tabPerformance.Name = "tabPerformance";
            this.tabPerformance.Padding = new System.Windows.Forms.Padding(3);
            this.tabPerformance.Size = new System.Drawing.Size(639, 328);
            this.tabPerformance.TabIndex = 0;
            this.tabPerformance.Text = "Performance";
            this.tabPerformance.UseVisualStyleBackColor = true;
            // 
            // statusPerformance
            // 
            this.statusPerformance.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.txtFPS,
            this.toolStripStatusLabel2,
            this.txtTris,
            this.toolStripStatusLabel3,
            this.txtVerts,
            this.toolStripStatusLabel4,
            this.txtBatches});
            this.statusPerformance.Location = new System.Drawing.Point(3, 303);
            this.statusPerformance.Name = "statusPerformance";
            this.statusPerformance.Size = new System.Drawing.Size(633, 22);
            this.statusPerformance.TabIndex = 0;
            this.statusPerformance.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(32, 17);
            this.toolStripStatusLabel1.Text = "FPS: ";
            // 
            // txtFPS
            // 
            this.txtFPS.Name = "txtFPS";
            this.txtFPS.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(28, 17);
            this.toolStripStatusLabel2.Text = "Tris:";
            // 
            // txtTris
            // 
            this.txtTris.Name = "txtTris";
            this.txtTris.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(36, 17);
            this.toolStripStatusLabel3.Text = "Verts:";
            // 
            // txtVerts
            // 
            this.txtVerts.Name = "txtVerts";
            this.txtVerts.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(49, 17);
            this.toolStripStatusLabel4.Text = "Batches:";
            // 
            // txtBatches
            // 
            this.txtBatches.Name = "txtBatches";
            this.txtBatches.Size = new System.Drawing.Size(0, 17);
            // 
            // tabMemory
            // 
            this.tabMemory.Controls.Add(this.label1);
            this.tabMemory.Controls.Add(this.lvwMemory);
            this.tabMemory.Location = new System.Drawing.Point(4, 22);
            this.tabMemory.Name = "tabMemory";
            this.tabMemory.Padding = new System.Windows.Forms.Padding(3);
            this.tabMemory.Size = new System.Drawing.Size(639, 328);
            this.tabMemory.TabIndex = 1;
            this.tabMemory.Text = "Memory";
            this.tabMemory.UseVisualStyleBackColor = true;
            // 
            // lvwMemory
            // 
            this.lvwMemory.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lvwMemory.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colAllocatorName,
            this.colAllocatorUsage,
            this.colAllocatorHigh,
            this.colStatus});
            this.lvwMemory.Location = new System.Drawing.Point(9, 19);
            this.lvwMemory.Name = "lvwMemory";
            this.lvwMemory.Size = new System.Drawing.Size(622, 301);
            this.lvwMemory.TabIndex = 0;
            this.lvwMemory.UseCompatibleStateImageBehavior = false;
            this.lvwMemory.View = System.Windows.Forms.View.Details;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Allocators";
            // 
            // colAllocatorName
            // 
            this.colAllocatorName.Text = "Allocator";
            this.colAllocatorName.Width = 150;
            // 
            // colAllocatorUsage
            // 
            this.colAllocatorUsage.Text = "Current";
            this.colAllocatorUsage.Width = 100;
            // 
            // colAllocatorHigh
            // 
            this.colAllocatorHigh.Text = "High Water";
            this.colAllocatorHigh.Width = 100;
            // 
            // colStatus
            // 
            this.colStatus.Text = "Status";
            this.colStatus.Width = 200;
            // 
            // CompanionUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(647, 378);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "CompanionUI";
            this.Text = "Arcadia Companion";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPerformance.ResumeLayout(false);
            this.tabPerformance.PerformLayout();
            this.statusPerformance.ResumeLayout(false);
            this.statusPerformance.PerformLayout();
            this.tabMemory.ResumeLayout(false);
            this.tabMemory.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPerformance;
        private System.Windows.Forms.StatusStrip statusPerformance;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel txtFPS;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripStatusLabel txtTris;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ToolStripStatusLabel txtVerts;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.ToolStripStatusLabel txtBatches;
        private System.Windows.Forms.ToolStripMenuItem reconnectToolStripMenuItem;
        private System.Windows.Forms.TabPage tabMemory;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView lvwMemory;
        private System.Windows.Forms.ColumnHeader colAllocatorName;
        private System.Windows.Forms.ColumnHeader colAllocatorUsage;
        private System.Windows.Forms.ColumnHeader colAllocatorHigh;
        private System.Windows.Forms.ColumnHeader colStatus;
    }
}

