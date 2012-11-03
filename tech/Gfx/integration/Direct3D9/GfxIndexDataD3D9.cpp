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
#include "GfxIndexDataD3D9.h"
#include "GfxCommon.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Serialization/ResourceSerializer.h"
#include <assert.h>
#include <new.h>

using namespace CoS;
//---------------------------------------------------------------------------
DEFINE_SERIALIZABLE(GfxIndexDataD3D9);
//---------------------------------------------------------------------------
GfxIndexDataD3D9::GfxIndexDataD3D9() 
: m_pD3D9IndexBuffer(0)
{
}
//---------------------------------------------------------------------------
GfxIndexDataD3D9::GfxIndexDataD3D9(int i) 
: GfxIndexData(i), m_pD3D9IndexBuffer(0)
{
}
//---------------------------------------------------------------------------
GfxIndexDataD3D9::~GfxIndexDataD3D9()
{
	assert(!m_pD3D9IndexBuffer);
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::initD3D9()
{
	Environment& env = Environment::get();

	// create D3D9 index buffer
	IDirect3DIndexBuffer9* pBuf;
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();
	HRESULT hr = pDevice->CreateIndexBuffer(
		(UINT)m_indexCount * m_indexSize,
		D3DUSAGE_WRITEONLY,
		m_indexSize == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&pBuf,
		0);

	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not create index buffer...");
		pRend->logD3DError(hr);
		return false;
	}

	m_pD3D9IndexBuffer = pBuf;

	// copy the index data to our new D3D9 buffer
	void* ppbData;
	hr = m_pD3D9IndexBuffer->Lock(
		0, 
		(UINT)m_indexSize * m_indexCount,
		&ppbData,
		0);

	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not lock index buffer...");
		pRend->logD3DError(hr);
		return false;
	}

	memcpy(ppbData, m_pIndexBuffer, (UINT)m_indexSize * m_indexCount);
	m_pD3D9IndexBuffer->Unlock();

	pRend->trackIndexData(this, true);
	return true;
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::initialize(
	unsigned int indexSize, // in bytes
	unsigned int indexCount, 
	void* pData)
{
	if (!GfxIndexData::initialize(indexSize, indexCount, pData))
	{
		return false;
	}

	Environment& env = Environment::get();
	if (env.isOffline)
	{
		return true;
	}

	return initD3D9();
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::release()
{
	releaseD3DResources();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(
		Environment::get().pRenderer);
	pRend->trackIndexData(this, false);
	return true;
}
//---------------------------------------------------------------------------
void GfxIndexDataD3D9::releaseD3DResources()
{
	if (m_pD3D9IndexBuffer)
	{
		m_pD3D9IndexBuffer->Release();
	}

	m_pD3D9IndexBuffer = 0;
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::destroy()
{
	return release() && GfxIndexData::destroy();
}
//---------------------------------------------------------------------------
void* GfxIndexDataD3D9::lock(bool /*bDiscardExisting*/)
{
	if (m_pD3D9IndexBuffer)
	{
		// D3D9 buffer lock
		size_t size = m_indexCount * m_indexSize;

		// TODO: flags and buffer offset
		void* pData;
		HRESULT hr = m_pD3D9IndexBuffer->Lock(0, (UINT)size, &pData, 0);
		if (SUCCEEDED(hr))
		{
			return pData;
		}

		Environment& env = Environment::get();
		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		pRend->logD3DError(hr);
	}

	return 0;

}
//---------------------------------------------------------------------------
void GfxIndexDataD3D9::unlock()
{
	if (m_pD3D9IndexBuffer)
	{
		// D3D9 buffer unlock
		HRESULT hr = m_pD3D9IndexBuffer->Unlock();
		if (FAILED(hr))
		{
			Environment& env = Environment::get();
			GfxRendererD3D9* pRend = 
				static_cast<GfxRendererD3D9*>(env.pRenderer);
			pRend->logD3DError(hr);
		}
	}
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::delayInit()
{
	// for memory-image objects, the D3D9 part of this will not have been
	// initialized, so we want to init it now, on first use (makes for faster
	// loads anyway)
	return initD3D9();
}
//---------------------------------------------------------------------------
bool GfxIndexDataD3D9::serialize(ResourceSerializer& serializer)
{
	// relocate the vtabl pointer
	serializer.relocate(GfxIndexDataD3D9::getSerialClass()->getId());

	// write us out
	serializer.relocate(this, sizeof(GfxIndexDataD3D9));

	// and then deal with the vertex buffer -- in situations that are 
	// serializable, the pointer *should* be non-null; check anyway
	if (m_pIndexBuffer != 0)
	{
		serializer.relocate(m_pIndexBuffer);

		// and then relocate the actual data
		serializer.relocate(m_pIndexBuffer, m_indexCount * m_indexSize);
	}

	return true;
}
