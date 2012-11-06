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
    public class Texture : BuildScript
    {
        public static string EXTENSION = "tex";
        public static string TOOL_NAME = "texc.exe";

        string m_inputFile = string.Empty;
        BuildManager m_mgr = null;

        public Texture(string inputPath, BuildManager mgr)
        {
            m_inputFile = inputPath;
            m_mgr = mgr;
        }

        public override bool build()
        {
            return build(m_outputRootPath);
        }

        /// <summary>
        /// Convert, using the nVidia Texture Tools (2) that we have in the 
        /// tool bin dir, anything that is not DDS, to DDS, before it can be used.
        /// Return the caller the new name of the input file, if conversion was needed.
        /// </summary>
        /// <param name="inputFile"></param>
        /// <returns></returns>
        string convertToDds(string inputFile)
        {
            string ext = System.IO.Path.GetExtension(inputFile);
            if (ext.ToLower() == ".dds")
            {
                return inputFile;
            }

            status.Output += String.Format("Preprocessing {0} into BC2/DXT3 format" + Environment.NewLine, System.IO.Path.GetFileName(inputFile));

            // otherwise, we need to convert it first
            string newFile = System.IO.Path.GetTempFileName();
            newFile = System.IO.Path.ChangeExtension(newFile, ".dds");

            // for now, just make everything BC2 (DXT3) -- todo: read this
            // from a build.txt file for overrides
            string args = "-bc2 -nocuda \"" + inputFile + "\" \"" + newFile + "\""; 

            Process nvtt = new Process();
            nvtt.StartInfo.FileName = m_toolPath + System.IO.Path.DirectorySeparatorChar + "nvcompress.exe";
            nvtt.StartInfo.Arguments = args;
            nvtt.StartInfo.CreateNoWindow = true;
            nvtt.StartInfo.RedirectStandardOutput = true;
            nvtt.StartInfo.UseShellExecute = false;

            status.Output += nvtt.StartInfo.FileName + " " + nvtt.StartInfo.Arguments + Environment.NewLine;

            if (nvtt.Start())
            {
                string output = nvtt.StandardOutput.ReadToEnd();
                nvtt.WaitForExit();
                status.Output += output + Environment.NewLine + Environment.NewLine;

                if (nvtt.ExitCode != 0)
                {
                    status.Status = JobStatus.State.FAILED;
                }

                nvtt.Close();
            }

            if (status.Status == JobStatus.State.FAILED)
                return string.Empty;
            else
                return newFile;
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

            // since textures have a prebuild step we need to set this to something meaningful during that step (in case of failure)
            status.OutputPath = outputDir + inputLeaf;

            // convert anything that is not a DDS, to a DDS, on the fly;
            // this just returns the original filename if it's already dds
            m_inputFile = convertToDds(m_inputFile);
            if (m_inputFile.Length == 0)
            {
                return false;
            }

            // replace the extension
            int pos = inputLeaf.LastIndexOf('.');
            if (pos >= 0) // should always be true if we made it this far
            {
                string ext = inputLeaf.Substring(pos + 1);
                inputLeaf = inputLeaf.Replace(ext, EXTENSION);
            }

            status.OutputPath = outputDir + inputLeaf;

            // make sure output directory exists
            System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(status.OutputPath));

            // do the build
            Process texc = new Process();
            texc.StartInfo.FileName = m_toolPath + System.IO.Path.DirectorySeparatorChar + TOOL_NAME;

            // build up an arguments string
            string args = "\"" + m_inputFile + "\"";
            args += " \"" + status.OutputPath + "\"";

            if (m_args.Length == 0)
            {
                args += " -v";
            }
            else
            {
                m_args = m_args.Replace("[outfile]", status.OutputPath);
                args += " " + m_args;
            }

            status.Output += texc.StartInfo.FileName + " " + args + Environment.NewLine + Environment.NewLine;

            texc.StartInfo.Arguments = args;
            texc.StartInfo.CreateNoWindow = true;
            texc.StartInfo.RedirectStandardOutput = true;
            texc.StartInfo.UseShellExecute = false;

            try
            {
                texc.Start();
                string output = texc.StandardOutput.ReadToEnd();
                texc.WaitForExit();
                status.Output += output;

                if (texc.ExitCode != 0)
                {
                    status.Status = JobStatus.State.FAILED;
                    texc.Close();
                    return false;
                }

                status.Status = JobStatus.State.SUCCEEDED;
                texc.Close();
            }
            catch
            {
                status.Status = JobStatus.State.FAILED;
                status.Output += String.Format("Could not start tool '{0}'" + Environment.NewLine, texc.StartInfo.FileName);
                return false;
            }

            return true;
        }
    }
}
