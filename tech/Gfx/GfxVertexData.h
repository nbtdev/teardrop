/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXVERTEXDATA_INCLUDED)
#define TDGFXVERTEXDATA_INCLUDED

#include "GfxCommon.h"
#include <vector>

namespace Teardrop
{
	class GfxVertexFormat;

	class GfxVertexData
	{
	public:
		//! normal c'tor (cannot fail)
		GfxVertexData();
		//! placement c'tor (cannot fail)
		GfxVertexData(int);
		//! d'tor (cannot fail)
		virtual ~GfxVertexData();

        GfxVertexData(const GfxVertexData& other) = delete;
        GfxVertexData& operator=(const GfxVertexData& other) = delete;

		typedef enum
		{
			NONE				= 0x00,
			STATIC				= 0x01,
			WRITE_ONLY			= 0x02,
			DYNAMIC				= 0x04,
		} CreationFlags;

		//! initialize (anything that can fail), with specified format and buffer creation flags
		virtual bool initialize(
			CreationFlags flags=NONE, 
			unsigned int vertSize=0, 
			unsigned int vertCount=0, 
			void* pData=0);
		//! destroy the vertex data
		virtual bool destroy();
		//! release the vertex data's underlying structures
		virtual bool release();
		//! lock the vertex buffer for editing
		virtual void* lock(bool bDiscardExisting=true);
		//! done editing, unlock the vertex buffer
		virtual void unlock();
		//! the user updated the current generic buffer, so mark the data
		//! for upload to the GPU 
		void updateData();
		
		/** accessors
		*/
		unsigned int getVertexCount() const;
		unsigned int getVertexSize() const;
		void* getBuffer();

		/** mutators
		*/
		//! set the vertex data to use a completely different buffer
		bool setVertexData(
			CreationFlags flags, 
			unsigned int vertSize, 
			unsigned int vertCount, 
			void* pData=0);

		DECLARE_GFX_ALLOCATOR();

	protected:
		//! size of single vertex element
		unsigned int m_vertexSize;
		//! number of vertices in the buffer
		unsigned int m_vertexCount;
		//! (was type of primitive defined in the vertex data)
		unsigned char pad[1];
		//! whether or not we need to clean up the vertex buffer pointer
		bool m_bDeleteVB;
		//! unused
		bool m_bDataChanged;
		//! unused
		unsigned char m_flags;
		//! pointer to the actual vertex data
        void* mVertexBuffer;

	private:
	};
}

#endif // TDGFXVERTEXDATA_INCLUDED
