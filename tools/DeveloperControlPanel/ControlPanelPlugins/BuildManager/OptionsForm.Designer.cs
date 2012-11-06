/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

namespace Teardrop
{
    partial class dlgOptions
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
            this.cmdApply = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.txtDataRoot = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtOutputRoot = new System.Windows.Forms.TextBox();
            this.cmdBrowseData = new System.Windows.Forms.Button();
            this.cmdBrowseOutput = new System.Windows.Forms.Button();
            this.cmdClose = new System.Windows.Forms.Button();
            this.cmdBrowseToolDir = new System.Windows.Forms.Button();
            this.txtToolDir = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cmdApply
            // 
            this.cmdApply.Location = new System.Drawing.Point(467, 158);
            this.cmdApply.Name = "cmdApply";
            this.cmdApply.Size = new System.Drawing.Size(75, 23);
            this.cmdApply.TabIndex = 0;
            this.cmdApply.Text = "&Apply";
            this.cmdApply.UseVisualStyleBackColor = true;
            this.cmdApply.Click += new System.EventHandler(this.cmdApply_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Location = new System.Drawing.Point(386, 158);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(75, 23);
            this.cmdCancel.TabIndex = 1;
            this.cmdCancel.Text = "&Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // txtDataRoot
            // 
            this.txtDataRoot.Location = new System.Drawing.Point(123, 6);
            this.txtDataRoot.Name = "txtDataRoot";
            this.txtDataRoot.Size = new System.Drawing.Size(371, 20);
            this.txtDataRoot.TabIndex = 2;
            this.txtDataRoot.TextChanged += new System.EventHandler(this.txtDataRoot_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Root Data Folder";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Root Output Folder";
            // 
            // txtOutputRoot
            // 
            this.txtOutputRoot.Location = new System.Drawing.Point(123, 36);
            this.txtOutputRoot.Name = "txtOutputRoot";
            this.txtOutputRoot.Size = new System.Drawing.Size(371, 20);
            this.txtOutputRoot.TabIndex = 5;
            this.txtOutputRoot.TextChanged += new System.EventHandler(this.txtOutputRoot_TextChanged);
            // 
            // cmdBrowseData
            // 
            this.cmdBrowseData.Location = new System.Drawing.Point(500, 4);
            this.cmdBrowseData.Name = "cmdBrowseData";
            this.cmdBrowseData.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseData.TabIndex = 6;
            this.cmdBrowseData.Text = "...";
            this.cmdBrowseData.UseVisualStyleBackColor = true;
            this.cmdBrowseData.Click += new System.EventHandler(this.cmdBrowseData_Click);
            // 
            // cmdBrowseOutput
            // 
            this.cmdBrowseOutput.Location = new System.Drawing.Point(500, 34);
            this.cmdBrowseOutput.Name = "cmdBrowseOutput";
            this.cmdBrowseOutput.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseOutput.TabIndex = 7;
            this.cmdBrowseOutput.Text = "...";
            this.cmdBrowseOutput.UseVisualStyleBackColor = true;
            this.cmdBrowseOutput.Click += new System.EventHandler(this.cmdBrowseOutput_Click);
            // 
            // cmdClose
            // 
            this.cmdClose.Location = new System.Drawing.Point(305, 158);
            this.cmdClose.Name = "cmdClose";
            this.cmdClose.Size = new System.Drawing.Size(75, 23);
            this.cmdClose.TabIndex = 8;
            this.cmdClose.Text = "C&lose";
            this.cmdClose.UseVisualStyleBackColor = true;
            this.cmdClose.Click += new System.EventHandler(this.cmdClose_Click);
            // 
            // cmdBrowseToolDir
            // 
            this.cmdBrowseToolDir.Location = new System.Drawing.Point(500, 64);
            this.cmdBrowseToolDir.Name = "cmdBrowseToolDir";
            this.cmdBrowseToolDir.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseToolDir.TabIndex = 11;
            this.cmdBrowseToolDir.Text = "...";
            this.cmdBrowseToolDir.UseVisualStyleBackColor = true;
            this.cmdBrowseToolDir.Click += new System.EventHandler(this.cmdBrowseToolDir_Click);
            // 
            // txtToolDir
            // 
            this.txtToolDir.Location = new System.Drawing.Point(123, 66);
            this.txtToolDir.Name = "txtToolDir";
            this.txtToolDir.Size = new System.Drawing.Size(371, 20);
            this.txtToolDir.TabIndex = 10;
            this.txtToolDir.TextChanged += new System.EventHandler(this.txtToolDir_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(78, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Tool Bin Folder";
            // 
            // dlgOptions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(554, 193);
            this.Controls.Add(this.cmdBrowseToolDir);
            this.Controls.Add(this.txtToolDir);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cmdClose);
            this.Controls.Add(this.cmdBrowseOutput);
            this.Controls.Add(this.cmdBrowseData);
            this.Controls.Add(this.txtOutputRoot);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtDataRoot);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdApply);
            this.Name = "dlgOptions";
            this.Text = "Options";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdApply;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.TextBox txtDataRoot;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtOutputRoot;
        private System.Windows.Forms.Button cmdBrowseData;
        private System.Windows.Forms.Button cmdBrowseOutput;
        private System.Windows.Forms.Button cmdClose;
        private System.Windows.Forms.Button cmdBrowseToolDir;
        private System.Windows.Forms.TextBox txtToolDir;
        private System.Windows.Forms.Label label3;
    }
}