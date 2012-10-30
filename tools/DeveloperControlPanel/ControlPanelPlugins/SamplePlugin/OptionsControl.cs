/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

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
