/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EXECUTABLE_INCLUDED)
#define EXECUTABLE_INCLUDED

#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	class Logic;

	namespace Gfx {
		class Renderer;
		class RenderTarget;
	}

	class Executable : public Reflection::Object
	{
	public:
		TD_CLASS(Executable, Object);
		TD_POINTER_PROPERTY(Logic, "Pluggable logic module for this executable", Logic, 0);

		Executable();
		~Executable();

		virtual bool onPreLoad();
		virtual bool onPostLoad();
		virtual void tick();
		virtual void renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt);
		virtual bool onPreUnload();
		virtual bool onPostUnload();

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // EXECUTABLE_INCLUDED
