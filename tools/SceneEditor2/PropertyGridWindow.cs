using System;
using System.Collections.Generic;
using System.Text;
using WeifenLuo.WinFormsUI.Docking;

namespace SceneEditor
{
    public class PropertyGridWindow : DockContent
    {
        private CoS.Controls.PropertyGrid propertyGrid1;

        public PropertyGridWindow()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.propertyGrid1 = new CoS.Controls.PropertyGrid();
            this.SuspendLayout();
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid1.Location = new System.Drawing.Point(0, 0);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(292, 266);
            this.propertyGrid1.TabIndex = 0;
            // 
            // ToolWindow
            // 
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.propertyGrid1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "ToolWindow";
            this.ResumeLayout(false);

        }

        public CoS.Controls.PropertyGrid PropertyGrid
        {
            get
            {
                return propertyGrid1;
            }
        }
    }
}
