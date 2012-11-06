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
    public interface LogWriter
    {
        void WriteLine(string line);
    }

    public class Log
    {
        LogWriter m_logWriter;

        public Log(LogWriter logWriter)
        {
            m_logWriter = logWriter;
        }

        public void addMessage(string message)
        {
            m_logWriter.WriteLine(message);
        }
    }
}
