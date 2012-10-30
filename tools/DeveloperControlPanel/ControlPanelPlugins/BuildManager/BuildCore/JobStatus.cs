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

namespace CoS
{
    public delegate void JobStateChangedHandler(JobStatus job);
    public class JobStatus
    {
        public event JobStateChangedHandler JobStateChanged;

        public enum State
        {
            UNKNOWN_TYPE,
            UNKNOWN_PATH,
            PENDING,
            RUNNING,
            FAILED,
            SUCCEEDED
        }

        string path = string.Empty;
        string outPath = string.Empty;
        string output = string.Empty;
        string error = string.Empty;
        State status = State.PENDING;
        object tag = null;

        public object Tag
        {
            get
            {
                return tag;
            }
            set
            {
                tag = value;
            }
        }

        public string Path
        {
            get
            {
                return path;
            }
            set
            {
                path = value;
            }
        }

        public string OutputPath
        {
            get
            {
                return outPath;
            }
            set
            {
                outPath = value;
            }
        }

        public string Output
        {
            get
            {
                return output;
            }
            set
            {
                output = value;
            }
        }

        public string Error
        {
            get
            {
                return error;
            }
            set
            {
                error = value;
            }
        }

        public State Status
        {
            get
            {
                return status;
            }
            set
            {
                status = value;
                updateNodeStatus();
            }
        }

        void updateNodeStatus()
        {
            if (JobStateChanged != null)
            {
                JobStateChanged(this);
            }
        }
    }
}
