/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Teardrop
{
    public class LogWindow : TextBox, LogWriter
    {
        public LogWindow()
        {
        }

        public void WriteLine(string message)
        {
            AppendMessage(message + Environment.NewLine);
        }

        void AppendMessageImpl(string message)
        {
            AppendText(message + Environment.NewLine);
        }

        delegate void AppendMessageDelegate(string message);
        public void AppendMessage(string message)
        {
            if (InvokeRequired)
            {
                object[] args = new object[1];
                args[0] = message;
                Invoke(new AppendMessageDelegate(AppendMessageImpl), args);
            }
            else
            {
                AppendMessageImpl(message);
            }
        }
    }
}
