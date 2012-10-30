using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;

namespace SceneEditor
{
    /// <summary>
    /// Class for managing toolbar button states for groups
    /// of tri-state buttons that should exhibit radio-button behavior
    /// </summary>
    public class ButtonGroup : List<System.Windows.Forms.ToolStripButton>
    {
        public new void Add(System.Windows.Forms.ToolStripButton button)
        {
            button.CheckedChanged += new EventHandler(CheckedChanged);
            base.Add(button);
        }

        void CheckedChanged(object sender, EventArgs e)
        {
            if (!((System.Windows.Forms.ToolStripButton)sender).Checked)
                return;

            foreach (System.Windows.Forms.ToolStripButton button in this)
            {
                if (button != sender)
                {
                    // uncheck it
                    button.Checked = false;
                }
                else
                {
                    button.Checked = true;
                }
            }
        }
    }
}
