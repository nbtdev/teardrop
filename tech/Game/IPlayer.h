/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(IPLAYER_INCLUDED)
#define IPLAYER_INCLUDED

namespace Teardrop
{
	class IPlayerControlled;
	class Avatar;

	class IPlayer 
	{
	public:
		virtual ~IPlayer();
		virtual bool destroy() = 0;
		virtual bool update(float deltaT) = 0;
		virtual void setPlayerControlled(IPlayerControlled*) = 0;
		virtual IPlayerControlled* getControlled() = 0;
		virtual unsigned int getPlayerId() const = 0; // netsession-unique player ID
		virtual Avatar* getAvatar() const = 0;
	};
}

#endif // IPLAYER_INCLUDED
