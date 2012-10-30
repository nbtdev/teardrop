using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Reflection;

using CoS.Math;
using CoS.Game;
using CoS;

namespace SceneEditor2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            rt = new Runtime(); // defined in other part of class
            rt.Initialize(); // shutdown is called in class Dispose() method

            Vector4 vecY = new Vector4(-.7071f, .7071f, 0, 0);
            Vector4 vecX = Vector4.UNIT_X;

            float d = Vector4.Dot(vecY, vecX);
            Vector4 vec = Vector4.Cross(vecX, vecY);

            Manifest manifest = rt.Manifest;
            ArrayList ents = manifest.GetEntriesForClassType("MACRO");
            foreach (Manifest.Entry ent in ents)
            {
                string s = ent.val;

                // create a MACRO and then break out
                CoS.Object obj = ObjectLibrary.Create("MACRO", s);
                PropertyInfo pi = obj.GetType().GetProperty("Transform");
                Transform t = new Transform(new Vector4(100, 30, 200, 0), new Quaternion(0, 0, 0, 1), new Vector4(1, 1, 1, 0));
                pi.SetValue(obj, t, null);
                propertyGrid1.SelectedObject = obj;
            }
        }
    }
}
