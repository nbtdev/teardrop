/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DYNAMICLIGHT_INCLUDED)
#define DYNAMICLIGHT_INCLUDED

#include "Gfx/GfxLight.h"
#include "Game/ZoneObject.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class DynamicLight
		: public ZoneObject,
		public GfxLight
	{

	public:
		TD_CLASS(DynamicLight, ZoneObject);
		TD_COMPLEX_PROPERTY(LightColor, "Light color", Vector4, "(1,1,1,1)", 0);
		TD_PROPERTY(CastsShadows, "Whether or not this light casts shadows", bool, true, 0);

		DynamicLight();
		virtual ~DynamicLight();

		// ZoneObject/Reflection::Object overrides
		bool initialize();
		bool destroy();

		//! GfxLight overrides
		const Vector4& getPosition() const;

		TD_DECLARE_ALLOCATOR();

	private:
		void notifyPropertyChangedLocal(const Reflection::PropertyDef* pPropDef);
	};
}

#endif // DYNAMICLIGHT_INCLUDED
