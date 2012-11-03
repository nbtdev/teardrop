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

#if !defined(COSGFXVERTEXFORMAT_INCLUDED)
#define COSGFXVERTEXFORMAT_INCLUDED

#include "GfxCommon.h"

namespace CoS
{
	struct Environment;

	class GfxVertexFormat
	{
		DECLARE_SERIALIZABLE(GfxVertexFormat);

	public:
		const static int MAX_ELEMENT_COUNT = 8;

		struct Element
		{
			Element()
			{
				semantic = 0;
				type = 0;
				usage_source = 0;
				offset = 0;
			}

			void setUsage(unsigned char c)
			{
				usage_source &= 0xF0;
				usage_source |= (c & 0x0F);
			}

			unsigned char getUsage() const
			{
				return usage_source & 0x0F;
			}

			void setSource(size_t source)
			{
				usage_source &= 0x0F;
				usage_source |= ((unsigned char)source << 4);
			}

			unsigned char getSource() const
			{
				return (usage_source & 0xF0) >> 4;
			}

			unsigned char semantic;
			unsigned char type;
			unsigned char usage_source;
			unsigned char offset;
		};

		//! normal c'tor (cannot fail)
		GfxVertexFormat();
		//! placement c'tor (cannot fail and does not initialize)
		GfxVertexFormat(int i);
		//! copy c'tor (cannot fail)
		GfxVertexFormat(const GfxVertexFormat& other);
		//! d'tor (cannot fail)
		~GfxVertexFormat();

		//! initialize this (anything that can fail)
		bool initialize();
		//! destroy this
		bool destroy();
		//! create implementation-dependent structures
		bool fix(Environment& env);
		//! release implementation-dependent structures
		bool release();

		/** operators
		*/
		GfxVertexFormat& operator=(const GfxVertexFormat& other);

		/** accessors
		*/
		bool hasPosition() const;
		bool hasNormal() const;
		bool hasColor(int usage=0) const;
		bool hasTexCoord(int usage=0) const;
		bool hasBinormal(int usage=0) const;
		bool hasTangent(int usage=0) const;
		bool hasBlendweight() const;
		bool hasBlendindex() const;
		size_t getNumElements() const;
		size_t getVertexSize() const;
		const void* getImpl() const { return m_pImpl; }

		Element& getElementAt(size_t index);
		const Element& getElementAt(size_t index) const;
		size_t getElementOffset(size_t index);
		const Element& getElementBySemantic(VertexElementSemantic semantic, int usage=0) const;
		size_t getElementOffsetBySemantic(VertexElementSemantic semantic, int usage=0) const;

		/** mutators
		*/
		//! add element to end of element table; returns false if no more elements can fit
		bool addElement(const Element& element);
		//! remove element by semantic and usage; compacts table if needed; returns false if element not found
		bool removeElement(VertexElementSemantic semantic, int usage=0);
		//! align vertex size to the alignment specified (Havok for example likes 16-byte aligned element sizes)
		void alignSize(unsigned int  alignment);
		//! override the vertex element size (again, for havok, who seems to have a bug now and then in their tools)
		void overrideSize(unsigned int size);

		DECLARE_GFX_ALLOCATOR();

	private:
		// 48 bytes
		Element m_layout[MAX_ELEMENT_COUNT];		// 32
		SerialPointer<void> m_pImpl;				// 8
		unsigned int m_vertexSize;					// 4
		unsigned int m_elementCount;				// 4
	};
}

#endif // COSGFXVERTEXFORMAT_INCLUDED
