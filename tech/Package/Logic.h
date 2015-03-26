/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LOGIC_INCLUDED)
#define LOGIC_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	class Executable;

	class Logic : public Reflection::Object
	{
	public:
		TD_CLASS(Logic, Object);
		TD_POINTER_PROPERTY(CameraController, "", CameraController, 0);

		Logic();
		~Logic();

		virtual bool attached(Executable* exe);
		virtual bool onPreLoad(Executable* exe);
		virtual bool onPostLoad(Executable* exe);
		virtual bool update(Executable* exe);
		virtual bool onPreUnload(Executable* exe);
		virtual bool onPostUnload(Executable* exe);
		virtual bool detached(Executable* exe);

		virtual void injectMouseMove(int absX, int absY, int relX=0, int relY=0);
		virtual void injectMouseWheel(int absZ, int relZ=0);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // LOGIC_INCLUDED
