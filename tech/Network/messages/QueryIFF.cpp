/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "QueryIFF.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(QueryIFF);
//---------------------------------------------------------------------------
QueryIFF::QueryIFF()
{
}
//---------------------------------------------------------------------------
QueryIFF::~QueryIFF()
{
}
//---------------------------------------------------------------------------
void QueryIFF::deserialize(Net::Stream& bs)
{
	bs.read(m_selfId);
	unsigned int sz;
	bs.read(sz);
	m_targetIds.resize(sz);
	for (size_t i=0; i<sz; ++i)
		bs.read(m_targetIds[i]);
}
//---------------------------------------------------------------------------
void QueryIFF::serialize(Net::Stream& bs)
{
	bs.write(m_selfId);
	unsigned int sz = m_targetIds.size();
	bs.write(sz);
	for (size_t i=0; i<sz; ++i)
		bs.write(m_targetIds[i]);
}
