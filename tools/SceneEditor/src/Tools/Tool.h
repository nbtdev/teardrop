/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TOOL_INCLUDED)
#define TOOL_INCLUDED

#include "Base/AttributeContainer.h"
#include <list>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	class GfxMeshInstance;
}

class Data;
class MessageBus;
class View;

/*
	class Tool

	Base class for editor tools. 

	An editor tool is something that the user manipulates to effect
	a change on the scene configuration. For example, a translation 
	tool would move the selected object (if any, and if it can be 
	moved) in response to user mouse movements. Likewise, a terrain
	sculpting "raise" tool might raise the affected terrain by 
	whatever configured amount is currently set. Tools generally are
	used by derived Mode classes.
*/

class Tool
	: public AttributeContainer
{
public:
	Tool();
	virtual ~Tool();

	virtual bool update(float deltaT, Data* pData) = 0;
	virtual bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData) = 0;
	virtual size_t getNumRenderables() = 0;
	virtual bool getRenderable(/*out*/Teardrop::GfxMeshInstance& inst, /*in*/size_t index) = 0;
	virtual void activate();
	virtual void deactivate();
	
	void setActiveView(View*);
	void setSelectedObject(Teardrop::Reflection::Object* pObj);

protected:
	View* m_pView;

	typedef std::list<Teardrop::Reflection::Object*> SelectedObjects;
	SelectedObjects m_selectedObjects;

	virtual void _onViewChanged();
	virtual void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);
};

#endif // TOOL_INCLUDED
