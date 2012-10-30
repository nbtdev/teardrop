using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Reflection;

using WeifenLuo.WinFormsUI.Docking;

using CoS.Math;
using CoS.Game;
using CoS.Gfx;
using CoS;

namespace SceneEditor
{
    public partial class MainForm : Form, IMessageBus
    {
        #region Fields
        Runtime m_rt = null;
        double m_time = 0;
        double m_accumTime = 0;
        System.Diagnostics.Stopwatch m_timer = System.Diagnostics.Stopwatch.StartNew();
        const double TIMER_INTERVAL = 0.010;

        const string APP_DATA = "CoS\\.SceneEditor";
        const string LAYOUT_CONFIG = "layout.xml";

        CoS.Controls.TreeView m_treeView = null;
        CoS.Controls.PropertyGrid m_propGrid = null;

        PropertyGridWindow m_propGridWindow = null;
        TreeViewWindow m_treeViewWindow = null;
        View m_view3D = null;
        View m_activeView = null;

        EditorMode m_mode = null;

        CoS.Gfx.Renderer m_renderer = null;
        CoS.Gfx.RenderWindow m_renderWindow = null;
        CoS.Game.Scene m_scene = null;

        System.Windows.Forms.ToolStrip m_currentToolBar = null;
        // this is just to have a reference to the toolbar button groups, which otherwise operate autonomously
        System.Collections.ArrayList m_toolbarButtonGroups = new ArrayList();
        #endregion

        #region IMessageBus Members
        public event EventHandler<SelectedObjectChangedArgs> SelectedObjectChanged;
        public void RaiseSelectedObjectChanged(object sender, CoS.Object obj)
        {
            if (SelectedObjectChanged != null)
                SelectedObjectChanged(sender, new SelectedObjectChangedArgs(obj));
        }
        public event EventHandler<SceneEventArgs> SceneLoaded;
        public void RaiseSceneLoaded(object sender, CoS.Game.Scene scene)
        {
            if (SceneLoaded != null)
                SceneLoaded(sender, new SceneEventArgs(scene));
        }
        public event EventHandler<SceneEventArgs> SceneUnloaded;
        public void RaiseSceneUnloaded(object sender, CoS.Game.Scene scene)
        {
            if (SceneUnloaded != null)
                SceneUnloaded(sender, new SceneEventArgs(scene));
        }
        public event EventHandler<SceneEventArgs> SceneSaved;
        public void RaiseSceneSaved(object sender, CoS.Game.Scene scene)
        {
            if (SceneSaved != null)
                SceneSaved(sender, new SceneEventArgs(scene));
        }
        public event EventHandler<TimerEventArgs> Timer;
        public void RaiseTimerTick(object sender)
        {
            // first see how much time has elapsed since the last raise
            // and if it's beyond our interval, raise an event
            double time = m_timer.ElapsedTicks / (double)System.Diagnostics.Stopwatch.Frequency;
            double elapsed = time - m_time;
            m_time = time;
            m_accumTime += elapsed;

            while (m_accumTime > TIMER_INTERVAL)
            {
                m_accumTime -= TIMER_INTERVAL;

                if (m_scene != null)
                    m_scene.Update(TIMER_INTERVAL);

                if (Timer != null)
                    Timer(sender, new TimerEventArgs(TIMER_INTERVAL));
            }
        }
        #endregion

