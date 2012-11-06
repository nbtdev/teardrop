/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

namespace Teardrop
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.txtOutputDir = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtDataDir = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.txtToolsDir = new System.Windows.Forms.TextBox();
            this.cmdEditOutputDir = new System.Windows.Forms.Button();
            this.cmdEditToolDir = new System.Windows.Forms.Button();
            this.cmdEditDataDir = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Output Directory:";
            // 
            // txtOutputDir
            // 
            this.txtOutputDir.Location = new System.Drawing.Point(106, 19);
            this.txtOutputDir.Name = "txtOutputDir";
            this.txtOutputDir.Size = new System.Drawing.Size(210, 20);
            this.txtOutputDir.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(5, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Data Directory:";
            // 
            // txtDataDir
            // 
            this.txtDataDir.Location = new System.Drawing.Point(106, 45);
            this.txtDataDir.Name = "txtDataDir";
            this.txtDataDir.Size = new System.Drawing.Size(210, 20);
            this.txtDataDir.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(5, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Tools Directory:";
            // 
            // txtToolsDir
            // 
            this.txtToolsDir.Location = new System.Drawing.Point(106, 71);
            this.txtToolsDir.Name = "txtToolsDir";
            this.txtToolsDir.Size = new System.Drawing.Size(210, 20);
            this.txtToolsDir.TabIndex = 4;
            // 
            // cmdEditOutputDir
            // 
            this.cmdEditOutputDir.Location = new System.Drawing.Point(322, 19);
            this.cmdEditOutputDir.Name = "cmdEditOutputDir";
            this.cmdEditOutputDir.Size = new System.Drawing.Size(29, 20);
            this.cmdEditOutputDir.TabIndex = 6;
            this.cmdEditOutputDir.Text = "...";
            this.cmdEditOutputDir.UseVisualStyleBackColor = true;
            this.cmdEditOutputDir.Click += new System.EventHandler(this.cmdEditOutputDir_Click);
            // 
            // cmdEditToolDir
            // 
            this.cmdEditToolDir.Location = new System.Drawing.Point(322, 71);
            this.cmdEditToolDir.Name = "cmdEditToolDir";
            this.cmdEditToolDir.Size = new System.Drawing.Size(29, 20);
            this.cmdEditToolDir.TabIndex = 7;
            this.cmdEditToolDir.Text = "...";
            this.cmdEditToolDir.UseVisualStyleBackColor = true;
            this.cmdEditToolDir.Click += new System.EventHandler(this.cmdEditToolDir_Click);
            // 
            // cmdEditDataDir
            // 
            this.cmdEditDataDir.Location = new System.Drawing.Point(322, 45);
            this.cmdEditDataDir.Name = "cmdEditDataDir";
            this.cmdEditDataDir.Size = new System.Drawing.Size(29, 20);
            this.cmdEditDataDir.TabIndex = 8;
            this.cmdEditDataDir.Text = "...";
            this.cmdEditDataDir.UseVisualStyleBackColor = true;
            this.cmdEditDataDir.Click += new System.EventHandler(this.cmdEditDataDir_Click);
            // 
            // Options
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cmdEditDataDir);
            this.Controls.Add(this.cmdEditToolDir);
            this.Controls.Add(this.cmdEditOutputDir);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtToolsDir);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtDataDir);
            this.Controls.Add(this.txtOutputDir);
            this.Controls.Add(this.label1);
            this.Name = "Options";
            this.Size = new System.Drawing.Size(436, 247);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtOutputDir;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtDataDir;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtToolsDir;
        private System.Windows.Forms.Button cmdEditOutputDir;
        private System.Windows.Forms.Button cmdEditToolDir;
        private System.Windows.Forms.Button cmdEditDataDir;
    }
}
