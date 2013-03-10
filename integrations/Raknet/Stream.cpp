/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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