using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using CoS;
using CoS.Gfx;
using CoS.Math;

namespace SceneEditor
{
    /// <summary>
    /// Class to manage selection and manipulation of objects
    /// in a loaded scene. 
    /// </summary>
    public class ObjectMode : EditorMode
    {
        #region Enums
        public enum ToolType
        {
            NONE,
            SELECT,
            MOVE,
            ROTATE,
            SCALE,
            PLACE,
        }
        #endregion

        #region Events and Delegates
        #endregion

        #region Fields
        Tool m_tool = null;
        CoS.Gfx.RenderWindow m_window = null;
        ToolType m_toolType = ToolType.NONE;
        #endregion

        #region Properties
        #endregion

        #region Methods, Constructor(s) and Event Handlers
        public ObjectMode(ToolStrip toolbar, CoS.Gfx.RenderWindow win)
        {
            m_window = win;

            foreach (ToolStripItem item in toolbar.Items)
            {
                if (item.GetType() == typeof(ToolStripButton))
                {
                    // hook into its check-state change event
                    ToolStripButton btn = item as ToolStripButton;
                    btn.CheckedChanged += new EventHandler(ToolButtonCheckedChanged);
                }
            }
        }

        /// <summary>
        ///  Called by a toolbar button when its checkstate changes -- we
        ///  only care about and act on those who check state is changed to 
        ///  "true".
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void ToolButtonCheckedChanged(object sender, EventArgs e)
        {
            ToolStripButton btn = sender as ToolStripButton;
            if (btn != null)
            {
                if (btn.Checked)
                {
                    // then update the current mode tool
                    m_toolType = (ToolType)btn.Tag;

                    switch (m_toolType)
                    {
                        case ToolType.SELECT:
                            m_tool = new SelectTool();
                            break;

                        default:
                            m_tool = null;
                            break;
                    }
                }
            }
        }
        #endregion

        #region Private Methods
        #endregion

        #region Overrides
        public bool HandleKeyEvent(object sender, System.Windows.Forms.KeyEventArgs args)
        {
            if (m_tool != null)
                return m_tool.HandleKeyEvent(sender, args);

            return false;
        }

        public bool HandleMouseEvent(object sender, System.Windows.Forms.MouseEventArgs args)
        {
            if (m_tool != null)
                return m_tool.HandleMouseEvent(sender, args);

            return false;
        }
        #endregion
    }
}
