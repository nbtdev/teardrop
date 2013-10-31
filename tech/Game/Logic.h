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
	class Executable;

	class Logic : public Reflection::Object
	{
	public:
		TD_CLASS(Logic, Object);

		Logic();
		~Logic();

		virtual bool onPreLoad(Executable* exe);
		virtual bool onPostLoad(Executable* exe);
		virtual bool update(Executable* exe);
		virtual bool onPreUnload(Executable* exe);
		virtual bool onPostUnload(Executable* exe);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // LOGIC_INCLUDED
