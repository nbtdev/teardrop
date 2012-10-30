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
    partial class OptionsControl
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
            this.cmdBrowseToolDir = new System.Windows.Forms.Button();
            this.txtToolDir = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cmdBrowseOutput = new System.Windows.Forms.Button();
            this.cmdBrowseData = new System.Windows.Forms.Button();
            this.txtOutputRoot = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtDataRoot = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // cmdBrowseToolDir
            // 
            this.cmdBrowseToolDir.Location = new System.Drawing.Point(491, 66);
            this.cmdBrowseToolDir.Name = "cmdBrowseToolDir";
            this.cmdBrowseToolDir.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseToolDir.TabIndex = 23;
            this.cmdBrowseToolDir.Text = "...";
            this.cmdBrowseToolDir.UseVisualStyleBackColor = true;
            this.cmdBrowseToolDir.Click += new System.EventHandler(this.cmdBrowseToolDir_Click);
            // 
            // txtToolDir
            // 
            this.txtToolDir.Location = new System.Drawing.Point(114, 68);
            this.txtToolDir.Name = "txtToolDir";
            this.txtToolDir.Size = new System.Drawing.Size(371, 20);
            this.txtToolDir.TabIndex = 22;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 71);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(78, 13);
            this.label3.TabIndex = 21;
            this.label3.Text = "Tool Bin Folder";
            // 
            // cmdBrowseOutput
            // 
            this.cmdBrowseOutput.Location = new System.Drawing.Point(491, 36);
            this.cmdBrowseOutput.Name = "cmdBrowseOutput";
            this.cmdBrowseOutput.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseOutput.TabIndex = 19;
            this.cmdBrowseOutput.Text = "...";
            this.cmdBrowseOutput.UseVisualStyleBackColor = true;
            this.cmdBrowseOutput.Click += new System.EventHandler(this.cmdBrowseOutput_Click);
            // 
            // cmdBrowseData
            // 
            this.cmdBrowseData.Location = new System.Drawing.Point(491, 6);
            this.cmdBrowseData.Name = "cmdBrowseData";
            this.cmdBrowseData.Size = new System.Drawing.Size(27, 23);
            this.cmdBrowseData.TabIndex = 18;
            this.cmdBrowseData.Text = "...";
            this.cmdBrowseData.UseVisualStyleBackColor = true;
            this.cmdBrowseData.Click += new System.EventHandler(this.cmdBrowseData_Click);
            // 
            // txtOutputRoot
            // 
            this.txtOutputRoot.Location = new System.Drawing.Point(114, 38);
            this.txtOutputRoot.Name = "txtOutputRoot";
            this.txtOutputRoot.Size = new System.Drawing.Size(371, 20);
            this.txtOutputRoot.TabIndex = 17;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 13);
            this.label2.TabIndex = 16;
            this.label2.Text = "Root Output Folder";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 13);
            this.label1.TabIndex = 15;
            this.label1.Text = "Root Data Folder";
            // 
            // txtDataRoot
            // 
            this.txtDataRoot.Location = new System.Drawing.Point(114, 8);
            this.txtDataRoot.Name = "txtDataRoot";
            this.txtDataRoot.Size = new System.Drawing.Size(371, 20);
            this.txtDataRoot.TabIndex = 14;
            // 
            // OptionsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.cmdBrowseToolDir);
            this.Controls.Add(this.txtToolDir);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cmdBrowseOutput);
            this.Controls.Add(this.cmdBrowseData);
            this.Controls.Add(this.txtOutputRoot);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtDataRoot);
            this.Name = "OptionsControl";
            this.Size = new System.Drawing.Size(549, 198);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdBrowseToolDir;
        private System.Windows.Forms.TextBox txtToolDir;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button cmdBrowseOutput;
        private System.Windows.Forms.Button cmdBrowseData;
        private System.Windows.Forms.TextBox txtOutputRoot;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtDataRoot;
    }
}
