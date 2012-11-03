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

#if !defined(PROXYMESSAGE_INCLUDED)
#define PROXYMESSAGE_INCLUDED

#include "Network/Message.h"

namespace CoS
{
	namespace Net
	{
		class ProxyMessage
			: public Message
		{
		public:
			unsigned int m_playerId;
			const static unsigned int MASK = 0x40000000;

			ProxyMessage(const Packet& packet);
			virtual ~ProxyMessage();

			// these are virtual for overriding by Raknet message classes
			void deserialize(RakNet::BitStream& bs);
			void serialize(RakNet::BitStream& bs);

			COS_DECLARE_ALLOCATOR();

		protected:
			ProxyMessage();
			virtual void _deserialize(RakNet::BitStream& bs) = 0;
			virtual void _serialize(RakNet::BitStream& bs) = 0;
		};
	}
}

#endif // PROXYMESSAGE_INCLUDED
