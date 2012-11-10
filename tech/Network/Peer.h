/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NETPEER_INCLUDED)
#define NETPEER_INCLUDED

namespace Teardrop
{
	namespace Net
	{
		class Peer
		{
		public:
			virtual ~Peer();

			virtual unsigned int getPlayerId() = 0;
		};
	}
}

#endif // NETPEER_INCLUDED
