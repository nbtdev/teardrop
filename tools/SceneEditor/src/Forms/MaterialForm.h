/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIALFORM_INCLUDED)
#define MATERIALFORM_INCLUDED

#include "MaterialFormFrame.h"
#include "FastDelegate.h"

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	class GfxShader;
	union InputEvent;
}

class wxScintilla;

class MaterialForm
	:	 public MaterialFormFrame
{
	wxScintilla* m_pSourceEdit;
	Teardrop::GfxShader* m_pShader;
	Teardrop::Reflection::Object* m_pObject;

public:
	// events

	// ctor/dtor
	MaterialForm();
	~MaterialForm();

	void setObject(Teardrop::Reflection::Object* pObj);
	void onObjectSelectionChanged(void* sender, Teardrop::Reflection::Object* pObj);

	// form overrides
	void OnClose( wxCloseEvent& event );
	void OnFormResize( wxSizeEvent& evt );
	void OnApply(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);

private:
	void onInputEvent(const Teardrop::InputEvent& evt, bool& bHandled);
};

#endif // MATERIALFORM_INCLUDED
