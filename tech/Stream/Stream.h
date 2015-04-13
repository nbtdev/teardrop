/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(TDSTREAM_INCLUDED)
#define TDSTREAM_INCLUDED

#include <cstddef>

namespace Teardrop
{
	class Stream
	{
	public:
		typedef enum
		{
			BEGIN,
			CURRENT,
			END
		} SeekType;

		virtual ~Stream();

		//! read data from this stream
		virtual int read(void* pData, size_t len, bool async=false) = 0;
		//! write data to this stream
		virtual int write(const void* pData, size_t len, bool async=false) = 0;
		//! return stream length
		virtual size_t length() = 0;
		//! return stream position
		virtual size_t getPosition() = 0;
		//! seek to another part of this stream
		virtual bool seek(int offset, SeekType seekType = CURRENT, bool async=false) = 0;
		//! check to see if we are at the end of the stream
		virtual bool isEnd() = 0;

		Stream& operator<<(const char*);
		Stream& operator<<(int v);
		Stream& operator<<(unsigned int v);
		Stream& operator<<(bool v);

		Stream& operator>>(int& v);
		Stream& operator>>(unsigned int& v);
		Stream& operator>>(bool& v);

		static const char* ENDL;
		static const char* TAB;
	};

}

#endif // TDSTREAM_INCLUDED
