using CoS;

namespace SceneEditor
{
    partial class MainForm
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
            if (m_scene != null)
                m_scene.Destroy();

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
            WeifenLuo.WinFormsUI.Docking.DockPanelSkin dockPanelSkin1 = new WeifenLuo.WinFormsUI.Docking.DockPanelSkin();
            WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin autoHideStripSkin1 = new WeifenLuo.WinFormsUI.Docking.AutoHideStripSkin();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient1 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin dockPaneStripSkin1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripSkin();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient dockPaneStripGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient2 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient2 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient3 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient dockPaneStripToolWindowGradient1 = new WeifenLuo.WinFormsUI.Docking.DockPaneStripToolWindowGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient4 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient5 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.DockPanelGradient dockPanelGradient3 = new WeifenLuo.WinFormsUI.Docking.DockPanelGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient6 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            WeifenLuo.WinFormsUI.Docking.TabGradient tabGradient7 = new WeifenLuo.WinFormsUI.Docking.TabGradient();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.m_dock = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openSceneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_status = new System.Windows.Forms.StatusStrip();
            this.m_tools = new System.Windows.Forms.ToolStrip();
            this.m_toolStripMode = new System.Windows.Forms.ToolStripComboBox();
            this.m_toolsTerrain = new System.Windows.Forms.ToolStrip();
            this.m_terrainToolA = new System.Windows.Forms.ToolStripButton();
            this.m_terrainToolB = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.m_terrainToolC = new System.Windows.Forms.ToolStripButton();
            this.m_toolsObject = new System.Windows.Forms.ToolStrip();
            this.m_cmdObjectSelect = new System.Windows.Forms.ToolStripButton();
            this.m_cmdObjectMove = new System.Windows.Forms.ToolStripButton();
            this.m_cmdObjectRotate = new System.Windows.Forms.ToolStripButton();
            this.m_cmdObjectScale = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.m_cmdObjectPlace = new System.Windows.Forms.ToolStripButton();
            this.menuStrip1.SuspendLayout();
            this.m_tools.SuspendLayout();
            this.m_toolsTerrain.SuspendLayout();
            this.m_toolsObject.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_dock
            // 
            this.m_dock.ActiveAutoHideContent = null;
            this.m_dock.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_dock.DockBackColor = System.Drawing.SystemColors.AppWorkspace;
            this.m_dock.DocumentStyle = WeifenLuo.WinFormsUI.Docking.DocumentStyle.DockingSdi;
            this.m_dock.Location = new System.Drawing.Point(0, 49);
            this.m_dock.Name = "m_dock";
            this.m_dock.Size = new System.Drawing.Size(864, 548);
            dockPanelGradient1.EndColor = System.Drawing.SystemColors.ControlLight;
            dockPanelGradient1.StartColor = System.Drawing.SystemColors.ControlLight;
            autoHideStripSkin1.DockStripGradient = dockPanelGradient1;
            tabGradient1.EndColor = System.Drawing.SystemColors.Control;
            tabGradient1.StartColor = System.Drawing.SystemColors.Control;
            tabGradient1.TextColor = System.Drawing.SystemColors.ControlDarkDark;
            autoHideStripSkin1.TabGradient = tabGradient1;
            dockPanelSkin1.AutoHideStripSkin = autoHideStripSkin1;
            tabGradient2.EndColor = System.Drawing.SystemColors.ControlLightLight;
            tabGradient2.StartColor = System.Drawing.SystemColors.ControlLightLight;
            tabGradient2.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripGradient1.ActiveTabGradient = tabGradient2;
            dockPanelGradient2.EndColor = System.Drawing.SystemColors.Control;
            dockPanelGradient2.StartColor = System.Drawing.SystemColors.Control;
            dockPaneStripGradient1.DockStripGradient = dockPanelGradient2;
            tabGradient3.EndColor = System.Drawing.SystemColors.ControlLight;
            tabGradient3.StartColor = System.Drawing.SystemColors.ControlLight;
            tabGradient3.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripGradient1.InactiveTabGradient = tabGradient3;
            dockPaneStripSkin1.DocumentGradient = dockPaneStripGradient1;
            tabGradient4.EndColor = System.Drawing.SystemColors.ActiveCaption;
            tabGradient4.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
            tabGradient4.StartColor = System.Drawing.SystemColors.GradientActiveCaption;
            tabGradient4.TextColor = System.Drawing.SystemColors.ActiveCaptionText;
            dockPaneStripToolWindowGradient1.ActiveCaptionGradient = tabGradient4;
            tabGradient5.EndColor = System.Drawing.SystemColors.Control;
            tabGradient5.StartColor = System.Drawing.SystemColors.Control;
            tabGradient5.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripToolWindowGradient1.ActiveTabGradient = tabGradient5;
            dockPanelGradient3.EndColor = System.Drawing.SystemColors.ControlLight;
            dockPanelGradient3.StartColor = System.Drawing.SystemColors.ControlLight;
            dockPaneStripToolWindowGradient1.DockStripGradient = dockPanelGradient3;
            tabGradient6.EndColor = System.Drawing.SystemColors.GradientInactiveCaption;
            tabGradient6.LinearGradientMode = System.Drawing.Drawing2D.LinearGradientMode.Vertical;
            tabGradient6.StartColor = System.Drawing.SystemColors.GradientInactiveCaption;
            tabGradient6.TextColor = System.Drawing.SystemColors.ControlText;
            dockPaneStripToolWindowGradient1.InactiveCaptionGradient = tabGradient6;
            tabGradient7.EndColor = System.Drawing.Color.Transparent;
            tabGradient7.StartColor = System.Drawing.Color.Transparent;
            tabGradient7.TextColor = System.Drawing.SystemColors.ControlDarkDark;
            dockPaneStripToolWindowGradient1.InactiveTabGradient = tabGradient7;
            dockPaneStripSkin1.ToolWindowGradient = dockPaneStripToolWindowGradient1;
            dockPanelSkin1.DockPaneStripSkin = dockPaneStripSkin1;
            this.m_dock.Skin = dockPanelSkin1;
            this.m_dock.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(864, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openSceneToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // openSceneToolStripMenuItem
            // 
            this.openSceneToolStripMenuItem.Name = "openSceneToolStripMenuItem";
            this.openSceneToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.openSceneToolStripMenuItem.Text = "&Open Scene...";
            this.openSceneToolStripMenuItem.Click += new System.EventHandler(this.openSceneToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(152, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.windowToolStripMenuItem.Text = "&Window";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // m_status
            // 
            this.m_status.Location = new System.Drawing.Point(0, 597);
            this.m_status.Name = "m_status";
            this.m_status.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.m_status.Size = new System.Drawing.Size(864, 22);
            this.m_status.TabIndex = 3;
            this.m_status.Text = "statusStrip1";
            // 
            // m_tools
            // 
            this.m_tools.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_toolStripMode});
            this.m_tools.Location = new System.Drawing.Point(0, 24);
            this.m_tools.Name = "m_tools";
            this.m_tools.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.m_tools.Size = new System.Drawing.Size(864, 25);
            this.m_tools.TabIndex = 4;
            this.m_tools.Text = "toolStrip1";
            // 
            // m_toolStripMode
            // 
            this.m_toolStripMode.Name = "m_toolStripMode";
            this.m_toolStripMode.Size = new System.Drawing.Size(121, 25);
            // 
            // m_toolsTerrain
            // 
            this.m_toolsTerrain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_terrainToolA,
            this.m_terrainToolB,
            this.toolStripSeparator3,
            this.m_terrainToolC});
            this.m_toolsTerrain.Location = new System.Drawing.Point(0, 49);
            this.m_toolsTerrain.Name = "m_toolsTerrain";
            this.m_toolsTerrain.Size = new System.Drawing.Size(864, 25);
            this.m_toolsTerrain.TabIndex = 5;
            this.m_toolsTerrain.Text = "toolStrip1";
            this.m_toolsTerrain.Visible = false;
            // 
            // m_terrainToolA
            // 
            this.m_terrainToolA.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_terrainToolA.Image = ((System.Drawing.Image)(resources.GetObject("m_terrainToolA.Image")));
            this.m_terrainToolA.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_terrainToolA.Name = "m_terrainToolA";
            this.m_terrainToolA.Size = new System.Drawing.Size(23, 22);
            this.m_terrainToolA.Text = "TerrainToolA";
            // 
            // m_terrainToolB
            // 
            this.m_terrainToolB.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_terrainToolB.Image = ((System.Drawing.Image)(resources.GetObject("m_terrainToolB.Image")));
            this.m_terrainToolB.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_terrainToolB.Name = "m_terrainToolB";
            this.m_terrainToolB.Size = new System.Drawing.Size(23, 22);
            this.m_terrainToolB.Text = "TerrainToolB";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // m_terrainToolC
            // 
            this.m_terrainToolC.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_terrainToolC.Image = ((System.Drawing.Image)(resources.GetObject("m_terrainToolC.Image")));
            this.m_terrainToolC.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_terrainToolC.Name = "m_terrainToolC";
            this.m_terrainToolC.Size = new System.Drawing.Size(23, 22);
            this.m_terrainToolC.Text = "TerrainToolC";
            // 
            // m_toolsObject
            // 
            this.m_toolsObject.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_cmdObjectSelect,
            this.m_cmdObjectMove,
            this.m_cmdObjectRotate,
            this.m_cmdObjectScale,
            this.toolStripSeparator2,
            this.m_cmdObjectPlace});
            this.m_toolsObject.Location = new System.Drawing.Point(0, 49);
            this.m_toolsObject.Name = "m_toolsObject";
            this.m_toolsObject.Size = new System.Drawing.Size(864, 25);
            this.m_toolsObject.TabIndex = 6;
            this.m_toolsObject.Text = "toolStrip1";
            this.m_toolsObject.Visible = false;
            // 
            // m_cmdObjectSelect
            // 
            this.m_cmdObjectSelect.CheckOnClick = true;
            this.m_cmdObjectSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_cmdObjectSelect.Image = ((System.Drawing.Image)(resources.GetObject("m_cmdObjectSelect.Image")));
            this.m_cmdObjectSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_cmdObjectSelect.Name = "m_cmdObjectSelect";
            this.m_cmdObjectSelect.Size = new System.Drawing.Size(23, 22);
            this.m_cmdObjectSelect.Text = "Select";
            // 
            // m_cmdObjectMove
            // 
            this.m_cmdObjectMove.CheckOnClick = true;
            this.m_cmdObjectMove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_cmdObjectMove.Image = ((System.Drawing.Image)(resources.GetObject("m_cmdObjectMove.Image")));
            this.m_cmdObjectMove.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_cmdObjectMove.Name = "m_cmdObjectMove";
            this.m_cmdObjectMove.Size = new System.Drawing.Size(23, 22);
            this.m_cmdObjectMove.Text = "Move";
            // 
            // m_cmdObjectRotate
            // 
            this.m_cmdObjectRotate.CheckOnClick = true;
            this.m_cmdObjectRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_cmdObjectRotate.Image = ((System.Drawing.Image)(resources.GetObject("m_cmdObjectRotate.Image")));
            this.m_cmdObjectRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_cmdObjectRotate.Name = "m_cmdObjectRotate";
            this.m_cmdObjectRotate.Size = new System.Drawing.Size(23, 22);
            this.m_cmdObjectRotate.Text = "Rotate";
            // 
            // m_cmdObjectScale
            // 
            this.m_cmdObjectScale.CheckOnClick = true;
            this.m_cmdObjectScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_cmdObjectScale.Image = ((System.Drawing.Image)(resources.GetObject("m_cmdObjectScale.Image")));
            this.m_cmdObjectScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_cmdObjectScale.Name = "m_cmdObjectScale";
            this.m_cmdObjectScale.Size = new System.Drawing.Size(23, 22);
            this.m_cmdObjectScale.Text = "Scale";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // m_cmdObjectPlace
            // 
            this.m_cmdObjectPlace.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.m_cmdObjectPlace.Image = ((System.Drawing.Image)(resources.GetObject("m_cmdObjectPlace.Image")));
            this.m_cmdObjectPlace.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.m_cmdObjectPlace.Name = "m_cmdObjectPlace";
            this.m_cmdObjectPlace.Size = new System.Drawing.Size(23, 22);
            this.m_cmdObjectPlace.Text = "Place";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(864, 619);
            this.Controls.Add(this.m_toolsObject);
            this.Controls.Add(this.m_toolsTerrain);
            this.Controls.Add(this.m_dock);
            this.Controls.Add(this.m_status);
            this.Controls.Add(this.m_tools);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Scene Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.m_tools.ResumeLayout(false);
            this.m_tools.PerformLayout();
            this.m_toolsTerrain.ResumeLayout(false);
            this.m_toolsTerrain.PerformLayout();
            this.m_toolsObject.ResumeLayout(false);
            this.m_toolsObject.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private WeifenLuo.WinFormsUI.Docking.DockPanel m_dock;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.StatusStrip m_status;
        private System.Windows.Forms.ToolStrip m_tools;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSceneToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox m_toolStripMode;
        private System.Windows.Forms.ToolStrip m_toolsTerrain;
        private System.Windows.Forms.ToolStripButton m_terrainToolA;
        private System.Windows.Forms.ToolStripButton m_terrainToolB;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton m_terrainToolC;
        private System.Windows.Forms.ToolStrip m_toolsObject;
        private System.Windows.Forms.ToolStripButton m_cmdObjectSelect;
        private System.Windows.Forms.ToolStripButton m_cmdObjectMove;
        private System.Windows.Forms.ToolStripButton m_cmdObjectRotate;
        private System.Windows.Forms.ToolStripButton m_cmdObjectScale;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton m_cmdObjectPlace;


    }
}

