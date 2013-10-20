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
	class Stream;

	class Asset : public Reflection::Object
	{
	public:
		TD_CLASS(Asset, Object);

		Asset();
		virtual ~Asset();

		// return pointer to data blob
		const void* data();
		// return length of data blob in bytes
		int length();
		// we don't own this data blob
		void setData(void* data, int len);
		// we own this data blob
		void* createData(int len);
		// write asset data to strm
		virtual int serialize(Stream& strm);
		// read asset data from strm
		virtual int deserialize(Stream& strm);

		TD_DECLARE_ALLOCATOR();

	protected:
		// this always points to real data
		void* mData;
		// this contains the length of the data blob in bytes
		int mLength;
		// if the data is on the heap and owned by us, this and 
		// mData will be the same and we should delete this in 
		// dtor; otherwise, this is zero
		unsigned char* mDynamicData;
	};
} // namespace Teardrop

#endif // ASSET_INCLUDED
