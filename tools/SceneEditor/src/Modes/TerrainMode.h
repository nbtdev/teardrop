/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TERRAINMODE_INCLUDED)
#define TERRAINMODE_INCLUDED

#include "Mode.h"
#include "Memory/Memory.h"

/*
	class TerrainMode

	Terrain sculpting mode.
*/

class TerrainMode
	: public Mode
{
	Tool* m_pCurrentTool;

public:
	TerrainMode();
	virtual ~TerrainMode();

	// Mode implementation
	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	Tool* getActiveTool();
	void setActiveTool(Tool* pTool, size_t toolIdx);
	const Tool** getTools();
	size_t getNumTools();

	TD_DECLARE_ALLOCATOR();

protected:
	void _activeViewChanged();
};

#endif // TERRAINMODE_INCLUDED
