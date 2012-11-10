/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
