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

#include "stdafx.h"
#include "Stream.h"
#include "Util/_String.h"

using namespace Teardrop::Integration;

Raknet::Stream::Stream(unsigned char* _data, const unsigned int lengthInBytes, bool _copyData)
: mBS(_data, lengthInBytes, _copyData)
{

}

Raknet::Stream::Stream()
{

}

void Raknet::Stream::write(bool c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(char c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(short c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(int c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(long long c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(long c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(unsigned char c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(unsigned short c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(unsigned int c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(unsigned long long c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(float c)
{
	mBS.Write(c);
}

void Raknet::Stream::write(Teardrop::String& c)
{
	RakNet::RakString s((const char*)c);
	mBS.Write(s);
}

void Raknet::Stream::write(char* output, size_t nBytes)
{
	mBS.Write(output, nBytes);
}

void Raknet::Stream::read(bool& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(char& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(short& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(int& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(long long& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(long& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(unsigned char& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(unsigned short& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(unsigned int& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(unsigned long long& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(float& c)
{
	mBS.Read(c);
}

void Raknet::Stream::read(Teardrop::String& c)
{
	RakNet::RakString s;
	mBS.Read(s);
	c = s;
}

void Raknet::Stream::read(char* input, size_t nBytes)
{
	mBS.Read(input, nBytes);
}
