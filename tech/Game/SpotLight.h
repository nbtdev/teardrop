/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(SPOTLIGHT_INCLUDED)
#define SPOTLIGHT_INCLUDED

#include "Game/DynamicLight.h"

namespace Teardrop
{
	class SpotLight
		: public DynamicLight
	{

	public:
		TD_CLASS(SpotLight, DynamicLight);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Range, "Effective radius (used for culling)", float, 1000, 0);
		TD_COMPLEX_PROPERTY(Attenuation, "Attenuation parameters (constant, linear, quadratic)", Vector4, "(1,0,0,0)", 0);
		TD_PROPERTY(InnerAngle, "Inner cone angle (degrees)", float, 30, 0);
		TD_PROPERTY(OuterAngle, "Outer cone angle (degrees)", float, 60, 0);

		SpotLight();
		virtual ~SpotLight();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SPOTLIGHT_INCLUDED
