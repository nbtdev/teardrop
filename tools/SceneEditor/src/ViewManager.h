/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWMANAGER_INCLUDED)
#define VIEWMANAGER_INCLUDED

#include <vector>
#include "FastDelegate.h"

class View;
class Data;
class Tool;

class ViewManager
{
	typedef std::vector<View*> Views;
	Views m_views;
	View* m_pActiveView;
	Tool* m_pActiveTool;

public:
	ViewManager();
	ViewManager(View* pDefault);
	virtual ~ViewManager();

	// events 
	func::delegate1<View*> ActiveViewChanged;

	void update(float deltaT, Data* pData);
	bool handleInput(const Teardrop::InputEvent& event);
	void setActiveTool(Tool* pTool);

	TD_DECLARE_ALLOCATOR();
};

#endif // VIEWMANAGER_INCLUDED