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

#include "Project.h"
#include "Package/Package.h"
#include "Package/DeferredResolution.h"
#include "Util/FileSystem.h"
#include "PackageManager/PackageManager.h"
#include "PackageManager/PackageMetadata.h"
#include "ProgressFeedback.h"
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

    NameChanged.raise((const char*)mName);
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

static void updateFeedback(ProgressFeedback* feedback = 0, int progress = 0, const char* infoText = 0)
{
	if (feedback)
		feedback->updateProgress(progress, infoText);
}

static void updateFeedback(ProgressFeedback* feedback, const char* infoText)
{
	if (feedback)
		feedback->updateTitle(infoText);
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

	int p = 0;
	while (package) {
		const char* name = package->Attribute("name");
		const char* filename = package->Attribute("filename");

		std::string pkgInfoText("Loading Package: ");
		updateFeedback(feedback, pkgInfoText.c_str());

		if (name && filename) {
			pkgInfoText.append(name);

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

	// Phase 3 -- initialize all objects in the project
	for (PackageManagers::iterator it = mPackageManagers.begin(); it != mPackageManagers.end(); ++it) {
		(*it)->initializeAllObjects();
	}

	// load all thumbnails in parallel
	tbb::task* dummy = new(tbb::task::allocate_root()) DummyTask;
    dummy->set_ref_count(int(metaList.size() + 1));
	for (std::list<Metadata*>::iterator it = metaList.begin(); it != metaList.end(); ++it) {
		tbb::task* t = new(dummy->allocate_child()) ThumbnailTask(*it);
		dummy->spawn(*t);
	}

	dummy->wait_for_all();
	return true;
}

PackageManager* Project::addPackage(const char* packagePath)
{
	// verify that the package's path is our package directory; if not, copy the 
	// package file to our packages dir first and change the path
	String packagesDir(mPath);
	packagesDir += "/packages/";

	String baseName;
	FileSystem::baseName(baseName, packagePath);

	String packageDir;
	FileSystem::directoryName(packageDir, packagePath);
	
	if (!FileSystem::isSamePath(packagesDir, packageDir)) {
		String fileName;
		FileSystem::fileName(fileName, packagePath);
		
		String newPackagePath(packagesDir);
		newPackagePath.append("/");
		newPackagePath.append(fileName);
		
		if (!FileSystem::copyFile(packagePath, newPackagePath)) {
			// then what?
			return 0;
		}
	}

	PackageManager* pkgMgr = new PackageManager;
	pkgMgr->metadata()->setName(baseName);

	DeferredObjectResolves deferred;
	ObjectIdToObject lut;

	if (!pkgMgr->load(packagesDir, deferred, lut)) {
		delete pkgMgr;
		return 0;
	}

	mPackageManagers.push_back(pkgMgr);

	// then do Phase 2 load (object reference resolution)
	for (DeferredObjectResolves::iterator it = deferred.begin(); it != deferred.end(); ++it) {
		DeferredResolution& d = *it;
		ObjectIdToObject::iterator o = lut.find(d.mUUID);
		if (o != lut.end()) {
			d.mProp->setData(d.mObject, o->second);
		}
	}

	std::list<Metadata*> metaList;
	pkgMgr->getAllMetadata(metaList);

	// load all thumbnails in parallel
	tbb::task* dummy = new(tbb::task::allocate_root()) DummyTask;
    dummy->set_ref_count(int(metaList.size() + 1));

	for (std::list<Metadata*>::iterator it = metaList.begin(); it != metaList.end(); ++it) {
		tbb::task* t = new(dummy->allocate_child()) ThumbnailTask(*it);
		dummy->spawn(*t);
	}
	dummy->wait_for_all();

	return pkgMgr;
}

void Project::removePackage(PackageManager* pkgMgr)
{
	mPackageManagers.remove(pkgMgr);
	delete pkgMgr;
}
