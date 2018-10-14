/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(PLAYER_INCLUDED)
#define PLAYER_INCLUDED

#include "Network/MessageFwd.h"
#include "Util/_String.h"
#include <queue>

typedef std::queue<Teardrop::Net::MessagePtr> Mailslot;

namespace Teardrop
{
	namespace Reflection 
	{
		class ClassDef;
	}

	class Avatar;
	class PlayerInput;
	class Command;
	class Variant;

	class Player 
	{
	public:
		static const float PLAYER_DEFAULT_SYNC_INTERVAL;

		Player(unsigned int id, const String& name);
		~Player();
		
		unsigned int id() const; // netsession-unique player ID
		const String& name() const; // player name
		Avatar* avatar() const; // will be null until player is in game
		const Reflection::ClassDef* avatarClass() const; // currently selected avatar class (MACRO, etc)
		const String& avatarName() const; // specific member of avatar class (chassis name/designation, etc)
		Variant* variant() const; // currently selected variant (or null if none)
		PlayerInput* playerInput() const;
		int team() const; // player team (NO_TEAM if none)
		bool syncIntervalElapsed() const;
		float syncInterval() const;
		bool ready() const;

		void attach(Avatar* avatar);
		void detachAvatar();
		void attach(PlayerInput* controller);
		void detachPlayerController();
		void setPlayerAvatarMetadata(const Reflection::ClassDef* pClass, const String& name);
		void setVariant(Variant* pVariant);
		void setTeam(int team);
		void setSyncInterval(float interval);
		void setReady(bool ready);

		void handle(Command& cmd);
		void post(Net::MessagePtr msg);
		bool update(float deltaT);

	protected:
		unsigned int mId;
		String mName;
		Avatar* mAvatar;
		const Reflection::ClassDef* mAvatarClass;
		String mAvatarName;
		Variant* mVariant;
		PlayerInput* mController;
		int mTeam;
		float mSyncInterval;
		float mTimeSinceLastSync;
		bool mSyncIntervalElapsed;
		bool mReady;
		Mailslot mMailslot;
	};
}

#endif // PLAYER_INCLUDED
