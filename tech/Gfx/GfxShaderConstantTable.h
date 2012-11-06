/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXSHADERCONSTANTTABLE_INCLUDED)
#define GFXSHADERCONSTANTTABLE_INCLUDED

#include <vector>
#include "Gfx/GfxShaderConstant.h"

namespace Teardrop
{
	class String;
	class Matrix44;
	class Vector4;
	class GfxShaderConstant;

	class GfxShaderConstantTable
	{
		typedef std::vector<GfxShaderConstant> Constants;
		Constants m_constants;
		Vector4* m_pData;
		size_t m_numReg;

	public:
		GfxShaderConstantTable();
		~GfxShaderConstantTable();

		// begin constructing a table
		void begin();
		// add a float constant to the table; len is the optional length of a constant array
		GfxShaderConstant* addFloatConstant(const String& name, size_t len=1, Vector4* pDefault=0);
		// add a float4x4 constant to the table; len is the optional length of a constant array
		GfxShaderConstant* addMatrixConstant(const String& name, size_t len=1, Matrix44* pDefault=0);
		// end construction and finalize the table
		void end();
		// return number of constants defined in this table
		size_t getNumConstants() const;
		// return a constant entry by index
		GfxShaderConstant* getConstant(size_t index);
		// return a constant entry by name
		GfxShaderConstant* getConstant(const String& name);

		DECLARE_GFX_ALLOCATOR();
	};
}

#endif // GFXSHADERCONSTANTTABLE_INCLUDED
