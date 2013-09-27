/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ATTRIBUTEMAPASSET_INCLUDED)
#define ATTRIBUTEMAPASSET_INCLUDED

#include "Asset/TextureAsset.h"

namespace Teardrop
{
	class AttributeMapAsset : public TextureAsset
	{
	public:
		TD_CLASS(AttributeMapAsset, TextureAsset);

		AttributeMapAsset();
		virtual ~AttributeMapAsset();

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // ATTRIBUTEMAPASSET_INCLUDED
