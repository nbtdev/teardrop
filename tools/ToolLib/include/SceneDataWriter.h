/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENEDATAWRITER_INCLUDED)
#define SCENEDATAWRITER_INCLUDED

#include <string>
#include <list>

struct SceneEntry
{
	std::string name;
	float trans[4];
	float scale[4];
	float axis[4];
	float angle;
	float minCorner[4];
	float maxCorner[4];
};
typedef std::list<SceneEntry> SceneEntries;

class SceneDataWriter
{
public:
	SceneDataWriter();
	~SceneDataWriter();

	bool write(const SceneEntries& entries, const char* dataFileName);
};

#endif // SCENEDATAWRITER_INCLUDED
