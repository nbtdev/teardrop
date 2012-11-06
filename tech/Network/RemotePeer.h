/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(REMOTEPEER_INCLUDED)
#define REMOTEPEER_INCLUDED

#include "Network/Peer.h"

namespace Teardrop
{
	namespace Net
	{
		/*
			A sort of convenience class for connections made to
			an endpoint through the Peer::Connect method.
		*/

		class RemotePeer
			: public Peer
		{
			friend class Peer;

		public:
			RemotePeer();
			~RemotePeer();

			void send(Message* pMsg);
			void disconnect();

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // REMOTEPEER_INCLUDED
