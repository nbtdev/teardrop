using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor
{
    public interface EditorMode
    {
        bool HandleKeyEvent(object sender, System.Windows.Forms.KeyEventArgs args);
        bool HandleMouseEvent(object sender, System.Windows.Forms.MouseEventArgs args);
    }
}
