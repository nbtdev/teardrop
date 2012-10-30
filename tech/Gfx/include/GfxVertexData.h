/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#if !defined(COSGFXVERTEXDATA_INCLUDED)
#define COSGFXVERTEXDATA_INCLUDED

#include "GfxCommon.h"

namespace CoS
{
	class GfxVertexFormat;
	class ResourceSerializer;

	class GfxVertexData
	{
		DECLARE_SERIALIZABLE(GfxVertexData);
		DECLARE_SERIALIZABLE_VTABLE

	public:
		//! normal c'tor (cannot fail)
		GfxVertexData();
		//! placement c'tor (cannot fail)
		GfxVertexData(int);
		//! d'tor (cannot fail)
		virtual ~GfxVertexData();

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

		/**
			Serialization
		*/
		//! package for storage
		virtual bool serialize(ResourceSerializer& serializer);

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
		SerialPointer<void> m_pVertexBuffer;

	private:
		// NOT IMPLEMENTED
		GfxVertexData(const GfxVertexData& other);
		GfxVertexData& operator=(const GfxVertexData& other);
	};
}

#endif // COSGFXVERTEXDATA_INCLUDED
