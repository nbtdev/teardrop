/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROP_INCLUDED)
#define PROP_INCLUDED

#include "Game/ZoneObject.h"
#include "Memory/Allocators.h"

class TiXmlElement;

namespace Teardrop
{
	class Stream;

	class Prop : public ZoneObject
	{
	public:
		TD_CLASS(Prop, ZoneObject);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(Dynamic, "Whether this prop participates in active dynamic sim", bool, false, 0);

		Prop();
		virtual ~Prop();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		static Prop* create(TiXmlElement* pElem, const char* pBasePath);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // PROP_INCLUDED
