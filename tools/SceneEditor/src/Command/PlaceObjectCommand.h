/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLACEOBJECT_COMMAND_INCLUDED)
#define PLACEOBJECT_COMMAND_INCLUDED

#include "Command/Command.h"
#include "Memory/include/Memory.h"

// Place object on/in a level

namespace CoS
{
	namespace Reflection 
	{
		class ClassDef;
	}

	class Zone;
	class Transform;
}

namespace Editor 
{
	class DeleteObjectCommand;

	class PlaceObjectCommand : public Command
	{
		// command-specific data
		CoS::Reflection::ClassDef* m_pClassDef;
		CoS::Zone* m_pZone;
		CoS::Transform m_transform;
		unsigned int m_objId;

		// the opposite of a place-object is a delete-object...
		DeleteObjectCommand* m_pDeleteCmd;

	public:
		PlaceObjectCommand(
			CoS::Reflection::ClassDef* pClassDef,
			CoS::Zone* pZone,
			const CoS::Transform& xform,
			unsigned int objectId
			);
		~PlaceObjectCommand();

		// Command implementation
		void execute();
		void undo();

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // PLACEOBJECT_COMMAND_INCLUDED
