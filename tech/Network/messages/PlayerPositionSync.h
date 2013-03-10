/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERPOSITIONSYNC_INCLUDED)
#define PLAYERPOSITIONSYNC_INCLUDED

#include "Network/ProxyMessage.h"
#include "Network/Network.h"
#include "Network/Protocol.h"
#include "Memory/Memory.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace Teardrop
{
	namespace Net
	{
		class PlayerPositionSync
			: public ProxyMessage
		{
		public:
			TD_NETMESSAGE(PlayerPositionSync, MSGID_PLAYER_POSITION_SYNC);
			Teardrop::Vector4 m_position;
			Teardrop::Quaternion m_orientation;
			float m_twistAngle;

			PlayerPositionSync();
			virtual ~PlayerPositionSync();

			TD_DECLARE_ALLOCATOR();

		protected:
			void deserialize(Net::Stream& bs);
			void serialize(Net::Stream& bs);
		};
	}
}

#endif // PLAYERPOSITIONSYNC_INCLUDED
