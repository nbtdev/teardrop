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

#if !defined(CONNECTIONLOST_INCLUDED)
#define CONNECTIONLOST_INCLUDED

#include "Network/Message.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Net
	{
		class ConnectionLost
			: public Message
		{
		public: 
			static const unsigned int ID = (0x80000000 | 20); // ID_CONNECTION_LOST -- fix this hack someday
			unsigned int mPlayerId;

			ConnectionLost();
			virtual ~ConnectionLost();
			size_t getId() { return ID; }

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // CONNECTIONLOST_INCLUDED
