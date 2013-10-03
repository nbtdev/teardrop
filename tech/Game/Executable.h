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

	class Executable : public Reflection::Object
	{
	public:
		TD_CLASS(Executable, Object);
		TD_POINTER_PROPERTY(Logic, "Pluggable logic module for this executable", Logic);

		Executable();
		~Executable();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // EXECUTABLE_INCLUDED
