/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(IPLAYERCONTROLLED_INCLUDED)
#define IPLAYERCONTROLLED_INCLUDED

namespace Teardrop
{
	/*
		IPlayerControlled

		Protocol class for interfacing between Player and objects that
		the player can control. Control typically involves issuing commands
		to a controlled object.
	*/

	class Command;

	class IPlayerControlled
	{
	public:
		virtual void issueCommand(Command& command) = 0;
	};
}

#endif // IPLAYERCONTROLLED_INCLUDED
