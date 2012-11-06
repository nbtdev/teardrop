/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWLISTENER_INCLUDED)
#define VIEWLISTENER_INCLUDED

namespace Teardrop
{
	class ZoneObject;
	union InputEvent;
}

class ViewListener
{
public:
	virtual bool DoFileOpen(wxString filename) = 0;
	virtual bool DoFileSave(wxString filename) = 0;
	virtual void OnObjectSelected(Teardrop::Reflection::Object* pSelected) = 0;
	virtual void OnInputEvent(const Teardrop::InputEvent& evt) = 0;
};

#endif // VIEWLISTENER_INCLUDED
