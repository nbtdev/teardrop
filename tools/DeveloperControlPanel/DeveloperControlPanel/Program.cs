/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DeveloperControlPanel
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

            KillAnyProcessThatIsMe();

            bool doRun = true;
            while (doRun)
            {
                //try
                {
                    Application.Run(new Main());
                    doRun = false;
                }
                //catch (Exception e)
                {
                //    System.Windows.Forms.DialogResult res = System.Windows.Forms.MessageBox.Show(
                //        e.Message + Environment.NewLine + Environment.NewLine + "Restart the Control Panel?",
                //        "Arcadia Developer Control Panel -- Fatal Error",
                //        MessageBoxButtons.YesNo);

                //    doRun = (res == DialogResult.Yes);
                }
            }
        }

        static void KillAnyProcessThatIsMe()
        {
            Process me = Process.GetCurrentProcess();
            Process found = null;

            foreach (Process clsProcess in Process.GetProcesses())
            {
                if (clsProcess.ProcessName.StartsWith("DeveloperControlPanel") && clsProcess.Id != me.Id)
                {
                    found = clsProcess;
                    break;
                }
            }
            if (found != null)
            {
                found.Kill();
                found.WaitForExit();
            }
        }
    }
}