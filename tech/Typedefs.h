/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TYPEDEFS_INCLUDED)
#define TYPEDEFS_INCLUDED

#include <queue>
#include "Network/Message.h"
#include "Game/Command.h"

typedef std::queue<Teardrop::Net::MessagePtr> Mailslot;

typedef std::queue<Teardrop::Command> CommandQueue;


#endif // TYPEDEFS_INCLUDED
