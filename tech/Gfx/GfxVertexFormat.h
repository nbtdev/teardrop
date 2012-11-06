/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXVERTEXFORMAT_INCLUDED)
#define TDGFXVERTEXFORMAT_INCLUDED

#include "GfxCommon.h"

namespace Teardrop
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

#endif // TDGFXVERTEXFORMAT_INCLUDED
