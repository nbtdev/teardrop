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

#if !defined(GFXSHADERCONSTANT_INCLUDED)
#define GFXSHADERCONSTANT_INCLUDED

namespace CoS
{
	class Vector4;
	class Matrix44;

	class GfxShaderConstant
	{
		friend class GfxShaderConstantTable;

		unsigned __int64 m_name;
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
		unsigned __int64 getName() const { return m_name; }
		const Vector4* getValue() const { return m_pValue; }
		// assumes that val points to at least enough memory to cover m_len Vector4's
		void setValue(const Vector4* val);
		void setValue(const Matrix44* val);
		void setValue(float val);

		DECLARE_GFX_ALLOCATOR();

	};
}

#endif // GFXSHADERCONSTANT_INCLUDED
