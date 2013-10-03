/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LOGIC_INCLUDED)
#define LOGIC_INCLUDED

#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"

namespace Teardrop
{
	class Logic : public Reflection::Object
	{
	public:
		TD_CLASS(Logic, Object);

		Logic();
		~Logic();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // LOGIC_INCLUDED
