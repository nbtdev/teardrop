/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "GameEnded.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(GameEnded);
//---------------------------------------------------------------------------
GameEnded::GameEnded()
{
	m_channel = 2;
}
//---------------------------------------------------------------------------
GameEnded::~GameEnded()
{
}
//---------------------------------------------------------------------------
void GameEnded::deserialize(Net::Stream& bs)
{
}
//---------------------------------------------------------------------------
void GameEnded::serialize(Net::Stream& bs)
{
}
