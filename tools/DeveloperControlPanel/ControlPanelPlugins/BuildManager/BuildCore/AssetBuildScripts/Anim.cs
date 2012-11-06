/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Teardrop
{
    public class Anim : BuildScript
    {
        public static string EXTENSION = "anm";
        public static string TOOL_NAME = "hkxc.exe";

        string m_inputFile = string.Empty;
        BuildManager m_mgr = null;

        string m_error = string.Empty;
        string m_out = string.Empty;

        public Anim(string inputPath, BuildManager mgr)
        {
            m_inputFile = inputPath;
            m_mgr = mgr;
        }

        void StdErrorHandler(Object sender, DataReceivedEventArgs e)
        {
            m_error += e.Data + Environment.NewLine;
        }

        void StdOutHandler(Object sender, DataReceivedEventArgs e)
        {
            m_out += e.Data + Environment.NewLine;
        }

        public override bool build()
        {
            return build(m_outputRootPath);
        }

        /// <summary>
        /// provide an alternate output location for the built asset
        /// </summary>
        /// <param name="outputDir"></param>
        /// <returns></returns>
        public bool build(string outputDir)
        {
            status.Status = JobStatus.State.RUNNING;

            // derive output name from the output root and the file name
            // do this by stripping the input root off of the front of the
            // input filename, and then appending the remainder to the output
            // root path
            string inputLeaf = m_inputFile.Replace(m_inputRootPath, "");

            //m_outputLog.addMessage("Processing " + inputLeaf + "...");

            // replace the extension
            int pos = inputLeaf.LastIndexOf('.');
            if (pos >= 0) // should always be true if we made it this far
            {
                string ext = inputLeaf.Substring(pos + 1);
                inputLeaf = inputLeaf.Replace(ext, EXTENSION);
            }

            status.OutputPath = outputDir + inputLeaf;

            // make sure this directory exists first...
            System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(status.OutputPath));
            status.OutputPath = "\"" + status.OutputPath + "\"";

            // do the build
            Process hkxc = new Process();
            hkxc.StartInfo.FileName = m_toolPath + System.IO.Path.DirectorySeparatorChar + TOOL_NAME;

            // build up an arguments string
            string args = "\"" + m_inputFile + "\"";

            if (m_args.Length == 0)
            {
                args += " " + status.OutputPath;
                args += " -v";
            }
            else
            {
                m_args = m_args.Replace("[outfile]", status.OutputPath);
                args += " " + m_args;
            }

            status.Output += hkxc.StartInfo.FileName + " " + args + Environment.NewLine + Environment.NewLine;

            hkxc.StartInfo.Arguments = args;
            hkxc.StartInfo.CreateNoWindow = true;
            hkxc.StartInfo.RedirectStandardOutput = true;
            hkxc.StartInfo.RedirectStandardError = true;
            hkxc.StartInfo.UseShellExecute = false;

            try
            {
                hkxc.Start();
                string output = hkxc.StandardOutput.ReadToEnd();
                hkxc.WaitForExit();
                string error = hkxc.StandardError.ReadToEnd();
                status.Output += output;
                status.Output += error;

                if (hkxc.ExitCode != 0)
                {
                    status.Status = JobStatus.State.FAILED;
                    hkxc.Close();
                    return false;
                }

                status.Status = JobStatus.State.SUCCEEDED;
                hkxc.Close();
            }
            catch
            {
                status.Status = JobStatus.State.FAILED;
                status.Output += String.Format("Could not start tool '{0}'" + Environment.NewLine, hkxc.StartInfo.FileName);
                return false;
            }

            return true;
        }
    }
}
