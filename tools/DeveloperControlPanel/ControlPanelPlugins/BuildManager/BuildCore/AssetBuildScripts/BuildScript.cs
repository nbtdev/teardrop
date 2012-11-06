/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace Teardrop
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
