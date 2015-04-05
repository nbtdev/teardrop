/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Texture2DD3D11.h"
#include "Asset/TextureAsset.h"
#include "Gfx/Exception.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

static DXGI_FORMAT sLut[] = {
	DXGI_FORMAT_R8_UINT,            // TEXFMT_R
	DXGI_FORMAT_R8G8_UINT,          // TEXFMT_RG
	DXGI_FORMAT_UNKNOWN,            // TEXFMT_RGB
	DXGI_FORMAT_R8G8B8A8_UINT,      // TEXFMT_ARGB
	DXGI_FORMAT_R32_FLOAT,          // TEXFMT_R_F32
	DXGI_FORMAT_R32G32_FLOAT,       // TEXFMT_RG_F32
	DXGI_FORMAT_UNKNOWN,            // TEXFMT_RGB_F32
	DXGI_FORMAT_R32G32B32A32_FLOAT, // TEXFMT_RGBA_F32
	DXGI_FORMAT_BC1_UNORM,          // TEXFMT_BC1
	DXGI_FORMAT_BC2_UNORM,          // TEXFMT_BC2
	DXGI_FORMAT_BC3_UNORM,          // TEXFMT_BC3
	DXGI_FORMAT_UNKNOWN,            // TEXFMT_PVRTC
};

Texture2D::Texture2D(ComPtr<ID3D11Device> aDevice, TextureAsset* aAsset, Usage aUsage)
	: Gfx::Texture2D(aAsset)
	, mDevice(mDevice)
{
	assert(mDevice);
	assert(mAsset);

	// create texture object
	D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;

	if (aUsage == USAGE_RENDERTARGET)
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	else
		if (aUsage == USAGE_DYNAMIC)
			usage = D3D11_USAGE_DYNAMIC;

	bool genMips = mAsset->getGenerateMipmaps();

	// because "0" actually means "generate them all", when what we really 
	// meant was "generate nothing beyond the first one", which would be "1"
	UINT actualMips = genMips ? 0 : 1;

	// use D3DX to load DDS textures, another way otherwise
	TextureAsset::Format fmt = mAsset->getFormat();
	mD3D11Format = sLut[aAsset->getFormat()];

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aAsset->getWidth();
	desc.Height = aAsset->getHeight();
	desc.MipLevels = actualMips;
	desc.ArraySize = 1;
	desc.Format = mD3D11Format;
	desc.SampleDesc.Count = 1;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = mAsset->data();

	HRESULT hr = mDevice->CreateTexture2D(
		&desc,
		&data,
		&mTexObject
		);

	if (FAILED(hr)) {
		throw Exception("Could not create 2D texture object in Texture2D");
	}
}

Texture2D::~Texture2D()
{
}

ComPtr<ID3D11Texture2D> Texture2D::textureObject()
{
	return mTexObject;
}

DXGI_FORMAT Texture2D::textureFormat()
{
	return mD3D11Format;
}

} // Direct3D9
} // Gfx
} // Teardrop
