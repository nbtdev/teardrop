using System;
using System.Collections.Generic;
using System.Text;
using WeifenLuo.WinFormsUI.Docking;

using CoS;
using CoS.Gfx;
using CoS.Math;
using CoS.Game;

namespace SceneEditor
{
    public class View : DockContent
    {
        private CoS.Controls.RenderWindow renderWindow1;
        CameraController m_camCtl = null;
        Camera m_cam = null;
        Viewport m_vp = null;
        Scene m_scene = null;
        IMessageBus m_msgBus;

        public View(IMessageBus msgBus)
        {
            m_msgBus = msgBus;

            InitializeComponent();
            SetStyle(System.Windows.Forms.ControlStyles.AllPaintingInWmPaint | System.Windows.Forms.ControlStyles.Opaque, true);

            msgBus.SceneLoaded += new EventHandler<SceneEventArgs>(HandleSceneLoaded);
            msgBus.Timer += new EventHandler<TimerEventArgs>(OnTimer);
        }

        void HandleSceneLoaded(object sender, SceneEventArgs e)
        {
            m_scene = e.Scene;
        }

        public void Initialize()
        {
            m_cam = new Camera();
            m_camCtl = new FreeCamController(m_cam);
            m_vp = View3D.GfxWindow.AddViewport(0, 0, 1, 1);

            m_cam.Position = new Vector4(100, 100, 100, 0);
            m_cam.Orientation = Quaternion.IDENTITY;
            m_cam.LookAt = Vector4.ZERO;
        }

        private void InitializeComponent()
        {
            this.renderWindow1 = new CoS.Controls.RenderWindow();
            this.SuspendLayout();
            // 
            // renderWindow1
            // 
            this.renderWindow1.BackColor = System.Drawing.Color.DimGray;
            this.renderWindow1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.renderWindow1.GfxWindow = null;
            this.renderWindow1.Location = new System.Drawing.Point(0, 0);
            this.renderWindow1.Name = "renderWindow1";
            this.renderWindow1.Renderer = null;
            this.renderWindow1.Size = new System.Drawing.Size(292, 266);
            this.renderWindow1.TabIndex = 0;
            // 
            // RenderWindow
            // 
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.renderWindow1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "RenderWindow";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.RenderWindow_Paint);
            this.ResumeLayout(false);

        }

        public CoS.Controls.RenderWindow View3D
        {
            get
            {
                return renderWindow1;
            }
        }

        public bool HandleKeyEvent(object sender, System.Windows.Forms.KeyEventArgs args)
        {
            return false;
        }

        public bool HandleMouseEvent(object sender, System.Windows.Forms.MouseEventArgs args)
        {
            return false;
        }

        void OnTimer(object sender, TimerEventArgs e)
        {
            // skip it if there's no scene
            if (m_scene != null)
            {
                // update any current camera controller
                m_camCtl.Update(e.DeltaT);

                // find visible objects in the scene...
                List<CoS.Object> objs = m_scene.GetVisibleObjects(m_cam);
                foreach (CoS.Object obj in objs)
                {
                    MeshInstance inst = ZoneObject.GetRenderable(obj);
                    if (inst != null)
                    {
                        renderWindow1.Renderer.QueueForRendering(inst);
                    }
                }
            }
        }

        private void RenderWindow_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            // and tickle the timer event
            m_msgBus.RaiseTimerTick(this);

            renderWindow1.Renderer.SetViewport(m_vp);
            renderWindow1.Renderer.SetCamera(m_cam);

            if (renderWindow1 != null)
                renderWindow1.Render();

            // causes window to be redrawn constantly (C# version of the "main loop")
            Invalidate();
        }
    }
}
