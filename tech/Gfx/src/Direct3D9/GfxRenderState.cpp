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

#include "stdafx.h"
#include "GfxRenderState.h"
#include "GfxRendererD3D9.h"
#include "GfxRenderTargetD3D9.h"
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxMaterial.h"
#include "Math/include/Matrix44.h"
#include "Math/include/Vector4.h"
#include "Util/include/Environment.h"
#include "Resource/include/ResourceManager.h"
#include "shader/GfxShaderCode.h"

using namespace CoS;
//---------------------------------------------------------------------------
static D3DCULL s_cullLUT[] = 
{
	/*
		CULL_NONE,
		CULL_CCW,
		CULL_CW
	*/
	D3DCULL_NONE,
	D3DCULL_CCW,
	D3DCULL_CW,
};
//---------------------------------------------------------------------------
GfxRenderState::AutoConstant GfxRenderState::m_autoConst[] = {
//   semantic				string name             len  arr  pD  upd  inv  trsp deps
	{AC_NONE,				"NONE",					4,   0,   0,  0,   0,   0,   {0, 0, 0}},

	{AC_WORLD,				"WORLD",				4,   0,   0,  1,   0,   0,   {0, 0, 0}},
	{AC_VIEW,				"VIEW",					4,   0,   0,  1,   0,   0,   {0, 0, 0}},
	{AC_PROJ,				"PROJ",					4,   0,   0,  1,   0,   0,   {0, 0, 0}},
	{AC_WORLDVIEW,			"WORLDVIEW",			4,   0,   0,  1,   0,   0,   {AC_WORLD, AC_VIEW, 0}},
	{AC_WORLDVIEWPROJ,		"WORLDVIEWPROJ",		4,   0,   0,  1,   0,   0,   {AC_WORLD, AC_VIEW, AC_PROJ}},
	{AC_VIEWPROJ,			"VIEWPROJ",				4,   0,   0,  1,   0,   0,   {AC_VIEW,  AC_PROJ, 0}},
	{AC_WORLDVIEWINV,		"WORLDVIEWINV",			4,   0,   0,  1,   1,   0,   {AC_WORLD, AC_VIEW, 0}},
	{AC_WORLDINV,			"WORLDINV",				4,   0,   0,  1,   1,   0,   {AC_WORLD, 0,       0}},
	{AC_VIEWINV,			"VIEWINV",				4,   0,   0,  1,   1,   0,   {AC_VIEW,  0,       0}},
	{AC_WORLDINVTRANS,		"WORLDINVTRANS",		4,   0,   0,  1,   1,   1,   {AC_WORLD, 0,       0}},
	{AC_WORLDVIEWINVTRANS,	"WORLDVIEWINVTRANS",	4,   0,   0,  1,   1,   1,   {AC_WORLD, AC_VIEW, 0}},
	{AC_WORLDVIEWTRANS,		"WORLDVIEWTRANS",		4,   0,   0,  1,   0,   1,   {AC_WORLD, AC_VIEW, 0}},
	{AC_LIGHTVIEWPROJ,		"LIGHTVIEWPROJ",		32,  1,   0,  0,   0,   1,   {0, 0, 0}},

	// NOTE -- only matrices above AC_AMBIENT (it's treated special in the shader autoparam code)
	{AC_AMBIENT,			"WORLDAMBIENT",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_CAMERADEPTH,		"CAMERADEPTH",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},

	// arbitrary limit of 8 lights max per pass...
	{AC_LIGHTPOS,			"LIGHTPOS",				8,   1,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_LIGHTDIR,			"LIGHTDIR",				8,   1,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_LIGHTCOL,			"LIGHTCOL",				8,   1,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_LIGHTSCOL,			"LIGHTSCOL",			8,   1,   0,  0,   0,   0,   {0, 0, 0}},

	{AC_AMBIENTCOL,			"AMBIENTCOL",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_DIFFUSECOL,			"DIFFUSECOL",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_SPECULARCOL,		"SPECULARCOL",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},
	{AC_EMISSIVECOL,		"EMISSIVECOL",			1,   0,   0,  0,   0,   0,   {0, 0, 0}},

	{AC_MATRIXPALETTE,		"MATRIXPALETTE",		256, 1,   0,  0,   0,   0,   {0, 0, 0}},
};
//---------------------------------------------------------------------------
GfxRenderState::GfxRenderState(Environment& e)
: env(e)
{
	char* pThis = (char*)this;
	pThis += sizeof(Environment&);
	memset(pThis, 0, sizeof(GfxRenderState) - sizeof(Environment&));

	for (int i=0; i<MAX_SAMPLERS; ++i)
	{
		samplers[i].addrU =
		samplers[i].addrV =
		samplers[i].addrW = GfxTextureStage::ADDRMODE_FORCE_SIZE;

		samplers[i].magFilter = 
		samplers[i].minFilter = 
		samplers[i].mipFilter = GfxTextureStage::FILTER_FORCE_SIZE;
	}

	// set some variables to match the D3D defaults
	depthWrite = true;
	depthCheck = true;
	srcBlend = D3DBLEND_ONE;
	destBlend = D3DBLEND_ZERO;
	depthFunc = D3DCMP_LESSEQUAL;
	cullMode = GfxMaterial::CULL_NONE;
	colorWrite = true;

	// initialize auto-constant table, data cache and semantic lookup table
	memset(m_constData, 0, sizeof(m_constData));
	m_autoConst[AC_NONE].pData = m_constData;

	size_t offset = 4;
	for (size_t i=1; i<AC_MAX; ++i)
	{
		m_acLut[String(m_autoConst[i].string)] = m_autoConst[i].semantic;
		m_autoConst[i].pData = m_constData + offset;
		offset += m_autoConst[i].len;
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::updateVSInvariantConstants()
{
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

	HRESULT hr = pRend->getDevice()->SetVertexShaderConstantF(
		(UINT)GfxShaderCode::INVARIANT_VSCONSTANTS_F_START,
		&vfConst[GfxShaderCode::INVARIANT_VSCONSTANTS_F_START][0],
		(UINT)GfxShaderCode::INVARIANT_VSCONSTANTS_F_COUNT);

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::updatePSInvariantConstants()
{
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

	HRESULT hr = pRend->getDevice()->SetPixelShaderConstantF(
		(UINT)GfxShaderCode::INVARIANT_PSCONSTANTS_F_START,
		&pfConst[GfxShaderCode::INVARIANT_PSCONSTANTS_F_START][0],
		(UINT)GfxShaderCode::INVARIANT_PSCONSTANTS_F_COUNT);

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::updateVSConstants()
{
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

	HRESULT hr = pRend->getDevice()->SetVertexShaderConstantF(
		(UINT)GfxShaderCode::VARIANT_VSCONSTANTS_F_START,
		&vfConst[GfxShaderCode::VARIANT_VSCONSTANTS_F_START][0],
		(UINT)GfxShaderCode::VARIANT_VSCONSTANTS_F_COUNT);

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::updateBoneConstants()
{
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

	HRESULT hr = pRend->getDevice()->SetVertexShaderConstantF(
		(UINT)GfxShaderCode::BONE_CONSTANTS_START,
		&vfConst[GfxShaderCode::BONE_CONSTANTS_START][0],
		(UINT)GfxShaderCode::BONE_CONSTANTS_COUNT);

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::updatePSConstants()
{
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

	HRESULT hr = pRend->getDevice()->SetPixelShaderConstantF(
		(UINT)GfxShaderCode::VARIANT_PSCONSTANTS_F_START,
		&pfConst[GfxShaderCode::VARIANT_PSCONSTANTS_F_START][0],
		(UINT)GfxShaderCode::VARIANT_PSCONSTANTS_F_COUNT);

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
//---------------------------------------------------------------------------
AutoConst GfxRenderState::getAutoConstBySemantic(
	const char* name)
{
	if (!name)
		return AC_NONE;

	return getAutoConstBySemantic(String(name));
}
//---------------------------------------------------------------------------
AutoConst GfxRenderState::getAutoConstBySemantic(
	const String& semantic)
{
	AutoConstByName::iterator it = m_acLut.find(semantic);
	if (it == m_acLut.end())
		return AC_NONE;

	return it->second;
}
//---------------------------------------------------------------------------
const Vector4* GfxRenderState::getConstant(AutoConst c)
{
	if (c >= AC_MAX)
		return m_autoConst[AC_NONE].pData; // should always point to zeros

	AutoConstant* pAC = &m_autoConst[c];

	// len of 4 and not an array means (for now) that it's a matrix, and we 
	// need to check to see if it needs updating before we return its data
	if (pAC->len == 4 && !pAC->isArray && pAC->needsUpdate)
	{
		Matrix44 tmp, tmp2;
		size_t* pDep = pAC->deps;

		if (pDep[0]) 
		{
			if (pDep[1]) 
			{
				mul(tmp, 
					(Matrix44&)*(m_autoConst[pDep[0]].pData),
					(Matrix44&)*(m_autoConst[pDep[1]].pData));
			}
			else
			{
				tmp = (Matrix44&)*(m_autoConst[pDep[0]].pData);
			}

			if (pDep[2])
			{
				mul(tmp, 
					tmp, 
					(Matrix44&)*(m_autoConst[pDep[2]].pData));
			}

			((Matrix44&)*(pAC->pData)) = tmp;
		}

		if (pAC->invert)
		{
			tmp.invert((Matrix44&)*(pAC->pData));
			tmp = (Matrix44&)*(pAC->pData);
		}

		if (pAC->transpose)
		{
			tmp.transpose((Matrix44&)*(pAC->pData));
		}

		pAC->needsUpdate = 0;
	}

	return pAC->pData;
}
//---------------------------------------------------------------------------
static void setEntry(float* start, const float* src, size_t size)
{
	memcpy(start, src, size);
}
//---------------------------------------------------------------------------
void GfxRenderState::setCameraDepth(const Vector4& depth)
{
	*(m_autoConst[AC_CAMERADEPTH].pData) = depth;
}
//---------------------------------------------------------------------------
void GfxRenderState::setWorldMatrix(const Matrix44& m)
{
	// update matrix cache
	((Matrix44&)*(m_autoConst[AC_WORLD].pData)) = m;
	m_autoConst[AC_WORLDVIEW].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWINV].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWTRANS].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWINVTRANS].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWPROJ].needsUpdate = 1;
}
//---------------------------------------------------------------------------
void GfxRenderState::setViewMatrix(const Matrix44& m)
{
	// update matrix cache
	((Matrix44&)*(m_autoConst[AC_VIEW].pData)) = m;
	m_autoConst[AC_VIEW].needsUpdate = 1;
	m_autoConst[AC_VIEWINV].needsUpdate = 1;
	m_autoConst[AC_VIEWPROJ].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEW].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWINV].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWTRANS].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWINVTRANS].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWPROJ].needsUpdate = 1;
}
//---------------------------------------------------------------------------
void GfxRenderState::setProjectionMatrix(const Matrix44& m)
{
	// update matrix cache
	((Matrix44&)*(m_autoConst[AC_PROJ].pData)) = m;
	m_autoConst[AC_VIEWPROJ].needsUpdate = 1;
	m_autoConst[AC_WORLDVIEWPROJ].needsUpdate = 1;
}
//---------------------------------------------------------------------------
void GfxRenderState::setLightViewProjMatrix(const Matrix44& m, size_t index)
{
	if (index >= 8)
		return;

	// update matrix cache
	Matrix44* pMat = (Matrix44*)m_autoConst[AC_LIGHTVIEWPROJ].pData;
	pMat[index] = m;
}
//---------------------------------------------------------------------------
void GfxRenderState::setViewProjectionMatrix(const Matrix44& m)
{
	//Matrix44 t;
	//m.transpose(t);

	setEntry(
		&vfConst[GfxShaderCode::VIEWPROJ][0],
		m._m,
		sizeof(Matrix44)
		);
}
//---------------------------------------------------------------------------
bool GfxRenderState::getColorWrite()
{
	return colorWrite;
}
//---------------------------------------------------------------------------
void GfxRenderState::setColorWrite(bool bWrite)
{
	if (bWrite != colorWrite)
	{
		colorWrite = bWrite;

		UINT val = bWrite ? 
			D3DCOLORWRITEENABLE_ALPHA|
			D3DCOLORWRITEENABLE_BLUE|
			D3DCOLORWRITEENABLE_GREEN|
			D3DCOLORWRITEENABLE_RED
			: 0;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_COLORWRITEENABLE, val);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setWVP(const Matrix44& m)
{
	setEntry(
		&vfConst[GfxShaderCode::WVP][0],
		m._m,
		sizeof(Matrix44)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setViewInverse(const Matrix44& m)
{
	setEntry(
		&vfConst[GfxShaderCode::VI][0],
		m._m,
		sizeof(Matrix44)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setTextureMatrix(const Matrix44& m, size_t index)
{
	setEntry(
		&vfConst[GfxShaderCode::TEXMAT + index][0],
		m._m,
		sizeof(Matrix44)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setMatrixPaletteEntry(const Matrix44& m, size_t index)
{
	Matrix44 tmp;
	m.transpose(tmp);

	Vector4* pData = m_autoConst[AC_MATRIXPALETTE].pData;
	memcpy(pData + index*3, tmp._m, sizeof(Vector4)*3);
}
//---------------------------------------------------------------------------
void GfxRenderState::setLightPos(const Vector4& v, size_t index)
{
	*(m_autoConst[AC_LIGHTPOS].pData + index) = v;
	setEntry(
		&vfConst[GfxShaderCode::LIGHT0POS + index][0],
		&v.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setLightDir(const Vector4& v, size_t index)
{
	*(m_autoConst[AC_LIGHTDIR].pData + index) = v;
	setEntry(
		&vfConst[GfxShaderCode::LIGHT0DIR + index][0],
		&v.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setLightColor(const Vector4& v, size_t index)
{
	*(m_autoConst[AC_LIGHTCOL].pData + index) = v;
	setEntry(
		&pfConst[GfxShaderCode::LIGHT0COL + index][0],
		&v.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setLightSpecularColor(const Vector4& v, size_t index)
{
	*(m_autoConst[AC_LIGHTSCOL].pData + index) = v;
	setEntry(
		&pfConst[GfxShaderCode::LIGHT0SCOL + index][0],
		&v.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setWorldAmbient(const Vector4& v)
{
	*(m_autoConst[AC_AMBIENT].pData) = v;
	setEntry(
		&pfConst[GfxShaderCode::AMBIENT][0],
		&v.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setMaterialDiffuse(const Vector4& v)
{
	// swizzle because the vertex format and the shader constant form are
	// different (ARGB in vertex vs RGBA in shader constant)
	Vector4 tmpV(v);
	tmpV.x = v.w;
	tmpV.y = v.x;
	tmpV.z = v.y;
	tmpV.w = v.z;
	*(m_autoConst[AC_DIFFUSECOL].pData) = tmpV;
	setEntry(
		&pfConst[GfxShaderCode::DIFFUSE_COL][0],
		&tmpV.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setMaterialAmbient(const Vector4& v)
{
	// swizzle because the vertex format and the shader constant form are
	// different (ARGB in vertex vs RGBA in shader constant)
	Vector4 tmpV(v);
	tmpV.x = v.w;
	tmpV.y = v.x;
	tmpV.z = v.y;
	tmpV.w = v.z;
	*(m_autoConst[AC_AMBIENTCOL].pData) = tmpV;
	setEntry(
		&pfConst[GfxShaderCode::AMBIENT_COL][0],
		&tmpV.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setMaterialSpecular(const Vector4& v)
{
	// swizzle because the vertex format and the shader constant form are
	// different (ARGB in vertex vs RGBA in shader constant)
	Vector4 tmpV(v);
	tmpV.x = v.w;
	tmpV.y = v.x;
	tmpV.z = v.y;
	tmpV.w = v.z;
	*(m_autoConst[AC_SPECULARCOL].pData) = tmpV;
	setEntry(
		&pfConst[GfxShaderCode::SPECULAR_COL][0],
		&tmpV.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setMaterialEmissive(const Vector4& v)
{
	// swizzle because the vertex format and the shader constant form are
	// different (ARGB in vertex vs RGBA in shader constant)
	Vector4 tmpV(v);
	tmpV.x = v.w;
	tmpV.y = v.x;
	tmpV.z = v.y;
	tmpV.w = v.z;
	*(m_autoConst[AC_EMISSIVECOL].pData) = tmpV;
	setEntry(
		&pfConst[GfxShaderCode::EMISSIVE_COL][0],
		&tmpV.x,
		sizeof(Vector4)
		);
}
//---------------------------------------------------------------------------
void GfxRenderState::setDepthWrite(bool write)
{
	if (depthWrite != write)
	{
		depthWrite = write;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);

		HRESULT hr = pRend->getDevice()->SetRenderState(D3DRS_ZWRITEENABLE, write ? TRUE : FALSE);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setDepthCheck(bool check)
{
	if (depthCheck != check)
	{
		depthCheck = check;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_ZFUNC, check ? depthFunc : D3DCMP_ALWAYS);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setDepthBias(unsigned int bias)
{
	if (depthBias != bias)
	{
		depthBias = bias;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_DEPTHBIAS, (DWORD)bias);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setFillMode(GfxMaterial::FillMode mode)
{
	if (fillMode != mode)
	{
		fillMode = mode;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_FILLMODE, (DWORD)mode);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setDepthFunc(unsigned int func)
{
	if (depthFunc != func)
	{
		depthFunc = func;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_ZFUNC, depthCheck ? depthFunc : D3DCMP_ALWAYS);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::enableAlphaBlend(bool enable)
{
	if (alphaBlend != enable)
	{
		alphaBlend = enable;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(
			D3DRS_ALPHABLENDENABLE, enable ? TRUE : FALSE);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setSrcBlend(unsigned int blend)
{
	if (srcBlend != blend)
	{
		srcBlend = blend;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(D3DRS_SRCBLEND, blend);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setDestBlend(unsigned int blend)
{
	if (destBlend != blend)
	{
		destBlend = blend;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(D3DRS_DESTBLEND, blend);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setCullMode(GfxMaterial::CullMode mode)
{
	if (cullMode != mode)
	{
		cullMode = mode;

		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		HRESULT hr = pRend->getDevice()->SetRenderState(D3DRS_CULLMODE, s_cullLUT[mode]);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
// map some Gfx values to D3D ones
/*  
	filtering
	NONE=0,
	NEAREST,
	BILINEAR,
	ANISOTROPIC,

	addressing
	UNSET=0,
	WRAP,
	MIRROR,
	CLAMP,
	BORDER,
*/

static DWORD D3D9FilterMap[] = 
{
	D3DTEXF_LINEAR,
	D3DTEXF_POINT,
	D3DTEXF_LINEAR,
	D3DTEXF_ANISOTROPIC,
};

static DWORD D3D9AddressMap[] = 
{
	D3DTADDRESS_WRAP,
	D3DTADDRESS_WRAP,
	D3DTADDRESS_MIRROR,
	D3DTADDRESS_CLAMP,
	D3DTADDRESS_BORDER,
};
//---------------------------------------------------------------------------
bool GfxRenderState::setSampler(unsigned int s, GfxTexture* pTex)
{
	if (s >= MAX_SAMPLERS)
	{
		return false;
	}

	SamplerState& sampler = samplers[s];
	if (!pTex)
	{
		// clear and return
		sampler.pTexture = 0;
		return false;
	}

	if (pTex && sampler.pTexture != pTex)
	{
		if (pTex->setToSampler(s))
		{
			sampler.pTexture = pTex;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void GfxRenderState::setSampler(unsigned int s, GfxTextureStage* pStage)
{
	if (!pStage)
	{
		setSampler(s, (GfxTexture*)0);
		return;
	}

	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	GfxTexture* pTex = AutoResPtr<GfxTexture>(pStage->getTextureHandle());
	if (!setSampler(s, pTex))
		return;

	// need to set up the stage, if needed
	HRESULT hr;
	SamplerState& sampler = samplers[s];
	GfxTextureStage::Filter mn, mg, mp;
	GfxTextureStage::AddressMode u, v, w;

	pStage->getFilter(mn, mg, mp);
	pStage->getTextureAddressing(u, v, w);

	if (sampler.minFilter != unsigned int(mn))
	{
		sampler.minFilter = mn;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_MINFILTER, D3D9FilterMap[mn]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}

	if (sampler.magFilter != unsigned int(mg))
	{
		sampler.magFilter = mg;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_MAGFILTER, D3D9FilterMap[mg]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}

	if (sampler.mipFilter != unsigned int(mp))
	{
		sampler.mipFilter = mp;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_MIPFILTER, D3D9FilterMap[mp]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}

	if (sampler.addrU != unsigned int(u))
	{
		sampler.addrU = u;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_ADDRESSU, D3D9AddressMap[u]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}

	if (sampler.addrV != unsigned int(v))
	{
		sampler.addrV = v;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_ADDRESSV, D3D9AddressMap[v]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}

	if (sampler.addrW != unsigned int(w))
	{
		sampler.addrW = w;
		hr = pRend->getDevice()->SetSamplerState(s, D3DSAMP_ADDRESSW, D3D9AddressMap[w]);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
void GfxRenderState::setRenderTarget(
	size_t idx, GfxRenderTargetD3D9* rt, bool force)
{
	if (idx > 4)
	{
		return;
	}

	if (pRT[idx] == rt && !force)
	{
		return;
	}

    Environment& env = Environment::get();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	HRESULT hr = pRend->getDevice()->SetRenderTarget((DWORD)idx, rt->getRenderSurface());

	IDirect3DSurface9* pDepth = rt->getDepthStencil();
	if (pDepth)
	{
		hr = pRend->getDevice()->SetDepthStencilSurface(pDepth);
	}

	if (FAILED(hr))
	{
		pRend->logD3DError(hr);
	}
}
