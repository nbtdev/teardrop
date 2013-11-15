/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxVertexFormat.h"
#include "GfxUtil.h"
#include <memory.h>
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxVertexFormat);
//---------------------------------------------------------------------------
static GfxVertexFormat::Element s_nullElement;
//---------------------------------------------------------------------------
GfxVertexFormat::GfxVertexFormat()
	: m_pImpl(0),
	m_vertexSize(0),
	m_elementCount(0)
{
}
//---------------------------------------------------------------------------
// placement c'tor
GfxVertexFormat::GfxVertexFormat(int i) 
{
	// TODO: need to do something with the Pimpl on placement-new
	// i.e. it's a IDirect3DVertexDeclaration9 on D3D9, and so on...
	m_pImpl = 0;
	UNREFERENCED_PARAMETER(i);
}
//---------------------------------------------------------------------------
GfxVertexFormat::GfxVertexFormat(const GfxVertexFormat& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
GfxVertexFormat::~GfxVertexFormat()
{
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
GfxVertexFormat& GfxVertexFormat::operator=(const GfxVertexFormat& other)
{
	memcpy(m_layout, other.m_layout, sizeof(m_layout));
	m_vertexSize = other.m_vertexSize;
	m_elementCount = other.m_elementCount;

	// TODO: need to find a way to wrap this so that when it's a ref-counted 
	// pointer (D3D9) that its ref count gets managed properly
	m_pImpl = other.m_pImpl;

	return *this;
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasPosition() const
{
	const Element& elem = getElementBySemantic(POSITION);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasNormal() const
{
	const Element& elem = getElementBySemantic(NORMAL);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasColor(int usage) const
{
	const Element& elem = getElementBySemantic(COLOR, usage);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasTexCoord(int usage) const
{
	const Element& elem = getElementBySemantic(TEXCOORD, usage);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasBinormal(int usage) const
{
	const Element& elem = getElementBySemantic(BINORMAL, usage);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasTangent(int usage) const
{
	const Element& elem = getElementBySemantic(TANGENT, usage);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasBlendweight() const
{
	const Element& elem = getElementBySemantic(BLENDWEIGHT);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::hasBlendindex() const
{
	const Element& elem = getElementBySemantic(BLENDINDEX);
	return (elem.semantic != UNUSED);
}
//---------------------------------------------------------------------------
size_t GfxVertexFormat::getNumElements() const
{
	return m_elementCount;
}
//---------------------------------------------------------------------------
size_t GfxVertexFormat::getVertexSize() const
{
	return m_vertexSize;
}
//---------------------------------------------------------------------------
void GfxVertexFormat::alignSize(unsigned int alignment)
{
	unsigned int rem = m_vertexSize % alignment;
	if (rem)
	{
		m_vertexSize += (alignment - rem);
	}
}
//---------------------------------------------------------------------------
void GfxVertexFormat::overrideSize(unsigned int size)
{
	m_vertexSize = size;
}
//---------------------------------------------------------------------------
GfxVertexFormat::Element& GfxVertexFormat::getElementAt(size_t index)
{
	assert(index < MAX_ELEMENT_COUNT);

	if (index >= MAX_ELEMENT_COUNT)
	{
		return s_nullElement;
	}

	return m_layout[index];
}
//---------------------------------------------------------------------------
const GfxVertexFormat::Element& GfxVertexFormat::getElementAt(size_t index) const
{
	assert(index < MAX_ELEMENT_COUNT);

	if (index >= MAX_ELEMENT_COUNT)
	{
		return s_nullElement;
	}

	return m_layout[index];
}
//---------------------------------------------------------------------------
size_t GfxVertexFormat::getElementOffset(size_t index)
{
	Element& elem = getElementAt(index);
	if (elem.semantic == UNUSED)
	{
		return 0xFFFFFFFF;
	}

	return elem.offset;
}
//---------------------------------------------------------------------------
const GfxVertexFormat::Element& GfxVertexFormat::getElementBySemantic(
	VertexElementSemantic semantic, int usage) const
{
	for (size_t i=0; i<MAX_ELEMENT_COUNT; ++i)
	{
		const Element& elem = m_layout[i];
		if (elem.semantic == semantic && elem.getUsage() == usage)
		{
			return elem;
		}
	}

	return s_nullElement;
}
//---------------------------------------------------------------------------
size_t GfxVertexFormat::getElementOffsetBySemantic(
	VertexElementSemantic semantic, int usage) const
{
	const Element& elem = getElementBySemantic(semantic, usage);
	if (elem.semantic == UNUSED)
	{
		return 0xFFFFFFFF;
	}

	return elem.offset;
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::addElement(const Element& element)
{
	assert(m_elementCount < MAX_ELEMENT_COUNT);

	for (size_t i=m_elementCount; i<MAX_ELEMENT_COUNT; ++i)
	{
		if (m_layout[i].semantic == UNUSED)
		{
			m_layout[i] = element;
			m_elementCount++;
			m_vertexSize += (unsigned char)GfxUtil::getSizeOf(
				(VertexElementType)element.type);
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::removeElement(VertexElementSemantic semantic, int usage)
{
	assert(m_elementCount > 0);

	for (size_t i=m_elementCount; i<MAX_ELEMENT_COUNT; ++i)
	{
		Element& elem = m_layout[i];
		if (elem.semantic == semantic && elem.getUsage() == usage)
		{
			// reduce the current vertex size and element count
			m_vertexSize -= (unsigned char)GfxUtil::getSizeOf(
				(VertexElementType)elem.type);
			m_elementCount--;

			// compact table by moving all higher-ordinal elements down one
			for (size_t j=i+1; j<MAX_ELEMENT_COUNT; ++j)
			{
				m_layout[i] = m_layout[j];
				++i;
			}

			// zero out the last element (in case the array was full before)
			m_layout[MAX_ELEMENT_COUNT] = s_nullElement;
			return true;
		}
	}

	return false;
}
