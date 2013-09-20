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
		// a Thumbnail is by definition a PNG image

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
			bool isValid() const;

			void resize(int w, int h, int nBytes);
			int setData(void* data, int nBytes);

		protected:
			char* mData;
			int mLen;
			int mWidth; 
			int mHeight;
		};
	}
}

#endif // THUMBNAIL_INCLUDED
