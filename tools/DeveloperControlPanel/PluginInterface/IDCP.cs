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

namespace DeveloperControlPanel.Plugin
{
    /// <summary>
    /// Event args class used by the ConfigDataChanged event
    /// </summary>
    public class ConfigDataChangedArgs : EventArgs
    {
        System.Collections.Hashtable m_data;

        public ConfigDataChangedArgs(System.Collections.Hashtable dict)
        {
            m_data = dict;
        }

        public System.Collections.Hashtable Data
        {
            get
            {
                return m_data;
            }
        }
    }

    /// <summary>
    /// Event args class used by the FilesChanged event
    /// </summary>
    public class FilesChangedArgs : EventArgs
    {
        string[] m_pathnames = null;

        public FilesChangedArgs(string[] pathnames)
        {
            m_pathnames = pathnames;
        }

        /// <summary>
        /// Array of string values containing the full paths to the changed files
        /// </summary>
        public string[] Pathnames
        {
            get
            {
                return m_pathnames;
            }
        }
    }

    /// <summary>
    /// Event args class used by the BuildCompleted event
    /// </summary>
    public class BuildCompletedArgs : EventArgs
    {
        bool m_success = false;
        string[] m_files = null;

        public BuildCompletedArgs(bool status, string [] files)
        {
            m_success = status;
            m_files = files;
        }

        /// <summary>
        /// Boolean indicator of build success (in total) 
        /// </summary>
        public bool Status
        {
            get
            {
                return m_success;
            }
        }

        /// <summary>
        /// List of full output pathnames that were built
        /// </summary>
        public string[]  Files
        {
            get
            {
                return m_files;
            }
        }
    }

    public enum IconStatus
    {
        NewlyLoaded,
        FileChangeDetected,
        Building,
        BuildSuccess,
        BuildFailure
    };

    public interface IADCP
    {
        /// <summary>
        /// (Asynchronous) Method for plugins to call to request 
        /// latest configuration data. You will need to handle the 
        /// ConfigDataChanged event to receive the requested data.
        /// </summary>
        void RequestConfigData();

        /// <summary>
        /// Called by configuration data managers to notify all plugins of 
        /// changed configuration data
        /// </summary>
        /// <param name="dict">Name-value config data pairs</param>
        void RaiseConfigDataChanged(System.Collections.Hashtable dict);

        /// <summary>
        /// Called by build management plugins to notify all other plugins that 
        /// a build has started.
        /// </summary>
        void RaiseBuildStarted();

        /// <summary>
        /// Called by build management plugins to indicate the end of a build.
        /// </summary>
        /// <param name="status"><b>true</b> if the build succeeded in total, <b>false</b> otherwise</param>
        void RaiseBuildCompleted(bool status, string [] files);

        /// <summary>
        /// Called by a data publisher to indicate completion of data publication to a data server
        /// </summary>
        void RaiseDataPublished();

        /// <summary>
        /// Called by a file system watcher when there are files ready to build
        /// </summary>
        void RaiseWatchedFilesChanged(string [] files);

        /// <summary>
        /// Method for plugins to call to change the tool tray icon.
        /// </summary>
        void SetIconStatus( IconStatus iconStatus );

        #region Shell Events
        event EventHandler<EventArgs> ConfigDataRequested;
        event EventHandler<ConfigDataChangedArgs> ConfigDataChanged;
        event EventHandler<FilesChangedArgs> WatchedFilesChanged;
        event EventHandler<EventArgs> BuildStarted;
        event EventHandler<BuildCompletedArgs> BuildCompleted;
        event EventHandler<EventArgs> DataPublished;
        event EventHandler<EventArgs> BuildNotifyBalloonClicked;
        #endregion

    }
}
