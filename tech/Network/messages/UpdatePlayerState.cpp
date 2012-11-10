/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "UpdatePlayerState.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(UpdatePlayerState);
//---------------------------------------------------------------------------
UpdatePlayerState::UpdatePlayerState()
{
}
//---------------------------------------------------------------------------
UpdatePlayerState::~UpdatePlayerState()
{
}
//---------------------------------------------------------------------------
void UpdatePlayerState::deserialize(Net::Stream& bs)
{
	bs.read(m_id);
	bs.read(m_playerTeam);
	bs.read(m_playerName);
	bs.read(m_playerCharClass);
	bs.read(m_playerEntityVariant);
	bs.read(m_playerChar);
	bs.read(m_bReady);
}
//---------------------------------------------------------------------------
void UpdatePlayerState::serialize(Net::Stream& bs)
{
	bs.write(m_id);
	bs.write(m_playerTeam);
	bs.write(m_playerName);
	bs.write(m_playerCharClass);
	bs.write(m_playerEntityVariant);
	bs.write(m_playerChar);
	bs.write(m_bReady);
}
