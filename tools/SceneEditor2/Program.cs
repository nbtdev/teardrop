using System;
using System.Collections.Generic;
using System.Windows.Forms;
using CoS.Marshal;

namespace SceneEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            CoS.Runtime rt = new CoS.Runtime();
            // rt is initialized in MainForm ctor -- hacky 

            Application.Run(new MainForm(rt));
            
            rt.Shutdown();
            rt.Dispose();
        }
    }
}
