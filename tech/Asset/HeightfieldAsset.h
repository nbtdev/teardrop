/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HEIGHTFIELDASSET_INCLUDED)
#define HEIGHTFIELDASSET_INCLUDED

#include "Asset/TextureAsset.h"

namespace Teardrop
{
	class HeightfieldAsset : public TextureAsset
	{
	public:
		TD_CLASS(HeightfieldAsset, TextureAsset);

		HeightfieldAsset();
		virtual ~HeightfieldAsset();

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // HEIGHTFIELDASSET_INCLUDED
