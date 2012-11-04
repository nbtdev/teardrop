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
#include "ChatMessage.h"

using namespace CoS;
using namespace Net;
using namespace RakNet;
//---------------------------------------------------------------------------
COS_NETMESSAGE_IMPL(ChatMessage);
//---------------------------------------------------------------------------
ChatMessage::ChatMessage(const Packet& packet) : Message(packet)
{
	m_team = 0; // default to "broadcast"
}
//---------------------------------------------------------------------------
ChatMessage::~ChatMessage()
{
}
//---------------------------------------------------------------------------
void ChatMessage::_deserialize(RakNet::BitStream& bs)
{
	RakString msg;
	bs.Read(msg);
	m_message = msg;
	bs.Read(m_team);
	bs.Read(m_senderId);
	bs.Read(m_source);
}
//---------------------------------------------------------------------------
void ChatMessage::_serialize(BitStream& bs)
{
	bs.Write(RakString(m_message));
	bs.Write(m_team);
	bs.Write(m_senderId);
	bs.Write(m_source);
}