/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(THUMBNAIL_INCLUDED)
#define THUMBNAIL_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Tools
	{
		class Thumbnail
		{
		public:
			Thumbnail();
			~Thumbnail();

			void* data();
			const void* data() const;
			int length() const;
			int width() const;
			int height() const;

		protected:
			char* mData;
			int mLen;
			int mWidth; 
			int mHeight;
		};
	}
}

#endif // THUMBNAIL_INCLUDED
