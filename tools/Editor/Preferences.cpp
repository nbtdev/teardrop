/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#include "Preferences.h"
#include "Util/FileSystem.h"
#include "Util/UUID.h"
#include "ThirdParty/tinyxml/tinyxml.h"
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
				mEngineId.fromString(value);
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

	String id;
	mEngineId.toString(id);
	renderEngine.SetAttribute("value", id);

	rendering.InsertEndChild(renderEngine);
	prefs.InsertEndChild(rendering);
}
