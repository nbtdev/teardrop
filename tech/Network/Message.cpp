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
#include "Message.h"
#include "Network.h"
#include "Stream.h"
#include "Peer.h"

//#if defined(_DEBUG)
#include "Util/Environment.h"
#include "Util/Logger.h"
//#endif

using namespace Teardrop;
using namespace Net;
//---------------------------------------------------------------------------
Message::Message()
{
	m_priority = PRIORITY_MEDIUM;
	m_reliability = RELIABILITY_RELIABLE;
	m_class = MESSAGE_USER;
	m_channel = 0;
	m_pPeer = 0;
}
//---------------------------------------------------------------------------
Message::~Message()
{
	delete m_pPeer;
}

bool Message::isProxyMessage()
{
	return false;
}
