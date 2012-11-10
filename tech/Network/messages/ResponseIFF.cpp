/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "ResponseIFF.h"
#include "Stream.h"

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
TD_NETMESSAGE_IMPL(ResponseIFF);
//---------------------------------------------------------------------------
ResponseIFF::ResponseIFF()
{
}
//---------------------------------------------------------------------------
ResponseIFF::~ResponseIFF()
{
}
//---------------------------------------------------------------------------
void ResponseIFF::deserialize(Net::Stream& bs)
{
	bs.read(m_selfId);
	unsigned int sz;
	bs.read(sz);
	m_iffInfo.resize(sz);
	for (size_t i=0; i<sz; ++i)
	{
		bs.read(m_iffInfo[i].m_targetId);
		bs.read(m_iffInfo[i].m_iff);
	}
}
//---------------------------------------------------------------------------
void ResponseIFF::serialize(Net::Stream& bs)
{
	bs.write(m_selfId);
	unsigned int sz = m_iffInfo.size();
	bs.write(sz);
	for (size_t i=0; i<sz; ++i)
	{
		bs.write(m_iffInfo[i].m_targetId);
		bs.write(m_iffInfo[i].m_iff);
	}
}
