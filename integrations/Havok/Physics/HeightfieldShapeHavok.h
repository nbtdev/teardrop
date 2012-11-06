/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
