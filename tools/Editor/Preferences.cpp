/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Preferences.h"
#include "Util/FileSystem.h"
#include "tinyxml/tinyxml.h"
#include <algorithm>

using namespace Teardrop;
using namespace Tools;

static const char* DEFAULT_PREFS_FILENAME = "editor.prefs";

Preferences::Preferences()
{

}

Preferences::~Preferences()
{

}

Preferences::General& Preferences::general()
{
	return mGeneral;
}

Preferences::Rendering& Preferences::rendering()
{
	return mRendering;
}

bool Preferences::load(const char* prefsFilename/* =0 */)
{
	String appDataPath;
	FileSystem::getAppDataPath(appDataPath);

	if (prefsFilename == 0)
		prefsFilename = DEFAULT_PREFS_FILENAME;

	appDataPath.append(FileSystem::PATHSEP).append(prefsFilename);

	TiXmlDocument doc;
	if (!doc.LoadFile(appDataPath))
		return false;

	TiXmlElement* root = doc.RootElement();
	if (!root)
		return false;

	TiXmlElement* general = root->FirstChildElement("general");
	if (general) {
		mGeneral.load(*general);
	}

	TiXmlElement* rendering = root->FirstChildElement("rendering");
	if (rendering) {
		mRendering.load(*rendering);
	}

	TiXmlElement* projects = root->FirstChildElement("projects");
	if (projects) {
		TiXmlElement* project = projects->FirstChildElement("project");
		while (project) {
			const char* pathname = project->GetText();
			mLastProjects.push_back(pathname);
			project = project->NextSiblingElement("project");
		}
	}

	return true;
}
bool Preferences::save(const char* prefsFilename/* =0 */)
{
	String appDataPath;
	FileSystem::getAppDataPath(appDataPath);

	if (prefsFilename == 0)
		prefsFilename = DEFAULT_PREFS_FILENAME;

	appDataPath.append(FileSystem::PATHSEP).append(prefsFilename);

	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", "", "");
	doc.InsertEndChild(decl);

	TiXmlElement root("preferences");

	mGeneral.save(root);
	mRendering.save(root);

	TiXmlElement projects("projects");
	for (ProjectList::iterator it = mLastProjects.begin(); it != mLastProjects.end(); ++it) {
		TiXmlElement project("project");
		TiXmlText name(*it);
		project.InsertEndChild(name);
		projects.InsertEndChild(project);
	}

	root.InsertEndChild(projects);
	doc.InsertEndChild(root);

	return doc.SaveFile(appDataPath);
}

void Preferences::addProject(const String& projectPathname)
{
	// see if this project already in the list
	ProjectList::iterator it = std::find(mLastProjects.begin(), mLastProjects.end(), projectPathname);
	if (it != mLastProjects.end())
		return;

	// remove the project at the end of the list...
	if (mLastProjects.size())
		mLastProjects.erase(--(mLastProjects.end()));

	// and put this one at the top
	mLastProjects.push_front(projectPathname);
}

const Preferences::ProjectList& Preferences::projectList()
{
	return mLastProjects;
}

Preferences::General::General()
	: mLoadLastProject(false)
{

}

Preferences::General::~General()
{

}

void Preferences::General::load(TiXmlElement& prefs)
{
	TiXmlElement* pref = prefs.FirstChildElement("pref");
	while (pref) {
		const char* name = pref->Attribute("name");
		const char* value = pref->Attribute("value");

		if (name && value) {
			if (!strcmp(name, "loadLastProject")) mLoadLastProject = (0 == strcmp(value, "true"));
		}

		pref = pref->NextSiblingElement("pref");
	}
}

void Preferences::General::save(TiXmlElement& prefs)
{
	TiXmlElement general("general");

	TiXmlElement loadLastProject("pref");
	loadLastProject.SetAttribute("name", "loadLastProject");
	loadLastProject.SetAttribute("value", mLoadLastProject ? "true" : "false");
	general.InsertEndChild(loadLastProject);
	prefs.InsertEndChild(general);
}

Preferences::Rendering::Rendering()
	: mEngine(ENGINE_D3D9)
{

}

Preferences::Rendering::~Rendering()
{

}

void Preferences::Rendering::load(TiXmlElement& prefs)
{
	TiXmlElement* pref = prefs.FirstChildElement("pref");
	while (pref) {
		const char* name = pref->Attribute("name");
		const char* value = pref->Attribute("value");

		if (name && value) {
			if (!strcmp(name, "renderEngine")) {
				if (!strcmp(value, "OPENGL")) mEngine = ENGINE_OPENGL;
				if (!strcmp(value, "D3D9")) mEngine = ENGINE_D3D9;
				if (!strcmp(value, "D3D11")) mEngine = ENGINE_D3D11;
			}
		}

		pref = pref->NextSiblingElement("pref");
	}
}

void Preferences::Rendering::save(TiXmlElement& prefs)
{
	TiXmlElement rendering("rendering");

	TiXmlElement renderEngine("pref");
	renderEngine.SetAttribute("name", "renderEngine");
	
	const char* engineName = "";
	switch (mEngine) {
		case ENGINE_OPENGL: engineName = "OPENGL"; break;
		case ENGINE_D3D9: engineName = "D3D9"; break;
		case ENGINE_D3D11: engineName = "D3D11"; break;
	}

	renderEngine.SetAttribute("value", engineName);
	rendering.InsertEndChild(renderEngine);
	prefs.InsertEndChild(rendering);
}
