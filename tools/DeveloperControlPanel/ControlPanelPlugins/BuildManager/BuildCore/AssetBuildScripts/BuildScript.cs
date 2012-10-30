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
    public class BuildScript
    {
        public virtual bool build()
        {
            throw new Exception("Method Not Implemented!");
        }

        protected string m_args = string.Empty;
        public string Args
        {
            get
            {
                return m_args;
            }
            set
            {
                m_args = value;
            }
        }

        protected string m_inputRootPath = string.Empty;
        public string InputRootPath
        {
            get
            {
                return m_inputRootPath;
            }
            set
            {
                m_inputRootPath = value;
            }
        }

        protected string m_outputRootPath = string.Empty;
        public string OutputRootPath
        {
            get
            {
                return m_outputRootPath;
            }
            set
            {
                m_outputRootPath = value;
            }
        }

        protected string m_toolPath = string.Empty;
        public string ToolPath
        {
            get
            {
                return m_toolPath;
            }
            set
            {
                m_toolPath = value;
            }
        }
        
        protected JobStatus status = null;
        public JobStatus Status
        {
            get
            {
                return status;
            }
            set
            {
                status = value;
            }
        }
    }
}
