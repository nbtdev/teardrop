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

#if !defined(RAKNET_STREAM_INCLUDED)
#define RAKNET_STREAM_INCLUDED

#include "Network/Stream.h"
#include "BitStream.h"

namespace Teardrop
{
	namespace Integration
	{
		namespace Raknet
		{
			class Stream : public Net::Stream
			{
			public:
				RakNet::BitStream mBS;

				Stream(unsigned char* _data, const unsigned int lengthInBytes, bool _copyData);
				Stream();

				void write(bool c);
				void write(char c);
				void write(short c);
				void write(int c);
				void write(long c);
				void write(long long c);
				void write(unsigned char c);
				void write(unsigned short c);
				void write(unsigned int c);
				void write(unsigned long long c);
				void write(float c);
				void write(String& c);
				void write(char* output, size_t nBytes);

				void read(bool& c);
				void read(char& c);
				void read(short& c);
				void read(int& c);
				void read(long& c);
				void read(long long& c);
				void read(unsigned char& c);
				void read(unsigned short& c);
				void read(unsigned int& c);
				void read(unsigned long long& c);
				void read(float& c);
				void read(String& c);
				void read(char* input, size_t nBytes);
			};
		}
	}
}

#endif // RAKNET_STREAM_INCLUDED
