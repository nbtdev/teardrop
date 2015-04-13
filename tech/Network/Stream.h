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

#if !defined(NET_STREAM_INCLUDED)
#define NET_STREAM_INCLUDED

namespace Teardrop
{
	class String;

	namespace Net
	{
		class Stream
		{
		public:
			virtual void write(bool c) = 0;
			virtual void write(char c) = 0;
			virtual void write(short c) = 0;
			virtual void write(int c) = 0;
			virtual void write(long c) = 0;
			virtual void write(long long c) = 0;
			virtual void write(unsigned char c) = 0;
			virtual void write(unsigned short c) = 0;
			virtual void write(unsigned int c) = 0;
			virtual void write(unsigned long long c) = 0;
			virtual void write(float c) = 0;
			virtual void write(String& c) = 0;
			virtual void write(char* output, size_t nBytes) = 0;

			virtual void read(bool& c) = 0;
			virtual void read(char& c) = 0;
			virtual void read(short& c) = 0;
			virtual void read(int& c) = 0;
			virtual void read(long& c) = 0;
			virtual void read(long long& c) = 0;
			virtual void read(unsigned char& c) = 0;
			virtual void read(unsigned short& c) = 0;
			virtual void read(unsigned int& c) = 0;
			virtual void read(unsigned long long& c) = 0;
			virtual void read(float& c) = 0;
			virtual void read(String& c) = 0;
			virtual void read(char* input, size_t nBytes) = 0;
		};
	}
}

#endif // NET_STREAM_INCLUDED
