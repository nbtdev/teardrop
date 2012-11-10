/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
