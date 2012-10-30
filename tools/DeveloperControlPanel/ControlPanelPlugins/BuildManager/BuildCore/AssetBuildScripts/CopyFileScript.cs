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
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace CoS
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
