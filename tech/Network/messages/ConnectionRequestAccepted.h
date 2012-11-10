/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CONNECTION_REQUEST_ACCEPTED_INCLUDED)
#define CONNECTION_REQUEST_ACCEPTED_INCLUDED

#include "Network/Message.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Net
	{
		class ConnectionRequestAccepted
			: public Message
		{
		public:
			static const unsigned int ID = (0x80000000 | 14); // ID_CONNECTION_REQUEST_ACCEPTED -- fix this hack someday

			ConnectionRequestAccepted();
			virtual ~ConnectionRequestAccepted();
			size_t getId() { return ID; }

			//! Message implementation
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // CONNECTION_REQUEST_ACCEPTED_INCLUDED
