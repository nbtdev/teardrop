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
