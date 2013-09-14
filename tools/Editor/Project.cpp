/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Project.h"
#include "Package/Package.h"
#include "Package/DeferredResolution.h"
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
	mName = "Untitled";
}

Project::~Project()
{
	for (PackageManagers::iterator it = mPackageManagers.begin(); it != mPackageManagers.end(); ++it) {
		delete *it;
	}
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

PackageManager* Project::createPackage()
{
	PackageManager* pkgMgr = new PackageManager();
	pkgMgr->metadata()->setName("Untitled Package");
	mPackageManagers.push_back(pkgMgr);
	return pkgMgr;
}

const String& Project::path()
{
	return mPath;
}

const Project::PackageManagers& Project::packages()
{
	return mPackageManagers;
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

	String packagePath(mPath);
	packagePath += "/packages/";

	for (PackageManagers::iterator it = mPackageManagers.begin(); it != mPackageManagers.end(); ++it) {
		PackageManager* pkgMgr = *it;

		TiXmlElement package("package");
		String name(pkgMgr->metadata()->getName());
		package.SetAttribute("name", name);

		name = pkgMgr->packageFilename();
		package.SetAttribute("filename", name);

		project.InsertEndChild(package);

		// write out the actual package file
		pkgMgr->save(packagePath);
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
	TiXmlElement* package = root->FirstChildElement("package");

	// for deferred object-reference resolution
	DeferredObjectResolves deferred;
	ObjectIdToObject lut;

	while (package) {
		const char* name = package->Attribute("name");
		const char* filename = package->Attribute("filename");

		if (name && filename) {
			PackageManager* pkgMgr = new PackageManager();
			pkgMgr->metadata()->setName(name);

			String path(mPath);
			path += "/packages/";
			pkgMgr->load(path, deferred, lut);

			mPackageManagers.push_back(pkgMgr);
		}

		package = package->NextSiblingElement("package");
	}

	// then do Phase 2 load (object reference resolution)
	for (DeferredObjectResolves::iterator it = deferred.begin(); it != deferred.end(); ++it) {
		DeferredResolution& d = *it;
		ObjectIdToObject::iterator o = lut.find(d.mUUID);
		if (o != lut.end()) {
			d.mProp->setData(d.mObject, o->second);
		}
	}

	return true;
}
