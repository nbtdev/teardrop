/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENEEDITORAPP_INCLUDED)
#define SCENEEDITORAPP_INCLUDED

class SceneEditorFrame;
class Controller;
class Data;
class UserInterface;

namespace Teardrop
{
	class SceneObject;
	class ZoneData;
}

class SceneEditorApp 
	:	public wxApp
{
	Controller* m_pController;
	SceneEditorFrame* m_pFrame;
	UserInterface* m_pUI;
	Data* m_pData;

public:
	SceneEditorApp();
	virtual ~SceneEditorApp();

	// wxApp overrides
	bool OnInit();
	int OnExit();
	int OnRun();
	void OnUnhandledException();

	bool DoFileOpen(wxString filename);
	bool DoEntityOpen(const Teardrop::String& filename);
	bool DoFileSave(wxString filename);

private:
	DECLARE_EVENT_TABLE()
	void OnIdle(wxIdleEvent& evt);
	void OnEndSession(wxCloseEvent& evt);
	//void onAppActivate(wxActivateEvent& evt);

	Teardrop::GfxRenderer* m_pRend;
	Teardrop::FileStream m_logStrm;
	Teardrop::Logger m_logger;
	Teardrop::Timer m_timer;
	Teardrop::ResourceManager m_resMgr;
	Teardrop::Input m_input;
	Teardrop::Manifest m_manifest;
};

DECLARE_APP(SceneEditorApp);

#endif // SCENEEDITORAPP_INCLUDED
