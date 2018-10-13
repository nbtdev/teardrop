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

#include "PackageSerializer.h"
#include "PackageMetadataSerializer.h"
#include "Package.h"
#include "UUIDStreamReader.h"
#include "UUIDStreamWriter.h"
#include "Asset/Asset.h"
#include "Stream/FileStream.h"
#include "Stream/MemoryStream.h"
#include "ThirdParty/tinyxml/tinyxml.h"
#include <list>
#include <map>

using namespace Teardrop;

const int PackageSerializer::PACKAGE_VERSION = 1;

PackageSerializer::PackageSerializer(Package* pkg)
	: mPkg(pkg)
{
}

PackageSerializer::~PackageSerializer()
{

}

static int serializeObjects(const Objects& objects, Stream& stream, std::list<Asset*>& assets)
{
	TiXmlDocument doc;
	TiXmlElement objectsElem("objects");

	for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it) {
		Reflection::Object* obj = *it;
		const Reflection::ClassDef* classDef = obj->getDerivedClassDef();

		// store off the things that need to be serialized
		if (classDef->isA(Asset::getClassDef())) {
			assets.push_back(static_cast<Asset*>(obj));
		}
		
		String objectId;
		obj->getObjectId().toString(objectId);

		TiXmlElement object("object");
		object.SetAttribute("id", objectId);
		object.SetAttribute("class", classDef->getName());

		// get all props from all superclasses as well
		while (classDef) {
			const Reflection::PropertyDef* prop = classDef->getProps();
			classDef = classDef->getBaseClass();

			while (prop) {
				TiXmlElement elem("property");
				elem.SetAttribute("name", prop->getName());

				if (prop->isNested()) {
					// then it's actually an object, so we need to 
					// get its own classdef and list of properties...
					Reflection::Object* o = (Reflection::Object*)prop->getDataPointer(obj);
					const Reflection::ClassDef* c = o->getDerivedClassDef();
					const Reflection::PropertyDef* pd = c->getProps();
					while (pd) {
						TiXmlElement np("property");
						np.SetAttribute("name", pd->getName());

						// only support a single level of nesting, for now
						String sVal;
						pd->getDataAsString(o, sVal);
						np.SetAttribute("value", sVal);
						elem.InsertEndChild(np);
						pd = pd->m_pNext;
					}
				}
				else {
					String sVal;
					prop->getDataAsString(obj, sVal);
					elem.SetAttribute("value", sVal);
				}

				object.InsertEndChild(elem);
				prop = prop->m_pNext;
			}
		}
		
		objectsElem.InsertEndChild(object);
	}

	doc.InsertEndChild(objectsElem);
	TiXmlPrinter printer;
	doc.Accept(&printer);

	// first, we need to know how long the XML string is
	int len = printer.Size();
	stream.write(&len, sizeof(len));

	// then the actual XML
	const char* xml = printer.CStr();
	stream.write(xml, len);

	return len + sizeof(len);
}

int PackageSerializer::serialize(Stream& stream, PackageMetadataSerializer* metadataSerializer)
{
	int nBytes = 0;

	// first, the package format version -- we always write the current version
	PackageHeader hdr;
	memset(&hdr, 0, sizeof(hdr));
	hdr.mVersion = PACKAGE_VERSION;

	nBytes += stream.write(&hdr, sizeof(hdr));

	// then the objects in the package
	const Objects& objs = mPkg->objects();
	std::list<Asset*> assetList;
	nBytes += serializeObjects(objs, stream, assetList);

	// then all asset data, if any
	for (std::list<Asset*>::iterator it = assetList.begin(); it != assetList.end(); ++it) {
		Asset* asset = *it;

		// we need to know its ID for later deserialization
		UUID id = asset->getObjectId();
        UUIDStreamWriter sw(stream, id);
        nBytes += sw.write();

		// then the actual asset data
		nBytes += asset->serialize(stream);
	}

	// mark the end of asset data with an all-zeroes UUID
	UUID zero;
    UUIDStreamWriter sw(stream, zero);
    nBytes += sw.write();

	// and finally, if the caller supplied an editor metadata serializer, allow that to work
	if (metadataSerializer)
		nBytes += metadataSerializer->serialize(mPkg, stream);

	return nBytes;
}

