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
using System.Threading;

namespace CoS
{
    enum AssetType
    {
        UNKNOWN,
        COPY,
        MESH,
        ANIM,
        RIG,
        RAGDOLL,
        RIGID_BODY,
        TEXTURE,
        SCENE,
    }

    public class BuildManager
    {
        string m_dataRoot = string.Empty;
        string m_outputRoot = string.Empty;
        string m_toolRoot = string.Empty;

        public BuildManager(string dataRootPath, string outputRootPath, string toolRoot)
        {
            m_dataRoot = dataRootPath;
            m_outputRoot = outputRootPath;
            m_toolRoot = toolRoot;
        }

        public string DataRoot
        {
            get
            {
                return m_dataRoot;
            }
            set
            {
                m_dataRoot = value;
            }
        }

        public string OutputRoot
        {
            get
            {
                return m_outputRoot;
            }
            set
            {
                m_outputRoot = value;
            }
        }

        public string ToolRoot
        {
            get
            {
                return m_toolRoot;
            }
            set
            {
                m_toolRoot = value;
            }
        }

        public bool buildAsset(JobStatus job)
        {
            string argLine = getBuildArgsLine(job.Path);
            AssetType type = AssetType.UNKNOWN;
            string args = string.Empty;

            if (argLine.Length > 0)
            {
                string[] parts = argLine.Split(':');
                char[] delim = { ' ' };
                string[] info = parts[1].Split(delim, 2);
                type = getAssetTypeFromDestExtension(info[0]);
                args = info[1];
            }
            else
            {
                type = getAssetTypeFromSourceExtension(System.IO.Path.GetExtension(job.Path));
            }

            if (type == AssetType.UNKNOWN)
            {
                // We should track and keep a list of all unknown assets, for diagnosing
                // what went wrong in a build.
                return false;
            }

            // otherwise, build it!
            BuildScript script = null;
            switch (type)
            {
                case AssetType.MESH:
                    {
                        script = new Mesh(job.Path, this);
                    }
                    break;

                case AssetType.ANIM:
                    {
                        script = new Anim(job.Path, this);
                    }
                    break;

                case AssetType.RIG:
                    {
                        script = new Rig(job.Path, this);
                    }
                    break;

                case AssetType.RAGDOLL:
                    {
                        script = new Ragdoll(job.Path, this);
                    }
                    break;

                case AssetType.TEXTURE:
                    {
                        script = new Texture(job.Path, this);
                    }
                    break;

                case AssetType.RIGID_BODY:
                    {
                        script = new RigidBody(job.Path, this);
                    }
                    break;

                case AssetType.SCENE:
                    {
                        script = new Scene(job.Path, this);
                    }
                    break;

                case AssetType.COPY:
                    {
                        script = new CopyFileScript(job.Path, this);
                    }
                    break;
            }

            script.Args = args;
            script.Status = job;
            script.OutputRootPath = m_outputRoot;
            script.InputRootPath = m_dataRoot;
            script.ToolPath = m_toolRoot;
            ThreadPool.QueueUserWorkItem(new WaitCallback(buildAsset), script);

            return true;
        }

        /// <summary>
        /// This is a thread proc that is invoked when a thread comes available. The state object 
        /// is a build script, which has BuildScript base class (so that we can call build() on it).
        /// </summary>
        /// <param name="stateInfo">BuildScript state object passed in when the work item was queued</param>
        static void buildAsset(object stateInfo)
        {
            BuildScript script = stateInfo as BuildScript;
            if (script != null)
            {
                // todo: get return information somehow?
                script.build();
            }
        }

        /// <summary>
        /// Get any build tool argument overrides for this file
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        string getBuildArgsLine(string path)
        {
            string rtn = string.Empty;

            // search up the directory tree (to the source data root) for a build.txt file
            string p = System.IO.Path.GetDirectoryName(path);
            string ext = System.IO.Path.GetExtension(path);

            while (p != m_dataRoot)
            {
                string bp = p + System.IO.Path.DirectorySeparatorChar + "build.txt";
                if (System.IO.File.Exists(bp))
                {
                    // find line in the file that corresponds to this file's extension
                    System.IO.FileStream fs = new System.IO.FileStream(bp, System.IO.FileMode.Open);
                    System.IO.TextReader r = new System.IO.StreamReader(fs);

                    string line = r.ReadLine();
                    while (line != null)
                    {
                        string[] parts = line.Split(':');
                        if (parts[0] == ext || parts[0] == ".*")
                        {
                            //string fname = System.IO.Path.GetFileNameWithoutExtension(path);
                            //rtn = parts[1].Replace("[outfile]", m_opts.m_outputPath + System.IO.Path.DirectorySeparatorChar + fname);
                            rtn = line;
                            break;
                        }

                        line = r.ReadLine();
                    }

                    fs.Close();
                }

                if (rtn.Length > 0)
                {
                    break;
                }

                // get the next level up
                p = p.Substring(0, p.LastIndexOf(System.IO.Path.DirectorySeparatorChar));
            }

            return rtn;
        }

        /// <summary>
        /// get the asset type from the discovered destination extension
        /// </summary>
        /// <param name="e"></param>
        /// <returns></returns>
        AssetType getAssetTypeFromDestExtension(string e)
        {
            if (e == ".xml")
            {
                return AssetType.SCENE;
            }

            if (e == ".tex")
            {
                return AssetType.TEXTURE;
            }

            if (e == ".msh")
            {
                return AssetType.MESH;
            }

            if (e == ".rbd")
            {
                return AssetType.RIGID_BODY;
            }

            if (e == ".rig")
            {
                return AssetType.RIG;
            }

            if (e == ".rag")
            {
                return AssetType.RAGDOLL;
            }

            if (e == ".anm")
            {
                return AssetType.ANIM;
            }

            if (e == "copy")
            {
                return AssetType.COPY;
            }

            return AssetType.UNKNOWN;
        }

        /// <summary>
        /// determines asset type by the file's extension
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        AssetType getAssetTypeFromSourceExtension(string e)
        {
            string ext = e.ToLower();

            if (ext == ".hkt")
            {
                // for right now, it's always mesh...we'll need to 
                // check for override directives later
                return AssetType.MESH;
            }

            // only TGA and DDS supported at this time
            if (ext == ".tga" || ext == ".dds")
            {
                // for right now, it's always mesh...we'll need to 
                // check for override directives later
                return AssetType.TEXTURE;
            }

            if (ext == ".xml" || ext == ".swf")
            {
                return AssetType.COPY;
            }

            return AssetType.UNKNOWN;
        }
    }
}
