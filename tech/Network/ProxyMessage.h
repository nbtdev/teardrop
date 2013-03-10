/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROXYMESSAGE_INCLUDED)
#define PROXYMESSAGE_INCLUDED

#include "Network/Message.h"

namespace Teardrop
{
	namespace Net
	{
		class ProxyMessage
			: public Message
		{
		public:
			unsigned int mPlayerId;

			ProxyMessage();
			virtual ~ProxyMessage();

			bool isProxyMessage();

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // PROXYMESSAGE_INCLUDED
