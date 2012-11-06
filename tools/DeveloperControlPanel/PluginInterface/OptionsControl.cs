/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace DeveloperControlPanel.Plugin
{
    public class OptionsControl : System.Windows.Forms.UserControl
    {
        public delegate void OptionsChangedHandler(object sender, EventArgs args);
        public event OptionsChangedHandler OnOptionsChanged;
        protected void RaiseOptionsChanged(OptionsControl sender)
        {
            if (OnOptionsChanged != null)
            {
                OnOptionsChanged(sender, null);
            }
        }

        protected bool m_bDirty = false;
        public bool Dirty
        {
            get
            {
                return m_bDirty;
            }
        }

        public virtual bool BeforeApplyOptions() { throw new Exception("Not implemented"); }
        public virtual bool BeforeCloseOptions() { throw new Exception("Not implemented"); }
        public virtual void OnApplyOptions() { throw new Exception("Not implemented"); }
        public virtual void OnCloseOptions() { throw new Exception("Not implemented"); }
        public virtual void OnCancelOptions() { throw new Exception("Not implemented"); }
    }
}
