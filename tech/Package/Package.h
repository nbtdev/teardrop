/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGE_INCLUDED)
#define PACKAGE_INCLUDED

#include "Memory/Memory.h"
#include <list>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	typedef std::list<Reflection::Object*> Objects;

	class Package
	{
	public:
		Package();
		~Package();

		void add(Reflection::Object* object);
		void remove(Reflection::Object* object);
		const Objects& objects() const;

		TD_DECLARE_ALLOCATOR();

	protected:
		Objects mObjects;
	};
} // namespace Teardrop

#endif // PACKAGE_INCLUDED
