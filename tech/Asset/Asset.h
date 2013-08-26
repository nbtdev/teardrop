/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ASSET_INCLUDED)
#define ASSET_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Asset : public Reflection::Object
	{
	public:
		TD_CLASS(Asset, Object);

		Asset();
		virtual ~Asset();

		TD_DECLARE_ALLOCATOR();

	protected:
	};
} // namespace Teardrop

#endif // ASSET_INCLUDED
