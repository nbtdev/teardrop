/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "stdafx.h"
#include "QueryIFF.h"

using namespace Teardrop;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(QueryIFF);
//---------------------------------------------------------------------------
QueryIFF::QueryIFF(const Packet& packet) : Message(packet)
{
}
//---------------------------------------------------------------------------
QueryIFF::~QueryIFF()
{
}
//---------------------------------------------------------------------------
void QueryIFF::_deserialize(RakNet::BitStream& bs)
{
	bs.Read(m_selfId);
	size_t sz;
	bs.Read(sz);
	m_targetIds.resize(sz);
	for (size_t i=0; i<sz; ++i)
		bs.Read(m_targetIds[i]);
}
//---------------------------------------------------------------------------
void QueryIFF::_serialize(BitStream& bs)
{
	bs.Write(m_selfId);
	size_t sz = m_targetIds.size();
	bs.Write(sz);
	for (size_t i=0; i<sz; ++i)
		bs.Write(m_targetIds[i]);
}
