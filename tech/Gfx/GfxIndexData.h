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

#if !defined(COSGFXINDEXDATA_INCLUDED)
#define COSGFXINDEXDATA_INCLUDED

namespace CoS
{
	class ResourceSerializer;

	class GfxIndexData
	{
		DECLARE_SERIALIZABLE_VTABLE
		DECLARE_SERIALIZABLE(GfxIndexData);

	public:
		//! normal c'tor (cannot fail)
		GfxIndexData();
		//! placement c'tor (cannot fail)
		GfxIndexData(int);
		//! d'tor (cannot fail)
		virtual ~GfxIndexData();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		virtual bool initialize(
			unsigned int  indexSize=0, 
			unsigned int  numIndices=0, 
			void* pData=0);

		//! destroy
		virtual bool destroy();
		//! release 
		virtual bool release();
		//! lock the index buffer for editing
		virtual void* lock(bool bDiscardExisting=true);
		//! done editing, unlock the index buffer
		virtual void unlock();
		
		/** accessors
		*/
		size_t getIndexSize() const;
		size_t getIndexCount() const;
		void* getBuffer();

		/** mutators
		*/
		//! set the index data to use a completely different buffer
		bool setIndexData(
			unsigned int indexSize, 
			unsigned int count, 
			void* pBuffer);

		/**
			Serialization
		*/
		//! package for storage
		virtual bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALIGNED_ALLOCATOR();

	protected:
		//! 2 for 16-bit indices, 4 for 32-bit
		unsigned int m_indexSize;
		//! number of indices in the buffer
		unsigned int m_indexCount;
		//! whether to delete the index buffer pointer on destroy
		unsigned int m_bDeleteIB;
		//! pointer to the actual index data
		SerialPointer<void> m_pIndexBuffer;

	private:
		//! NOT IMPLEMENTED
		GfxIndexData(const GfxIndexData& other);
		GfxIndexData& operator=(const GfxIndexData& other);
	};
}

#endif // COSGFXINDEXDATA_INCLUDED
