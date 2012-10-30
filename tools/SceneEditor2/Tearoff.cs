using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SceneEditor
{
    /// <summary>
    /// This class takes any Panel instance and reparents it to a 
    /// new overlapping window (for instance, if the user wants to
    /// remove a render window to a non-child of the main frame)
    /// </summary>
    public partial class Tearoff : Form
    {
        Panel m_panel = null;
        Control m_originalParent = null;
        Point m_p;

        public Tearoff(Panel panel)
        {
            InitializeComponent();

            m_originalParent = panel.Parent;
            m_p = panel.Location;
            m_panel = panel;
            panel.Parent = this;
            panel.Location = new Point(0, 0);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // put it back where it was before
            m_panel.Parent = m_originalParent;
            m_panel.Location = m_p;
        }
    }
}
