using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor
{
    public class SelectTool : Tool
    {
        public bool HandleKeyEvent(object sender, System.Windows.Forms.KeyEventArgs args)
        {
            return false;
        }

        public bool HandleMouseEvent(object sender, System.Windows.Forms.MouseEventArgs args)
        {
            return false;
        }
    }
}
