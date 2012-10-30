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
#include "GfxVertexFormat.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "Util/include/Environment.h"
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
static D3DDECLTYPE s_typeLUT[] = 
{
	/*
		NOTSET = 0,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		HALF2,
		HALF4,
		BYTE4,
		VET_COLOR
	*/
    D3DDECLTYPE_UNUSED,
    D3DDECLTYPE_FLOAT1,
    D3DDECLTYPE_FLOAT2,
    D3DDECLTYPE_FLOAT3,
    D3DDECLTYPE_FLOAT4,
    D3DDECLTYPE_FLOAT16_2,
    D3DDECLTYPE_FLOAT16_4,
    D3DDECLTYPE_UBYTE4,
    D3DDECLTYPE_D3DCOLOR,
    //D3DDECLTYPE_SHORT2 = 6,
    //D3DDECLTYPE_SHORT4 = 7,
    //D3DDECLTYPE_UBYTE4N = 8,
    //D3DDECLTYPE_SHORT2N = 9,
    //D3DDECLTYPE_SHORT4N = 10,
    //D3DDECLTYPE_USHORT2N = 11,
    //D3DDECLTYPE_USHORT4N = 12,
    //D3DDECLTYPE_UDEC3 = 13,
    //D3DDECLTYPE_DEC3N = 14,
};
//---------------------------------------------------------------------------
static D3DDECLUSAGE s_usageLUT[] = 
{
	/*
	UNUSED			= 0,
	POSITION		= 1,
	NORMAL			= 2,
	COLOR			= 4,
	TEXCOORD		= 8,
	BINORMAL		= 16,
	TANGENT			= 32,
	BLENDWEIGHT		= 64,
	BLENDINDEX		= 128,
	*/
    D3DDECLUSAGE_FOG,
    D3DDECLUSAGE_POSITION,
    D3DDECLUSAGE_NORMAL,
    D3DDECLUSAGE_COLOR,
    D3DDECLUSAGE_TEXCOORD,
    D3DDECLUSAGE_BINORMAL,
    D3DDECLUSAGE_TANGENT,
    D3DDECLUSAGE_BLENDWEIGHT,
	D3DDECLUSAGE_BLENDINDICES,
};
//---------------------------------------------------------------------------
bool GfxVertexFormat::fix(Environment& env)
{
	// sort out the vertex format into D3D9 vertex declaration
	D3DVERTEXELEMENT9 elems[GfxVertexFormat::MAX_ELEMENT_COUNT+1];

	size_t i = 0;
	for (; i<m_elementCount; ++i)
	{
		const GfxVertexFormat::Element& e = getElementAt(i);
		D3DVERTEXELEMENT9& elem = elems[i];

		elem.Method = D3DDECLMETHOD_DEFAULT;
		elem.Type = (BYTE)s_typeLUT[e.type];
		elem.Usage = (BYTE)s_usageLUT[e.semantic];
		elem.UsageIndex = e.getUsage();
		elem.Offset = e.offset;
		elem.Stream = (WORD)e.getSource();
	}

	// mark the end of the decl list
	elems[i].Stream = 0xFF;
	elems[i].Offset = 0;
	elems[i].Type = D3DDECLTYPE_UNUSED;
	elems[i].Method = 0;
	elems[i].Usage = 0;
	elems[i].UsageIndex = 0;

	// create the vertex declaration
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DVertexDeclaration9* pDecl;
	if (FAILED(pRend->getDevice()->CreateVertexDeclaration(
		elems,
		&pDecl)))
	{
		return false;
	}

	m_pImpl = (void*)pDecl;

	return true;
}
//---------------------------------------------------------------------------
bool GfxVertexFormat::release()
{
	if (m_pImpl != 0)
	{
		IDirect3DVertexDeclaration9* pDecl = 
			(IDirect3DVertexDeclaration9*)(void*)m_pImpl;
		if (FAILED(pDecl->Release()))
		{
			return false;
		}
	}

	m_pImpl = 0;

	return true;
}