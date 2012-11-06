/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace SamplePlugin
{
    public partial class OptionsControl : DeveloperControlPanel.Plugin.OptionsControl
    {
        public OptionsControl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Called by the framework to check to see if it's 
        /// ok to apply settings to all active options controls. 
        /// User controls can veto the apply by returning false.
        /// </summary>
        /// <returns></returns>
        public override bool BeforeApplyOptions()
        {
            return true;
        }

        /// <summary>
        /// Called by the framework to check to see if it's 
        /// ok to close the options dialog.
        /// User controls can veto the close by returning false.
        /// </summary>
        /// <returns></returns>
        public override bool BeforeCloseOptions()
        {
            return true;
        }

        /// <summary>
        /// Called by the framework when the user presses the Apply 
        /// button on the Options form
        /// </summary>
        public override void OnApplyOptions()
        {

        }

        /// <summary>
        /// Called by the framework when the user presses the Close
        /// button on the Options form
        /// </summary>
        public override void OnCloseOptions()
        {

        }

        /// <summary>
        /// Called by the framework when the user presses the Cancel 
        /// button on the Options form
        /// </summary>
        public override void OnCancelOptions()
        {

        }
    }
}
