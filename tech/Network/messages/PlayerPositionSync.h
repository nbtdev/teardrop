/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLAYERPOSITIONSYNC_INCLUDED)
#define PLAYERPOSITIONSYNC_INCLUDED

#include "Network/ProxyMessage.h"
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
			TD_PROXYMESSAGE(PlayerPositionSync, MSGID_PLAYER_POSITION_SYNC);
			Teardrop::Vector4 m_position;
			Teardrop::Quaternion m_orientation;
			float m_twistAngle;

			PlayerPositionSync(const Packet& packet);
			virtual ~PlayerPositionSync();

			TD_DECLARE_ALLOCATOR();

		protected:
			void _deserialize(RakNet::BitStream& bs);
			void _serialize(RakNet::BitStream& bs);
		};
	}
}

#endif // PLAYERPOSITIONSYNC_INCLUDED
