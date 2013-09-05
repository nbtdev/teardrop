/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMMANDTARGET_INCLUDED)
#define COMMANDTARGET_INCLUDED

namespace Teardrop
{
	class Command;

	// abstract interface defining the entry points for command target implementations
	class CommandTarget
	{
	public:
		virtual ~CommandTarget();

		virtual void handle(const Command& command) = 0;
	};
}

#endif // COMMANDTARGET_INCLUDED
