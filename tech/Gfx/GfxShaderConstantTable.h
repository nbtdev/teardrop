/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(GFXSHADERCONSTANTTABLE_INCLUDED)
#define GFXSHADERCONSTANTTABLE_INCLUDED

#include <vector>
#include "Gfx/GfxShaderConstant.h"

namespace CoS
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
