/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Serialization/ResourceDeserializer.h"
#include "Stream/FileStream.h"
#include "Memory/MemoryRegion.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
TD_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
//---------------------------------------------------------------------------
void usage()
{
}
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		usage();
		return -1;
	}

	FileStream in, out;
	if (!in.open(argv[1], READ|BINARY))
	{
		return -2;
	}

	void* pMem = new char[in.length()];
	in.read(pMem, in.length());
	in.close();

	if (!out.open(argv[2], WRITE|TRUNCATE|TEXT))
	{
		return -2;
	}

	ResourceDeserializer d;
	d.dumpDependencies(pMem, out);

	out.close();
	return 0;
}

