/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#if !defined(UPDATESERVERSTATE_INCLUDED)
#define UPDATESERVERSTATE_INCLUDED

#include "Network/Message.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Net
	{
		class UpdateServerState
			: public Message
		{
		public:
			TD_NETMESSAGE(UpdateServerState, MSGID_UPDATE_SERVER_INFO);
			unsigned int m_ip;
			int m_maxNumTeams;
			String m_serverName;
			String m_mapName;
			int m_mapId;
			size_t m_numPlayers;
			size_t m_ping;
			unsigned char m_updateOrRemove; // 0 for update, 1 for remove
			bool m_bInProgress;

			UpdateServerState();
			virtual ~UpdateServerState();

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // UPDATESERVERSTATE_INCLUDED
