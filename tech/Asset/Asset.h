/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

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
        uint64_t length();
		// we don't own this data blob
		void setData(void* data, int len);
		// we own this data blob
		void* createData(int len);
		// write asset data to strm
        virtual uint64_t serialize(Stream& strm);
		// read asset data from strm
        virtual uint64_t deserialize(Stream& strm);

		TD_DECLARE_ALLOCATOR();

	protected:
		// this always points to real data
		void* mData;
		// this contains the length of the data blob in bytes
        uint64_t mLength;
		// if the data is on the heap and owned by us, this and 
		// mData will be the same and we should delete this in 
		// dtor; otherwise, this is zero
		unsigned char* mDynamicData;
	};
} // namespace Teardrop

#endif // ASSET_INCLUDED
