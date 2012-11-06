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
using DeveloperControlPanel.Plugin;
using System.Net;
using System.Net.Sockets;

namespace Teardrop
{
    public class CompanionPlugin : Plugin
    {
        Socket m_socket = null;
        byte[] m_sendBuf = null;
        byte[] m_recvBuf = null;
        const int BUFSIZE = 8 * 1024;
        AsyncCallback m_readCB = new AsyncCallback(readCallback);
        CompanionUI m_ui = null;
        bool m_isConnected = false;
        System.Text.Encoding m_enc = new System.Text.ASCIIEncoding();

        #region Plugin Implementation
        OptionsControl m_optionsControl = null;
        PluginContext m_context = null;
        string m_name = "Companion";
        string m_displayName = "Companion";

        public override bool HasUI
        {
            get
            {
                return true;
            }
        }

        public override string Name
        {
            get
            {
                return m_name;
            }
        }

        public override string DisplayName
        {
            get
            {
                return m_displayName;
            }
        }

        public override PluginContext Context
        {
            set
            {
                m_context = value;
            }
            get
            {
                return m_context;
            }
        }

        public override OptionsControl OptionsContent
        {
            get
            {
                return m_optionsControl;
            }
        }

        public override void MenuAdjust(ref ToolStripMenuItem toolStripMenuItem, ref ContextMenuStrip contextMenuStrip)
        {
            ToolStripMenuItem menuItem = new ToolStripMenuItem();
            menuItem.Name = Name;
            menuItem.Text = DisplayName;
            menuItem.Tag = this;
            menuItem.Click += new EventHandler(OnShowTool);
            toolStripMenuItem.DropDown.Items.Insert(0,menuItem);

            ToolStripMenuItem itemCopy = new ToolStripMenuItem();
            itemCopy.Name = menuItem.Name;
            itemCopy.Text = menuItem.Text;
            itemCopy.Tag = menuItem.Tag;
            itemCopy.Click += new EventHandler(OnShowTool);
            contextMenuStrip.Items.Insert(0, itemCopy);
        }

        public override bool Startup()
        {
            m_ui = new CompanionUI();
            m_ui.Hide();
            m_ui.Reconnect += new EventHandler<EventArgs>(OnReconnect);
            
            // connect to running game, if there is one
            m_isConnected = initNetwork(true);

            m_context.m_bDirtyMenu = true;

            return true;
        }

        void OnReconnect(object sender, EventArgs e)
        {
            initNetwork(false);
        }

        public override void Shutdown()
        {
            if (m_socket != null)
            {
                m_socket.Disconnect(false);
                m_socket.Close();
            }
        }

        public override void OnShowTool(object sender, EventArgs e)
        {
            m_ui.Show();
        }

        #endregion


        static void readCallback(IAsyncResult ar)
        {
            CompanionPlugin pThis = (CompanionPlugin)ar.AsyncState;
            try
            {
                int bytes = pThis.m_socket.EndReceive(ar);
                pThis.processData(false);
                pThis.m_socket.BeginReceive(pThis.m_recvBuf, 0, pThis.m_recvBuf.Length, 0, pThis.m_readCB, pThis);
            }
            catch
            {
                pThis.processData(true);
            }
        }

        void consumeMemoryStats(byte[] buf, ref int offset)
        {
            // memory stats come in as 32 bytes of allocator string name, and then 
            // 4 bytes of current usage, 4 bytes of high-water mark, 4 bytes of 
            // hard limit and 4 of soft limit (budget). 
            // The list of allocators terminates with 16 null bytes.

            string name = string.Empty;
            uint current = 0;
            uint highwater = 0;
            uint hardLimit = 0;
            uint softLimit = 0;

            while (buf[offset] != 0)
            {
                name = m_enc.GetString(buf, offset, 32);
                offset += 32;
                current = BitConverter.ToUInt32(buf, offset);
                offset += sizeof(uint);
                highwater = BitConverter.ToUInt32(buf, offset);
                offset += sizeof(uint);
                hardLimit = BitConverter.ToUInt32(buf, offset);
                offset += sizeof(uint);
                softLimit = BitConverter.ToUInt32(buf, offset);
                offset += sizeof(uint);

                // post to the UI
                m_ui.setAllocatorStats(name, current, highwater, hardLimit, softLimit);
            }
        }

        private void processData(bool aborted)
        {
            if (aborted)
            {
                m_socket.Close();
                m_isConnected = false;
                return;
            }

            int bytes;
            m_ui.beginUpdate();
            while ((bytes = m_socket.Receive(m_recvBuf)) > 0)
            {
                // marshal up the data -- should be two floats and three UInt32s
                float startTime = BitConverter.ToSingle(m_recvBuf, 0);
                float endTime = BitConverter.ToSingle(m_recvBuf, 4);
                UInt32 numTris = BitConverter.ToUInt32(m_recvBuf, 8);
                UInt32 numBatches = BitConverter.ToUInt32(m_recvBuf, 12);
                UInt32 numVerts = BitConverter.ToUInt32(m_recvBuf, 16);

                // and following that, memory statistics
                int offset = 20;
                consumeMemoryStats(m_recvBuf, ref offset);

                // put it in the display
                try
                {
                    float time = endTime - startTime;
                    float frameTime = 0;
                    if (System.Math.Abs(time) > 0.000005f)
                    {
                        frameTime = 1.0f / time;
                    }

                    m_ui.FPS = String.Format("#####", time);
                }
                catch (Exception)
                {
                    m_ui.FPS = "0";
                }

                m_ui.Tris = numTris.ToString();
                m_ui.Verts = numVerts.ToString();
                m_ui.Batches = numBatches.ToString();
            }

            m_ui.endUpdate();
        }

        private bool initNetwork(bool initial)
        {
            if (m_isConnected)
            {
                return true;
            }

            IPHostEntry entry = Dns.GetHostEntry("127.0.0.1");

            foreach (IPAddress addr in entry.AddressList)
            {
                IPEndPoint ipe = new IPEndPoint(addr, 27015);
                m_socket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                try
                {
                    m_socket.Connect(ipe);
                }
                catch (Exception ex)
                {
                    if (!initial)
                    {
                        m_ui.Alert(ex.Message);
                    }
                }

                if (m_socket.Connected)
                {
                    m_socket.ReceiveBufferSize = BUFSIZE;
                    m_socket.SendBufferSize = BUFSIZE;
                    m_sendBuf = new byte[BUFSIZE];
                    m_recvBuf = new byte[BUFSIZE];

                    m_socket.BeginReceive(m_recvBuf, 0, m_recvBuf.Length, 0, m_readCB, this);
                    return true;
                }
            }

            if (!initial)
            {
                m_ui.Alert("Could not connect to a running game instance");
            }
            return false;
        }
    }
}
