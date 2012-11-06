/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DELETEOBJECT_COMMAND_INCLUDED)
#define DELETEOBJECT_COMMAND_INCLUDED

#include "Command/Command.h"
#include "Memory/include/Memory.h"

// Remove previously-placed object from a level

namespace CoS
{
	class ZoneObject;
	class Zone;
}

namespace Editor 
{
	class PlaceObjectCommand;

	class DeleteObjectCommand : public Command
	{
		// command-specific data
		CoS::ZoneObject* m_pZoneObject;
		CoS::Zone* m_pZone;

		// the opposite of a delete-object is a place-object...
		PlaceObjectCommand* m_pPlaceCommand;

	public:
		DeleteObjectCommand(CoS::ZoneObject* pZoneObject, CoS::Zone* pZone);
		~DeleteObjectCommand();

		// Command implementation
		void execute();
		void undo();

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // DELETEOBJECT_COMMAND_INCLUDED
