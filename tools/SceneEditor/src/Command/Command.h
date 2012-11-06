/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EDITOR_COMMAND_INCLUDED)
#define EDITOR_COMMAND_INCLUDED

// base class for all editor commands

namespace Editor
{
	struct Command
	{
		virtual void execute() = 0;
		virtual void undo() = 0;
	};
}

#endif // EDITOR_COMMAND_INCLUDED
