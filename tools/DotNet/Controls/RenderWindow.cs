using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using CoS;
using CoS.Gfx;

namespace CoS.Controls
{
    public partial class RenderWindow : Panel
    {
        #region Events and Delegates
        #endregion

        #region Fields
        Gfx.RenderWindow m_renderWindow;
        Gfx.Renderer m_renderer;
        #endregion

        #region Properties
        public Gfx.Renderer Renderer
        {
            get
            {
                return m_renderer;
            }
            set
            {
                m_renderer = value;
            }
        }

        public Gfx.RenderWindow GfxWindow
        {
            get
            {
                return m_renderWindow;
            }
            set
            {
                m_renderWindow = value;
            }
        }
        #endregion

        #region Methods, Constructor(s) and Event Handlers
        public RenderWindow()
        {
            InitializeComponent();

            this.Resize += new EventHandler(RenderWindow_Resize);
        }

        void RenderWindow_Resize(object sender, EventArgs e)
        {
            if (m_renderWindow != null)
                m_renderWindow.Resize(Size.Width, Size.Height);
        }

        public void Render()
        {
            if (m_renderer == null)
                return;

            // render a frame in the 3D view
            m_renderer.SetRenderTarget(m_renderWindow);
            m_renderer.BeginFrame();
            m_renderer.Flush();
            m_renderer.EndFrame();
            m_renderer.Swap();
        }
        #endregion

        #region Private Methods
        #endregion
    }
}
