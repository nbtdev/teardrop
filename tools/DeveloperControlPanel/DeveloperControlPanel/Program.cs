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