/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxShaderConstantTable.h"
#include "GfxShaderConstant.h"
#include "Math/Vector4.h"
#include "Util/_String.h"
#include "Util/Hash.h"
#include <algorithm>
#include <cstdint>
#include <cstring>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxShaderConstantTable::GfxShaderConstantTable()
{
	m_pData = 0;
	m_numReg = 0;
}
//---------------------------------------------------------------------------
GfxShaderConstantTable::~GfxShaderConstantTable()
{
	if (m_pData)
		GFX_FREE(m_pData);
}
//---------------------------------------------------------------------------
void GfxShaderConstantTable::begin()
{
	m_constants.clear();
	m_numReg = 0;
	if (m_pData)
	{
		GFX_FREE(m_pData);
		m_pData = 0;
	}
}
//---------------------------------------------------------------------------
static bool UDless(const GfxShaderConstant& r1, const GfxShaderConstant& r2) 
{
	return (r1.getName() < r2.getName());
}
//---------------------------------------------------------------------------
void GfxShaderConstantTable::end()
{
	m_pData = (Vector4*)GFX_ALLOCATE_ALIGNED(sizeof(Vector4)*m_numReg, 16);
	memset(m_pData, 0, sizeof(Vector4)*m_numReg);

	// sort the vector elements in order of their hash ID
	std::sort(m_constants.begin(), m_constants.end(), UDless);

	Vector4* p = m_pData;
	for (size_t i=0; i<m_constants.size(); ++i)
	{
		GfxShaderConstant& c = m_constants[i];

		if (c.m_pValue)
		{
			// first, copy any default value into the data
			memcpy(p, c.m_pValue, sizeof(Vector4)*c.m_len);
		}

		c.m_pValue = p;
		p += c.m_len;
	}
}
//---------------------------------------------------------------------------
GfxShaderConstant* GfxShaderConstantTable::addFloatConstant(
	const String& name, size_t len, Vector4* pDefault)
{
	GfxShaderConstant c;
	c.m_len = len;
	c.m_name = hashString64(name);
	c.m_pValue = pDefault;
	m_numReg += len;
	m_constants.push_back(c);

	return &m_constants[m_constants.size()-1];
}
//---------------------------------------------------------------------------
GfxShaderConstant* GfxShaderConstantTable::addMatrixConstant(
	const String& name, size_t len, Matrix44* pDefault)
{
	GfxShaderConstant c;
	c.m_len = len * 4; // float4x4 occupies 4 constant slots
	c.m_name = hashString64(name);
	c.m_pValue = (Vector4*)pDefault;
	m_numReg += c.m_len;
	m_constants.push_back(c);

	return &m_constants[m_constants.size()-1];
}
//---------------------------------------------------------------------------
size_t GfxShaderConstantTable::getNumConstants() const
{
	return m_constants.size();
}
//---------------------------------------------------------------------------
GfxShaderConstant* GfxShaderConstantTable::getConstant(size_t index)
{
	if (index < m_constants.size())
		return &m_constants[index];

	return 0;
}
//---------------------------------------------------------------------------
GfxShaderConstant* GfxShaderConstantTable::getConstant(const String& name)
{
    uint64_t hash = hashString64(name);

	for (size_t i=0; i<m_constants.size(); ++i)
	{
		if (m_constants[i].m_name == hash)
			return &m_constants[i];
	}

	return 0;
}
