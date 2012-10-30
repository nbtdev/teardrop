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
#include "GfxShaderD3D9.h"
#include "GfxRenderer.h"
#include "../GfxRendererD3D9.h"
#include "GfxSubMesh.h"
#include "GfxMaterial.h"
#include "GfxTextureStage.h"
#include "GfxTexture.h"
#include "GfxShaderCode.h"
#include "GfxShaderConstantTable.h"
#include "GfxShaderConstant.h"
#include "Util/include/Logger.h"
#include "Util/include/Hash.h"
#include "Util/include/Environment.h"
#include "Resource/include/ResourceManager.h"

#include <assert.h>
#include <sstream>

using namespace CoS;
//---------------------------------------------------------------------------
GfxShaderD3D9::GfxShaderD3D9(ID3DXEffectPool* pPool)
{
	m_pFX = 0;
	m_pFXPool = pPool;
	m_technique = 0;
	m_techniqueShadowCast = 0;
	m_techniqueShadowRecv = 0;
}
//---------------------------------------------------------------------------
GfxShaderD3D9::~GfxShaderD3D9()
{
}
//---------------------------------------------------------------------------
void GfxShaderD3D9::destroy()
{
	if (m_pFX)
		m_pFX->Release();
}
//---------------------------------------------------------------------------
bool GfxShaderD3D9::compile(const String& source, String& errs)
{
	return initialize(source, errs);
}
//---------------------------------------------------------------------------
bool GfxShaderD3D9::UDless(
	const ConstantTableEntry& r1, const ConstantTableEntry& r2) 
{
	return (r1.name < r2.name);
}
//---------------------------------------------------------------------------
bool GfxShaderD3D9::initD3D9(String& errs) 
{
	Environment& env = Environment::get();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();

	// create an effect from the source
	LPD3DXBUFFER pErr;

	// defines to alter the shader code generation
	D3DXMACRO defines[] = 
	{
		{ 0, 0 },
	};
	HRESULT hr = D3DXCreateEffectEx(
		pDevice,
		(const char*)m_source,
		m_source.length(),
		defines,						// defines
		0,								// includes
		0,								// skipConstants
		D3DXFX_DONOTSAVESTATE 			// flags
		/*| D3DXSHADER_NO_PRESHADER*/,
		m_pFXPool,						// effect pool
		&m_pFX,							// FX pointer
		&pErr							// compilation errors
		);

	if (FAILED(hr))
	{
		if (pErr)
		{
			char* pMsgs = (char*)pErr->GetBufferPointer();
			errs = pMsgs;
			m_errs = pMsgs;
			env.pLogger->logMessage((const char*)pMsgs);
			pErr->Release();
		}

		pRend->logD3DError(hr);
		return false;
	}

	hr = m_pFX->FindNextValidTechnique(0, &m_technique);
	if (FAILED(hr))
	{
		char buf[128];
		sprintf_s(buf, 128, "Could not find any valid techniques for shader '...'");
		env.pLogger->logMessage(buf);
		return false;
	}

	setupParams(m_constants);

	// get any defined shadow casting and receiving techniques
	D3DXTECHNIQUE_DESC techDesc;
	hr = m_pFX->GetTechniqueDesc(m_technique, &techDesc);
	if (SUCCEEDED(hr))
	{
		String castName(techDesc.Name);
		castName += "_ShadowCaster";
		m_techniqueShadowCast = m_pFX->GetTechniqueByName(castName);

		String recvName(techDesc.Name);
		recvName += "_ShadowReceiver";
		m_techniqueShadowRecv = m_pFX->GetTechniqueByName(recvName);
	}

	// track this shader for device reset reasons
	pRend->trackShader(this, true);

	return true;
}
//---------------------------------------------------------------------------
void GfxShaderD3D9::setupParams(Constants& constants)
{
	Environment& env = Environment::get();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	GfxRenderState* pState = pRend->getState();

	// set up the parameter mappings
	D3DXEFFECT_DESC desc;
	m_pFX->GetDesc(&desc);
	for (UINT i=0; i<desc.Parameters; ++i)
	{
		D3DXHANDLE hParam = m_pFX->GetParameter(0, i);

		// obtain the semantic for the parameter, or the name if no semantic
		D3DXPARAMETER_DESC paramDesc;
		m_pFX->GetParameterDesc(hParam, &paramDesc);

		if (paramDesc.Type >= D3DXPT_TEXTURE )
		{
			// then chances are it's a sampler declaration or other FX meta-type
			// and we don't want it
			continue;
		}

		// try to match the semantic against the known ones on the render state
		ConstantTableEntry ent;
		ent.semantic = pState->getAutoConstBySemantic(paramDesc.Semantic);
		ent.name = hashString64(paramDesc.Name);
		ent.handle = (void*)hParam;
		ent.numCols = paramDesc.Columns;
		ent.numRows = paramDesc.Class == D3DXPC_VECTOR ? paramDesc.Elements : paramDesc.Rows;
		ent.maxRows = paramDesc.Elements;

		constants.push_back(ent);
	}

	std::sort(constants.begin(), constants.end(), UDless);
}
//---------------------------------------------------------------------------
bool GfxShaderD3D9::initialize(
	const String& source, 
	String& errs)
{
	m_source = source;
	m_errs.clear();

	return initD3D9(errs);
}
//---------------------------------------------------------------------------
void GfxShaderD3D9::setState(
	GfxRenderState* pState, GfxShaderConstantTable* pTable, size_t numMatrices)
{
	if (!m_pFX)
	{
		String errs;
		if (!initD3D9(errs))
			return;
	}

	// populate the shader constants for this pass; we work down the list of 
	// "registered" constants and for each non-auto constant, we advance the 
	// index for the source constant table until we get a matching name hash;
	// typically, there should be no need to advance to find the right one 
	// because both the registry and source tables are sorted on this name hash
	size_t tableIdx = 0;

	for (size_t i=0; i<m_constants.size(); ++i)
	{
		ConstantTableEntry& ent = m_constants[i];
		D3DXHANDLE hParam = (D3DXHANDLE)ent.handle;

		// try to match the semantic against the known ones on the render state
		AutoConst mat = ent.semantic;
		if (mat != AC_NONE)
		{
			HRESULT hr = S_OK;

			if (mat == AC_MATRIXPALETTE)
			{
				// copy the bone data
				hr = m_pFX->SetVectorArray(hParam, (D3DXVECTOR4*)pState->getConstant(mat), numMatrices * 3);
			}
			else if (mat < AC_AMBIENT)
			{
				// it's a single matrix, set it from the state cache
				if (!ent.maxRows)
					hr = m_pFX->SetMatrix(hParam, (D3DXMATRIX*)pState->getConstant(mat));
				else
					hr = m_pFX->SetMatrixArray(hParam, (D3DXMATRIX*)pState->getConstant(mat), ent.numRows / 4);
				continue;
			}
			else
			{
				if (ent.maxRows)
					hr = m_pFX->SetVectorArray(hParam, (D3DXVECTOR4*)pState->getConstant(mat), ent.numRows);
				else
					hr = m_pFX->SetVector(hParam, (D3DXVECTOR4*)pState->getConstant(mat));
			}

			if (FAILED(hr))
			{
				GfxRendererD3D9* pRend = 
					static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);
				pRend->logD3DError(hr);
			}
		}
		else
		{
			if (pTable)
			{
				// it's a named constant and we need to get it from
				// the source data table
				GfxShaderConstant* pConst = pTable->getConstant(tableIdx++);

				if (pConst)
				{
					// make sure we've got the right one
					while (pConst->getName() != ent.name && tableIdx < pTable->getNumConstants())
						pConst = pTable->getConstant(tableIdx++);

					if (pConst->isMatrix())
					{
						m_pFX->SetMatrix(hParam, (D3DXMATRIX*)pConst->getValue());
					}
					else
					{
						if (ent.maxRows)
							m_pFX->SetVectorArray(hParam, (D3DXVECTOR4*)pConst->getValue(), ent.numRows);
						else
							m_pFX->SetVector(hParam, (D3DXVECTOR4*)pConst->getValue());
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
ID3DXEffect* GfxShaderD3D9::getFX()
{
	if (!m_pFX)
	{
		String errs;
		if (!initD3D9(errs))
			return 0;
	}

	return m_pFX;
}
//---------------------------------------------------------------------------
bool GfxShaderD3D9::setTechnique(ShaderVariant sv)
{
	if (!m_pFX)
	{
		String errs;
		if (!initD3D9(errs))
			return false;
	}

	HRESULT hr = E_FAIL;
	switch (sv)
	{
	case SV_SHADOWCASTER:
		if (m_techniqueShadowCast)
			hr = m_pFX->SetTechnique(m_techniqueShadowCast);
		break;
	case SV_SHADOWRECEIVER:
		if (m_techniqueShadowRecv)
			hr = m_pFX->SetTechnique(m_techniqueShadowRecv);
		break;
	case SV_DEFAULT:
	default:
		if (m_technique)
			hr = m_pFX->SetTechnique(m_technique);
		break;
	}

	if (FAILED(hr))
	{
		//Environment& env = Environment::get();
		//GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		//pRend->logD3DError(hr);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void GfxShaderD3D9::releaseD3DResources()
{
	if (m_pFX)
	{
		m_pFX->Release();
	}

	m_pFX = 0;
	m_technique = 0;
	m_constants.clear();
}
