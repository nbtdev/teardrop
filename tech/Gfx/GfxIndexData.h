/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXINDEXDATA_INCLUDED)
#define TDGFXINDEXDATA_INCLUDED

namespace Teardrop
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

#endif // TDGFXINDEXDATA_INCLUDED
