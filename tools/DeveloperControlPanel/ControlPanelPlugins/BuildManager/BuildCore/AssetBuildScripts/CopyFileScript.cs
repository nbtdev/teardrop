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
    public class CopyFileScript : BuildScript
    {
        string m_inputFile = string.Empty;
        BuildManager m_mgr = null;

        string m_error = string.Empty;
        string m_out = string.Empty;

        public CopyFileScript(string inputPath, BuildManager mgr)
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
            status.OutputPath = outputDir + inputLeaf;

            // make sure this directory exists first...
            System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(status.OutputPath));

            // do the copy
            System.IO.File.Copy(m_inputFile, status.OutputPath, true);
            status.Status = JobStatus.State.SUCCEEDED;

            return true;
        }
    }
}
