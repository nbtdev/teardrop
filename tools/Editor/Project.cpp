/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Project.h"
#include "Asset/Package.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "tinyxml/tinyxml.h"
#include <sstream>

using namespace Teardrop;
using namespace Tools;

const int Project::CURRENT_VERSION = 1;

Project::Project()
	: mVersion(CURRENT_VERSION)
{

}

Project::~Project()
{

}

const String& Project::name()
{
	return mName;
}

int Project::version()
{
	return mVersion;
}

void Project::rename(const String& newName)
{
	mName = newName;

	if (NameChanged)
		NameChanged((const char*)mName);
}

void Project::onPackageAdded(PackageManager* pkgMgr)
{
	mPackageManagers.push_back(pkgMgr);
}

void Project::onPackageRemoved(PackageManager* pkgMgr)
{
	mPackageManagers.remove(pkgMgr);
}

const String& Project::path()
{
	return mPath;
}

void Project::setPath(const String& path)
{
	mPath = path;
}

#if defined(_WIN32) || defined(_WIN64)
const char* PATHSEP = "\\";
#else
const char* PATHSEP = "/";
#endif

bool Project::write()
{
	String pathname(mPath);
	pathname += PATHSEP;
	pathname += mName;
	pathname += ".project";

	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", "", "");
	doc.InsertEndChild(decl);

	TiXmlElement project("project");
	std::stringstream ss;
	ss << mVersion;
	project.SetAttribute("version", ss.str().c_str());

	for (PackageManagers::iterator it = mPackageManagers.begin(); it != mPackageManagers.end(); ++it) {
		PackageManager* pkgMgr = *it;

		TiXmlElement package("package");
		String packageName(pkgMgr->metadata()->getName());
		package.SetAttribute("name", packageName);

		packageName.replaceAll(' ', '_');
		packageName += ".package";
		package.SetAttribute("filename", packageName);

		project.InsertEndChild(package);
	}

	doc.InsertEndChild(project);

	doc.SaveFile(pathname);

	return true;
}

bool Project::read()
{
	String pathname(mPath);
	pathname += PATHSEP;
	pathname += mName;
	pathname += ".project";

	TiXmlDocument doc;
	doc.LoadFile(pathname);

	if (doc.Error()) {
		return false;
	}

	// otherwise, read project file
	TiXmlElement* root = doc.RootElement();

	return true;
}
