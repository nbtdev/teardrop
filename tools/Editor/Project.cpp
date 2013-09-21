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
#include "QtUtils/ProgressFeedback.h"
#include "tinyxml/tinyxml.h"
#include <sstream>
#include <tbb/task.h>

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

bool Project::write(ProgressFeedback* feedback)
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
	std::string str(ss.str());
	project.SetAttribute("version", str.c_str());

	ss.str("");
	ss << mPackageManagers.size();
	str.assign(ss.str());
	project.SetAttribute("package_count", str.c_str());

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

class ThumbnailTask : public tbb::task
{
	Metadata* mMetadata;
	tbb::task* execute() {
		if (mMetadata)
			mMetadata->generateThumbnail();

		return NULL;
	}

public:
	ThumbnailTask(Metadata* meta) : mMetadata(meta) {}
};

class DummyTask : public tbb::task
{
public:
	DummyTask() {}
	tbb::task* execute() { return NULL; }
};

static void updateFeedback(ProgressFeedback* feedback=0, int progress=0, const char* infoText=0)
{
	if (feedback)
		feedback->updateProgress(progress, infoText);
}

bool Project::read(ProgressFeedback* feedback)
{
	String pathname(mPath);
	pathname += PATHSEP;
	pathname += mName;
	pathname += ".project";

	TiXmlDocument doc;
	doc.LoadFile(pathname);

	updateFeedback(feedback);

	if (doc.Error()) {
		return false;
	}

	// otherwise, read project file
	TiXmlElement* root = doc.RootElement();

	// TODO: check version compatibility
	const char* nPackageAttr = root->Attribute("package_count");
	int nPkg = -1;
	if (nPackageAttr)
		nPkg = atoi(nPackageAttr);

	TiXmlElement* package = root->FirstChildElement("package");

	// for deferred object-reference resolution
	DeferredObjectResolves deferred;
	ObjectIdToObject lut;

	// for metadata thumbnail loading
	std::list<Metadata*> metaList;

	int p = 1;
	while (package) {
		const char* name = package->Attribute("name");
		const char* filename = package->Attribute("filename");

		std::string pkgInfoText("Loading Package: ");

		if (name && filename) {
			pkgInfoText.append(name);
			updateFeedback(feedback, int(float(p)/float(nPkg)*100.f), pkgInfoText.c_str());

			PackageManager* pkgMgr = new PackageManager();
			pkgMgr->metadata()->setName(name);

			String path(mPath);
			path += "/packages/";
			pkgMgr->load(path, deferred, lut);

			// add package metadata to list to post-process
			pkgMgr->getAllMetadata(metaList);

			mPackageManagers.push_back(pkgMgr);
			updateFeedback(feedback, int(float(++p)/float(nPkg)*100.f), pkgInfoText.c_str());
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

	// load all thumbnails in parallel
	tbb::task* dummy = new(tbb::task::allocate_root()) DummyTask;
	dummy->set_ref_count(metaList.size() + 1);
	for (std::list<Metadata*>::iterator it = metaList.begin(); it != metaList.end(); ++it) {
		tbb::task* t = new(dummy->allocate_child()) ThumbnailTask(*it);
		dummy->spawn(*t);
	}

	dummy->wait_for_all();
	return true;
}
