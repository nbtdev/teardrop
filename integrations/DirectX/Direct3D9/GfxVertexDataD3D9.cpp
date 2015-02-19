/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxVertexDataD3D9.h"
#include "GfxRenderer.h"
#include "GfxRendererD3D9.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "GfxCommon.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxVertexDataD3D9::GfxVertexDataD3D9()
: m_pD3D9VertexBuffer(0)
{
}
//---------------------------------------------------------------------------
GfxVertexDataD3D9::GfxVertexDataD3D9(int i)
: GfxVertexData(i), m_pD3D9VertexBuffer(0)
{
}
//---------------------------------------------------------------------------
GfxVertexDataD3D9::~GfxVertexDataD3D9()
{
	assert(!m_pD3D9VertexBuffer);
}
//---------------------------------------------------------------------------
bool GfxVertexDataD3D9::initD3D9()
{
	Environment& env = Environment::get();

	// create D3D9 vertex buffer in the default pool
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();

	size_t bufSize = m_vertexSize * m_vertexCount;
	if (!bufSize)
		return false;

	IDirect3DVertexBuffer9* pBuf;

	DWORD f = 0;
	if (m_flags & DYNAMIC)
		f |= D3DUSAGE_DYNAMIC;
	if (m_flags & STATIC)
		f |= D3DUSAGE_WRITEONLY;
	if (m_flags & WRITE_ONLY)
		f |= D3DUSAGE_WRITEONLY;

	HRESULT hr = pDevice->CreateVertexBuffer(
		(UINT)bufSize,
		f,
		0, 
		D3DPOOL_DEFAULT,
		&pBuf,
		0);

	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not create vertex buffer...");
		pRend->logD3DError(hr);
		return false;
	}

	m_pD3D9VertexBuffer = pBuf;

	// copy our data into the D3D9 buffer
	void* ppbData;
	hr = m_pD3D9VertexBuffer->Lock(
		0, 
		(UINT)bufSize, 
		&ppbData, 
		0);

	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not lock vertex buffer...");
		pRend->logD3DError(hr);
		return false;
	}

	memcpy(ppbData, m_pVertexBuffer, bufSize);
	m_pD3D9VertexBuffer->Unlock();

	pRend->trackVertexData(this, true);
	return true;
}
//---------------------------------------------------------------------------
bool GfxVertexDataD3D9::initialize(
	CreationFlags flags, 
	unsigned int vertSize,
	unsigned int vertCount, 
	void* pData)
{
	if (!GfxVertexData::initialize(
		flags, 
		vertSize,
		vertCount,
		pData))
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
bool GfxVertexDataD3D9::release()
{
	releaseD3DResources();
	GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(
		Environment::get().pRenderer);
	pRend->trackVertexData(this, false);
	return true;
}
//---------------------------------------------------------------------------
void GfxVertexDataD3D9::releaseD3DResources()
{
	if (m_pD3D9VertexBuffer != 0)
	{
		m_pD3D9VertexBuffer->Release();
	}

	m_pD3D9VertexBuffer = 0;
}
//---------------------------------------------------------------------------
bool GfxVertexDataD3D9::destroy()
{
	return release() && GfxVertexData::destroy();
}
//---------------------------------------------------------------------------
void* GfxVertexDataD3D9::lock(bool bDiscardExisting)
{
	if (m_pD3D9VertexBuffer)
	{
		// D3D9 buffer lock
		void *pBuffer;
		HRESULT hr = m_pD3D9VertexBuffer->Lock(
			0, 
			(UINT)m_vertexCount * m_vertexSize,
			&pBuffer,
			bDiscardExisting ? D3DLOCK_DISCARD : 0);

		if (SUCCEEDED(hr))
		{
			return pBuffer;
		}

		Environment& env = Environment::get();
		GfxRendererD3D9* pRend = static_cast<GfxRendererD3D9*>(env.pRenderer);
		pRend->logD3DError(hr);
	}

	return 0;
}
//---------------------------------------------------------------------------
void GfxVertexDataD3D9::unlock()
{
	if (m_pD3D9VertexBuffer)
	{
		m_pD3D9VertexBuffer->Unlock();
	}
}
//---------------------------------------------------------------------------
bool GfxVertexDataD3D9::delayInit()
{
	// for memory-image objects, the D3D9 part of this will not have been
	// initialized, so we want to init it now, on first use (makes for faster
	// loads anyway)
	return initD3D9();
}
//---------------------------------------------------------------------------
IDirect3DVertexBuffer9* GfxVertexDataD3D9::getD3D9VertexBuffer()
{
	if (!m_pD3D9VertexBuffer)
	{
		// delay-init the buffer 
		delayInit();
	}

	// if the data changed, then we need to upload the new data first
	if (m_bDataChanged)
	{
		void* pData;
		size_t sz = m_vertexCount * m_vertexSize;
		HRESULT hr = m_pD3D9VertexBuffer->Lock(
			0, 
			sz, 
			&pData, 
			D3DLOCK_DISCARD);

		if (SUCCEEDED(hr))
		{
			memcpy(pData, m_pVertexBuffer, sz);
			m_pD3D9VertexBuffer->Unlock();
		}

		m_bDataChanged = false;
	}

	return m_pD3D9VertexBuffer;
}
//---------------------------------------------------------------------------
bool GfxVertexDataD3D9::serialize(ResourceSerializer& serializer)
{
	// relocate the vtabl pointer
	serializer.relocate(GfxVertexDataD3D9::getSerialClass()->getId());

	// write us out
	serializer.relocate(this, sizeof(GfxVertexDataD3D9));

	// and then deal with the vertex buffer -- in situations that are 
	// serializable, the pointer *should* be non-null; check anyway
	if (m_pVertexBuffer != 0)
	{
		// relocate the serial pointer
		serializer.relocate(m_pVertexBuffer);

		// and then relocate the actual data
		serializer.relocate(m_pVertexBuffer, m_vertexCount * m_vertexSize);
	}

	return true;
}
