/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXSHADERCONSTANT_INCLUDED)
#define GFXSHADERCONSTANT_INCLUDED

#include <Gfx/GfxCommon.h>
#include <cstddef>
#include <cstdint>

namespace Teardrop
{
	class Vector4;
	class Matrix44;

	class GfxShaderConstant
	{
		friend class GfxShaderConstantTable;

        uint64_t m_name;
		size_t m_bMatrix;
		size_t m_len;
		Vector4* m_pValue;
		size_t m_bModified;

	public:
		GfxShaderConstant();
		~GfxShaderConstant();

		// convenience method
		bool isModified() const { return (m_bModified != 0); }
		bool isMatrix() const { return m_bMatrix != 0; }
		void clearModified() { m_bModified = 0; }
        uint64_t getName() const { return m_name; }
		const Vector4* getValue() const { return m_pValue; }
		// assumes that val points to at least enough memory to cover m_len Vector4's
		void setValue(const Vector4* val);
		void setValue(const Matrix44* val);
		void setValue(float val);

		DECLARE_GFX_ALLOCATOR();

	};
}

#endif // GFXSHADERCONSTANT_INCLUDED
