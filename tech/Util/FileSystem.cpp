/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "FileSystem.h"
#include "_String.h"

using namespace Teardrop;

void FileSystem::directoryName(String& dirName, const String& pathname)
{
	dirName.clear();
	size_t pos = pathname.findLast('\\');
	
	if (pos == String::INVALID)
		pos = pathname.findLast('/');

	if (pos != String::INVALID) {
		dirName = pathname.substr(0, pos);
	}
}
