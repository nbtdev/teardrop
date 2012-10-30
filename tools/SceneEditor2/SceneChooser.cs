using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using CoS;
using CoS.Game;

namespace SceneEditor
{
    public partial class SceneChooser : Form
    {
        public struct Data
        {
            string m_name;
            object m_data;

            public Data(string name, object data)
            {
                m_name = name;
                m_data = data;
            }

            public object ItemData
            {
                get
                {
                    return m_data;
                }
            }

            public override string ToString()
            {
                return m_name;
            }
        }

        public SceneChooser(Manifest m)
        {
            InitializeComponent();

            // load up the list with scenes
            System.Collections.ArrayList scenes = m.GetEntriesForClassType("Scene");

            foreach (Manifest.Entry ent in scenes)
            {
                m_listScenes.Items.Add(new Data(ent.val, ent));
            }
        }

        public Data SelectedItem
        {
            get
            {
                return (Data)m_listScenes.SelectedItem;
            }
        }
    }
}
