/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "ResponseIFF.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(ResponseIFF);
//---------------------------------------------------------------------------
ResponseIFF::ResponseIFF(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
ResponseIFF::~ResponseIFF()
{
}
//---------------------------------------------------------------------------
void ResponseIFF::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_selfId);
	size_t sz;
	bs.Read(sz);
	m_iffInfo.resize(sz);
	for (size_t i=0; i<sz; ++i)
	{
		bs.Read(m_iffInfo[i].m_targetId);
		bs.Read(m_iffInfo[i].m_iff);
	}
}
//---------------------------------------------------------------------------
void ResponseIFF::_serialize(BitStream& bs)
{
	bs.Write(m_selfId);
	size_t sz = m_iffInfo.size();
	bs.Write(sz);
	for (size_t i=0; i<sz; ++i)
	{
		bs.Write(m_iffInfo[i].m_targetId);
		bs.Write(m_iffInfo[i].m_iff);
	}
}
