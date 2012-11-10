/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NETWORK_TYPES_INCLUDED)
#define NETWORK_TYPES_INCLUDED

namespace Teardrop
{
	namespace Net
	{
		typedef unsigned long long int uint64_t;

		// data packet sent with disconnected-ping (ID_PONG) responses
		// from a server to an interrogating client
		struct DiscoveryData
		{
			unsigned int mapId;
			unsigned int numPlayers;
			unsigned int bInProgress; // 0 or 1
			char mapName[96];
			char serverName[64];

			DiscoveryData();
		};
	}
}

#endif // NETWORK_TYPES_INCLUDED
