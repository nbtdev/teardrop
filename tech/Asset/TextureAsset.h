/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTUREASSET_INCLUDED)
#define TEXTUREASSET_INCLUDED

#include "Asset/Asset.h"

namespace Teardrop
{
	class TextureAsset : public Asset
	{
	public:
		TD_CLASS(TextureAsset, Asset);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Width, "Texture width in pixels", int, 0, "ReadOnly");
		TD_PROPERTY(Height, "Texture height in pixels", int, 0, "ReadOnly");

		TextureAsset();
		virtual ~TextureAsset();

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // TEXTUREASSET_INCLUDED
