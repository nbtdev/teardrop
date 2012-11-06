/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DeveloperControlPanel
{
    public partial class dlgAbout : Form
    {
        public dlgAbout()
        {
            InitializeComponent();
        }

        private void dlgAbout_Load(object sender, EventArgs e)
        {
            txtProduct.Text = Application.ProductName;
            txtVersion.Text = Application.ProductVersion;
        }
    }
}