/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIFLASHRESOURCE_INCLUDED)
#define UIFLASHRESOURCE_INCLUDED

#include "Resource/Resource.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	struct FourCC;

	namespace UI
	{
		/*
		*/

		class FlashResource : public Resource
		{
		public:
			const static FourCC& RESOURCE_TYPE;

			FlashResource();
			~FlashResource();

			//! Resource implementation
			bool destroy();
			bool release();

			TD_DECLARE_ALLOCATOR();

		private:
			FlashResource(const FlashResource&); // not implemented
			FlashResource& operator=(const FlashResource&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHRESOURCE_INCLUDED
