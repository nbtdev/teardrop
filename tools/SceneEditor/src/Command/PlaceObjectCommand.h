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
