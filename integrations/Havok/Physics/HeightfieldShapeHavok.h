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

#if !defined(HEIGHTFIELDSHAPEHAVOK_INCLUDED)
#define HEIGHTFIELDSHAPEHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "ShapeHavok.h"
#include "Math/Vector4.h"
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>

namespace Teardrop
{
	class HeightfieldShapeHavok : public ShapeHavok, public hkpSampledHeightFieldShape
	{
		size_t m_width;
		size_t m_height;
		size_t m_bpp;
		float m_ooy;
		void* m_pData;

	public:
		HeightfieldShapeHavok();
		~HeightfieldShapeHavok();

		//! HeightfieldShape implementation
		bool initialize();
		bool release();
		bool update(float deltaT);

		//! hkpSampledHeightFieldShape implementation
		HeightfieldShapeHavok(const hkpSampledHeightFieldBaseCinfo& ci);
		HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
		{
			return hkReal(((unsigned char*)m_pData)[z * m_width + x]);
		}

		HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
		{
			return false;
		}

		void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
		{
			hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
		}

		TD_DECLARE_ALLOCATOR();

	protected:

	};
}

#endif // HEIGHTFIELDSHAPEHAVOK_INCLUDED
