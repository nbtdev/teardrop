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
using Microsoft.Win32;

namespace ConfigurationManager
{

    public class OptionData
    {
        string m_outputDir;
        string m_dataDir;
        string m_toolDir;
        const string OPTIONS_FILE = "options.ini";

        public OptionData()
        {
            // put in some default values based on current directory
            string curDir = Environment.CurrentDirectory;
            m_toolDir = curDir;
            m_dataDir = System.IO.Path.GetFullPath("..\\..\\data");
            m_outputDir = System.IO.Path.GetFullPath("..\\..\\bin\\data");
        }

        public string OutputDir
        {
            get
            {
                return m_outputDir;
            }

            set
            {
                m_outputDir = value;
            }
        }

        public string DataDir
        {
            get
            {
                return m_dataDir;
            }

            set
            {
                m_dataDir = value;
            }
        }

        public string ToolDir
        {
            get
            {
                return m_toolDir;
            }

            set
            {
                m_toolDir = value;
            }
        }

        public bool Load()
        {
            // deserialize from XML
            return true;
        }

        public bool Save()
        {
            return true;
        }
    }

    public class OptionsApplyArgs : EventArgs
    {
        public OptionData m_optionData;

        public OptionsApplyArgs(OptionData optionData)
        {
            m_optionData = optionData;
        }
    }
}
