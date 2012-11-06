/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;

namespace Teardrop
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
