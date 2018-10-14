/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#if !defined(DYNAMICLIGHT_INCLUDED)
#define DYNAMICLIGHT_INCLUDED

#include "Gfx/Light.h"
#include "Game/ZoneObject.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class DynamicLight
        : public ZoneObject
        , public Gfx::Light
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