static bool deserializeObjects(const char* xml, Package* pkg, DeferredObjectResolves& deferred, ObjectIdToObject& lut)
{
	TiXmlDocument doc;
	doc.Parse(xml);
	if (doc.Error())
		return false;

	TiXmlElement* objects = doc.RootElement();
	if (!objects)
		return false;

	TiXmlElement* object = objects->FirstChildElement("object");

	while (object) {
		const char* id = object->Attribute("id");
		const char* type = object->Attribute("class");

		if (id && type) {
			Reflection::ClassDef* classDef = Reflection::ClassDef::findClassDef(type);
			if (classDef) {
				Reflection::Object* obj = classDef->createInstance();
				if (obj) {
					// set the object ID
					obj->setObjectId(id);

					// then populate the properties (first set the defaults on the object)
					obj->setupPropertyDefaults();

					TiXmlElement* propElem = object->FirstChildElement("property");
					while (propElem) {
						const char* name = propElem->Attribute("name");
						const char* value = propElem->Attribute("value");
						const Reflection::PropertyDef* prop = classDef->findProperty(name, true);
						if (prop) {
							if (value) {
								if (prop->isPointer()) {
									// pointers have to wait until all objects are loaded so that
									// they are available for resolution
									DeferredResolution def;
									def.mObject = obj;
									def.mProp = prop;
									def.mUUID.fromString(value);
									deferred.push_back(def);
								}
								else {
									prop->setDataFromString(obj, value);
								}
							}
							else if (prop->isNested()) {
								// then the property object instance already exists, just 
								// read in the nested object's property values
								Reflection::Object* o = (Reflection::Object*)prop->getDataPointer(obj);
								const Reflection::ClassDef* c = o->getDerivedClassDef();

								TiXmlElement* np = propElem->FirstChildElement("property");
								while (np) {
									const char* pname = np->Attribute("name");
									const char* pval = np->Attribute("value");

									if (pname && pval) {
										const Reflection::PropertyDef* pd = c->findProperty(pname);
										if (pd && pd->isPointer()) {
											DeferredResolution def;
											def.mObject = o;
											def.mProp = pd;
											def.mUUID.fromString(pval);
											deferred.push_back(def);
										}
										else {
											pd->setDataFromString(o, pval);
										}
									}

									np = np->NextSiblingElement("property");
								}
							}
						}

						propElem = propElem->NextSiblingElement("property");
					}
				}

				pkg->add(obj);
				lut[obj->getObjectId()] = obj;
			}
		}

		object = object->NextSiblingElement("object");
	}

	return true;
}

int PackageSerializer::deserialize(Stream& stream, DeferredObjectResolves& deferred, ObjectIdToObject& lut, PackageMetadataSerializer* metadataSerializer)
{
	int nBytes = 0;

	// first read the header
	PackageHeader hdr;
	nBytes += stream.read(&hdr, sizeof(hdr));

	if (hdr.mVersion != PACKAGE_VERSION) {
		// TODO: do something to invoke a legacy deserializer?
		return 0;
	}

	// read in the object definitions
	int len;
	nBytes += stream.read(&len, sizeof(len));

	// read this many bytes into a string
	std::vector<char> xml(len);
	nBytes += stream.read(&xml[0], len);

	// and then deserialize objects from that
	if (!deserializeObjects(&xml[0], mPkg, deferred, lut))
		return 0;

	// then the data
	Reflection::ClassDef* assetClass = Asset::getClassDef();
	UUID id, zero;
    UUIDStreamReader sr(stream, id);
    nBytes += sr.read();

	while (id != zero) {
		// find in the package, the object by its ID
		Reflection::Object* obj = mPkg->findById(id);
		if (obj && obj->getDerivedClassDef()->isA(assetClass)) {
			Asset* asset = static_cast<Asset*>(obj);
			nBytes += asset->deserialize(stream);
		}

        nBytes += sr.read();
	}

	// then finally, if the caller provided a metadata serializer, use it
	if (metadataSerializer) {
		nBytes += metadataSerializer->deserialize(mPkg, stream);
	}

	return nBytes;
}
