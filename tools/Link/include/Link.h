/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(LINK_INCLUDED)
#define LINK_INCLUDED

#include <set>
#include <string>

typedef std::set<std::string> StringSet;

struct LinkParams
{
	LinkParams()
	{
		bVerbose = false;
		cmdFileName = 0;
		libName = 0;
	}

	bool bVerbose;
	const char* cmdFileName;
	const char* libName;
	StringSet options;
};

#endif // LINK_INCLUDED
