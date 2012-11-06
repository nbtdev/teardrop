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
