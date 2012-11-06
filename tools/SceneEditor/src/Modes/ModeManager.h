/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MODEMANAGER_INCLUDED)
#define MODEMANAGER_INCLUDED

#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	union InputEvent;
}

class Mode;
class Tool;
class Data;
class View;
class Toolbar;

class ModeManager
{
public:
	ModeManager();
	~ModeManager();

	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	Tool* getActiveTool();
	void setActiveView(View*);

	TD_DECLARE_ALLOCATOR();

private:
	void onModeChanged(size_t);
	void onToolChanged(Tool*, size_t);

	enum
	{
		MODE_OBJECT,
		MODE_TERRAIN,
	};

	typedef std::vector<Mode*> Modes;
	Modes m_modes;
	size_t m_currentMode;
	View* m_pActiveView;
};

#endif // MODEMANAGER_INCLUDED
