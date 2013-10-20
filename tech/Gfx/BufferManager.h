/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BUFFERMANAGER_INCLUDED)
#define BUFFERMANAGER_INCLUDED

#include "Util/Singleton.h"

namespace Teardrop
{
	namespace Gfx
	{
		class VertexBuffer;
		class IndexBuffer;

		class BufferManager : public Singleton<BufferManager>
		{
		public:
			BufferManager();
			~BufferManager();

			virtual IndexBuffer* createIndexBuffer() = 0;
			virtual VertexBuffer* createVertexBuffer() = 0;
			virtual void release(IndexBuffer* buffer) = 0;
			virtual void release(VertexBuffer* buffer) = 0;

			TD_DECLARE_ALLOCATOR();

		protected:
		};
	}
}

#endif // BUFFERMANAGER_INCLUDED
