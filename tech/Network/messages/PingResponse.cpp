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