        public MainForm(Runtime rt)
        {
            m_rt = rt;

            InitializeComponent();

            m_dock.DocumentStyle = DocumentStyle.DockingSdi;

            // load previous UI layout if present
            //m_dock.LoadFromXml(getLayoutConfigPath(), 
            {
                m_propGridWindow = new PropertyGridWindow();
                m_propGridWindow.Show(m_dock, DockState.DockRight);

                m_treeViewWindow = new TreeViewWindow();
                m_treeViewWindow.Show(m_propGridWindow.Pane, DockAlignment.Top, 0.5);

                m_view3D = new View(this);
                m_view3D.Show(m_dock, DockState.Document);

                m_propGrid = m_propGridWindow.PropertyGrid;
                m_treeView = m_treeViewWindow.TreeView;
            }

            rt.RootWindow = m_view3D.View3D;
            rt.Initialize(); // shutdown is called in class Dispose() method
            m_view3D.View3D.GfxWindow = rt.RenderWindow;
            m_view3D.Initialize();
            m_renderWindow = rt.RenderWindow;
            m_view3D.View3D.Renderer = rt.Renderer;
            m_renderer = rt.Renderer;

            // finish setting up the UI
            m_toolStripMode.Items.Add(new DropDownItem("Object", m_toolsObject));
            m_toolStripMode.Items.Add(new DropDownItem("Terrain", m_toolsTerrain));
            m_toolStripMode.SelectedIndex = 0;
            m_currentToolBar = m_toolsObject; // todo: get this from saved settings?
            m_currentToolBar.Items.Insert(0, m_toolStripMode); // removes it from m_tools
            m_tools.Visible = false;
            m_currentToolBar.Visible = true;
            m_toolStripMode.SelectedIndexChanged += new EventHandler(ToolStripMode_SelectedIndexChanged);
            m_dock.BringToFront();

            // group buttons where applicable
            // todo: move to ObjectEditor.cs file?
            ButtonGroup grp = new ButtonGroup();
            grp.Add(m_cmdObjectSelect); m_cmdObjectSelect.Tag = ObjectMode.ToolType.SELECT;
            grp.Add(m_cmdObjectMove); m_cmdObjectMove.Tag = ObjectMode.ToolType.MOVE;
            grp.Add(m_cmdObjectRotate); m_cmdObjectRotate.Tag = ObjectMode.ToolType.ROTATE;
            grp.Add(m_cmdObjectScale); m_cmdObjectScale.Tag = ObjectMode.ToolType.SCALE;
            m_toolbarButtonGroups.Add(grp);
            m_cmdObjectPlace.Tag = ObjectMode.ToolType.PLACE;

            m_mode = new ObjectMode(m_currentToolBar, m_renderWindow);
            m_cmdObjectSelect.Checked = true; // default to "Select" mode

            // set the active view
            m_activeView = m_view3D;

            // hook up event handlers
            this.KeyUp += new KeyEventHandler(MainForm_KeyEvent);
            this.KeyDown += new KeyEventHandler(MainForm_KeyEvent);
            this.MouseMove += new MouseEventHandler(MainForm_MouseEvent);
            this.MouseUp += new MouseEventHandler(MainForm_MouseEvent);
            this.MouseDown += new MouseEventHandler(MainForm_MouseEvent);
        }

        void MainForm_MouseEvent(object sender, MouseEventArgs e)
        {
            if (m_mode != null)
            {
                if (!m_mode.HandleMouseEvent(sender, e))
                    HandleMouseEvent(sender, e);
            }
        }

        void MainForm_KeyEvent(object sender, KeyEventArgs e)
        {
            if (m_mode != null)
            {
                if (!m_mode.HandleKeyEvent(sender, e))
                    HandleKeyEvent(sender, e);
            }
        }

        void HandleMouseEvent(object sender, MouseEventArgs e)
        {
            if (m_activeView != null)
                m_activeView.HandleMouseEvent(sender, e);
        }

        void HandleKeyEvent(object sender, KeyEventArgs e)
        {
            if (m_activeView != null)
                m_activeView.HandleKeyEvent(sender, e);
        }

        void ToolStripMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            ToolStripComboBox cb = (ToolStripComboBox)sender;
            DropDownItem item = (DropDownItem)cb.SelectedItem;
            ToolStrip strip = (ToolStrip)item.Data;

            // move the dropdown over to the new one...
            strip.Items.Insert(0, m_currentToolBar.Items[0]);

            // and then swap the new and current ones
            strip.Visible = true;
            m_currentToolBar.Visible = false;
            m_currentToolBar = strip;

            // because the WeifenLuo dock control and toolbars/menustrips don't get along...
            m_dock.BringToFront();
        }

        string getUserDataPath()
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            path += "\\";
            path += APP_DATA;

            return path;
        }

        string getLayoutConfigPath()
        {
            string path = getUserDataPath();
            path += "\\";
            path += LAYOUT_CONFIG;

            return path;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        private void openSceneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // pop up the scene list dialog box
            SceneChooser dlg = new SceneChooser(m_rt.Manifest);
            if (dlg.ShowDialog(this) == DialogResult.OK)
            {
                SceneChooser.Data data = dlg.SelectedItem;
                Manifest.Entry ent = m_rt.Manifest.GetEntry(data.ToString());
                string path = Runtime.DataPath;
                path += ent.val;
                path += "/scene.xml";

                m_scene = new Scene();
                m_scene.Initialize();

                if (!m_scene.Load(path))
                {
                    MessageBox.Show(String.Format("Could not load scene file at \"{0}\"", path));
                    return;
                }

                // otherwise, broadcast the good news
                RaiseSceneLoaded(this, m_scene);
            }
        }
    }

    /// <summary>
    /// Helper class for drop-down combo-box items (make it easy to 
    /// associate data with an item)
    /// </summary>
    internal class DropDownItem
    {
        string m_label;
        object m_data;

        public DropDownItem(string label, object data)
        {
            m_label = label;
            m_data = data;
        }

        public object Data
        {
            get
            {
                return m_data;
            }
            set
            {
                m_data = value;
            }
        }

        public override string ToString()
        {
            return m_label;
        }
    }
}
