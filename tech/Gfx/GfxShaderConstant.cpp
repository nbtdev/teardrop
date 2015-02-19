/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxShaderConstant.h"
#include "Math/Matrix44.h"
#include "Math/Vector4.h"
#include <cstring>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxShaderConstant::GfxShaderConstant()
{
	m_pValue = 0;
	m_bMatrix = 0;
	m_name = 0;
	m_len = 0;
	m_bModified = 0;
}
//---------------------------------------------------------------------------
GfxShaderConstant::~GfxShaderConstant()
{
}
//---------------------------------------------------------------------------
void GfxShaderConstant::setValue(const Vector4* pValue)
{
	if (m_pValue)
	{
		memcpy(m_pValue, pValue, sizeof(Vector4) * m_len);
		m_bModified = 1;
		m_bMatrix = 0;
	}
}
//---------------------------------------------------------------------------
void GfxShaderConstant::setValue(const Matrix44* pValue)
{
	if (m_pValue)
	{
		memcpy(m_pValue, pValue, sizeof(Vector4) * m_len);
		m_bModified = 1;
		m_bMatrix = 1;
	}
}
//---------------------------------------------------------------------------
void GfxShaderConstant::setValue(float val)
{
	if (m_pValue)
	{
		// only sets the first component of the first Vector4 to which we point
		*((float*)m_pValue) = val;
		m_bModified = 1;
		m_bMatrix = 0;
	}
}
