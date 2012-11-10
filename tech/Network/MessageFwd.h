/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(NET_MESSAGEFWD_INCLUDED)
#define NET_MESSAGEFWD_INCLUDED

#include "Util/SharedPointer.h"

/* This file is needed to break a circular dependency between Network.h and Message.h,
   introduced by the use of MessagePtr in the NetworkSystem class (part of major SDK
   refactoring work). It's hacky, but effective and simple.
*/

namespace Teardrop
{
	namespace Net
	{
		class Message;
		typedef SharedPointer<Message> MessagePtr;
	}
}

#endif // NET_MESSAGEFWD_INCLUDED
