/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#if !defined(MATERIALFORM_INCLUDED)
#define MATERIALFORM_INCLUDED

#include "MaterialFormFrame.h"
#include "FastDelegate.h"

namespace CoS
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
	CoS::GfxShader* m_pShader;
	CoS::Reflection::Object* m_pObject;

public:
	// events

	// ctor/dtor
	MaterialForm();
	~MaterialForm();

	void setObject(CoS::Reflection::Object* pObj);
	void onObjectSelectionChanged(void* sender, CoS::Reflection::Object* pObj);

	// form overrides
	void OnClose( wxCloseEvent& event );
	void OnFormResize( wxSizeEvent& evt );
	void OnApply(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);

private:
	void onInputEvent(const CoS::InputEvent& evt, bool& bHandled);
};

#endif // MATERIALFORM_INCLUDED
