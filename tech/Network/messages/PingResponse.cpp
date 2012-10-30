/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "PingResponse.h"
#include "GetTime.h"

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
PingResponse::PingResponse(const Packet& packet) : Message(packet)
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
void PingResponse::deserialize(RakNet::BitStream& bs)
{
	unsigned char id;
	bs.Read(id); // ID_PONG

	RakNetTime time;
	bs.Read(time); // system time from remote response

	// read the offline ping response data, if any
	bs.Read((char*)&m_data, sizeof(m_data));

	m_time = RakNet::GetTime() - time;
}
//---------------------------------------------------------------------------
void PingResponse::serialize(BitStream& bs)
{
	// nothing to serialize -- this is never sent from inside the app
}
//---------------------------------------------------------------------------
void PingResponse::_deserialize(RakNet::BitStream& bs)
{
}
//---------------------------------------------------------------------------
void PingResponse::_serialize(BitStream& bs)
{
}
