/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#include "PingResponse.h"
#include "Stream.h"
#include "Network.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
#include <string.h> // for memset

using namespace Teardrop;
using namespace Net;

//---------------------------------------------------------------------------
PingResponse::PingResponse()
{
	// need to zero out the m_data member, because ping responses from a 
	// server before the server has set up its discovery data, will return
	// a zero-length data block...which will leave this memory uninitialized
	// in the deserialize below...
	memset(&m_data, 0, sizeof(m_data));
}
//---------------------------------------------------------------------------
PingResponse::~PingResponse()
{
}
//---------------------------------------------------------------------------
void PingResponse::deserialize(Net::Stream& bs)
{
	unsigned char id;
	bs.read(id); // ID_PONG

	uint64_t time;
	bs.read(time); // system time from remote response

	// read the offline ping response data, if any
	bs.read((char*)&m_data, sizeof(m_data));

	NetworkSystem* pSys = static_cast<NetworkSystem*>(
		Environment::get().pSystemMgr->getActiveSystem(System::SYSTEM_NETWORK));

	m_time = pSys->getTime() - time;
}
//---------------------------------------------------------------------------
void PingResponse::serialize(Net::Stream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
