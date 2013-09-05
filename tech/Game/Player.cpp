/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Player.h"
#include "PlayerInput.h"
#include "Network/Message.h"

using namespace Teardrop;

const float Player::PLAYER_DEFAULT_SYNC_INTERVAL = 0.05f; // 20Hz player update rate

Player::Player(unsigned int id, const String& name)
	: mId(id)
	, mName(name)
	, mAvatar(0)
	, mAvatarClass(0)
	, mVariant(0)
	, mController(0)
	, mTeam(0)
	, mSyncInterval(PLAYER_DEFAULT_SYNC_INTERVAL)
	, mTimeSinceLastSync(0.f)
	, mSyncIntervalElapsed(true)
	, mReady(false)
{
}

Player::~Player()
{
}

unsigned int Player::id() const
{
	return mId;
}

const String& Player::name() const
{
	return mName;
}

Avatar* Player::avatar() const
{
	return mAvatar;
}

const Reflection::ClassDef* Player::avatarClass() const
{
	return mAvatarClass;
}

const String& Player::avatarName() const
{
	return mAvatarName;
}

Variant* Player::variant() const
{
	return mVariant;
}

PlayerInput* Player::playerInput() const
{
	return mController;
}

int Player::team() const
{
	return mTeam;
}

bool Player::syncIntervalElapsed() const
{
	return mSyncIntervalElapsed;
}

float Player::syncInterval() const 
{
	return mSyncInterval;
}

bool Player::ready() const
{
	return mReady;
}

void Player::attach(Avatar* /*avatar*/)
{

}

void Player::detachAvatar()
{

}

void Player::attach(PlayerInput* /*filter*/)
{
}

void Player::detachPlayerController()
{

}

void Player::setPlayerAvatarMetadata(const Reflection::ClassDef* /*pClass*/, const String& /*name*/)
{

}

void Player::setVariant(Variant* /*pVariant*/)
{

}

void Player::setTeam(int /*team*/)
{

}

void Player::setReady(bool /*ready*/)
{

}

void Player::handle(Command& /*cmd*/)
{

}

void Player::post(Net::MessagePtr msg)
{
	mMailslot.push(msg);
}

bool Player::update(float deltaT)
{
	// caller needs to check this after each call to update, because the next time around
	// it gets reset to false
	if (mSyncIntervalElapsed)
		mSyncIntervalElapsed = false;

	mTimeSinceLastSync += deltaT;
	if (mTimeSinceLastSync > mSyncInterval) {
		mSyncIntervalElapsed = true;
	}

	return true;
}
