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

namespace CoS
{
    public class Options
    {
        public string m_dataPath = string.Empty;
        public string m_outputPath = string.Empty;
        public string m_toolPath = string.Empty;

        public Options()
        {
        }

        public bool load()
        {
            RegistryKey HKLM = Registry.LocalMachine;
            RegistryKey a = HKLM.OpenSubKey("Software\\Arcadia (Internal)\\Build");

            if (a == null)
            {
                // then create it
                HKLM.CreateSubKey("Software\\Arcadia (Internal)\\Build");
            }

            m_dataPath = a.GetValue("BuildInputRoot").ToString();
            m_outputPath = a.GetValue("BuildOutputRoot").ToString();
            m_toolPath = a.GetValue("ToolBinRoot").ToString();
            
            a.Close();

            return true;
        }

        public bool save()
        {
            RegistryKey HKLM = Registry.LocalMachine;
            RegistryKey a = HKLM.OpenSubKey("Software\\Arcadia (Internal)\\Build", true);

            a.SetValue("BuildInputRoot", m_dataPath);
            a.SetValue("BuildOutputRoot", m_outputPath);
            a.SetValue("ToolBinRoot", m_toolPath);

            a.Close();

            return true;
        }
    }
}
