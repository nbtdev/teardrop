/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageSerializer.h"
#include "PackageMetadataSerializer.h"
#include "Package.h"
#include "Asset/Asset.h"
#include "Stream/FileStream.h"
#include "Stream/MemoryStream.h"
#include "tinyxml/tinyxml.h"
#include <list>
#include <map>

using namespace Teardrop;

const int PackageSerializer::PACKAGE_VERSION = 1;

struct DataSectionEntry
{
	const void* mData;
	int mLen;
	int mOffset;
	int mOrdinal;
	Asset* mAsset;
};

struct ObjectSectionEntry
{
	ObjectSectionEntry() {}
	ObjectSectionEntry(const ObjectSectionEntry& other) { *this = other; }
	ObjectSectionEntry& operator=(const ObjectSectionEntry& other) {
		mXml = other.mXml;
		mLen = other.mLen;
		mOffset = other.mOffset;
		mObj = other.mObj;
		mOrdinal = other.mOrdinal;
		return *this;
	}

	int mLen;
	int mOffset;
	String mXml;
	int mOrdinal;
	Reflection::Object* mObj;
};

typedef std::list<DataSectionEntry> DataSectionEntries;
typedef std::list<ObjectSectionEntry> ObjectSectionEntries;
typedef std::map<UUID, DataSectionEntry> DataSectionByID;
typedef std::map<UUID, ObjectSectionEntry> ObjectSectionByID;

enum SectionType
{
	SECTION_DATA = 0,
	SECTION_OBJECT = 1,
	SECTION_SYMTAB = 2,
	SECTION_METADATA = 3,

	SECTION_SIZE = 0xFFFFFFFF
};

struct SectionHeader
{
	SectionType mType;
	int mSectionSize; // section length in bytes
	int mNumEntries; // number of entries in this section
	int mNextSection; // offset (from beginning of package) to next header
};

PackageSerializer::PackageSerializer(Package* pkg)
	: mPkg(pkg)
{
}

PackageSerializer::~PackageSerializer()
{

}

static void serializeObjectToXml(Reflection::Object* obj, String& xml, int offset=-1)
{
	Reflection::ClassDef* classDef = obj->getDerivedClassDef();
	
	String objectId;
	obj->getObjectId().toString(objectId);

	TiXmlDocument doc;
	TiXmlElement object("object");
	object.SetAttribute("id", objectId);
	object.SetAttribute("class", classDef->getName());

	if (offset >= 0) {
		String strOrdinal;
		StringUtil::toString(offset, strOrdinal);
		object.SetAttribute("data", strOrdinal);
	}

	// get all props from all superclasses as well
	while (classDef) {
		const Reflection::PropertyDef* prop = classDef->getProps();
		classDef = classDef->getBaseClass();

		while (prop) {
			TiXmlElement elem("property");
			elem.SetAttribute("name", prop->getName());

			String sVal;
			prop->getDataAsString(obj, sVal);
			elem.SetAttribute("value", sVal);

			object.InsertEndChild(elem);
			prop = prop->m_pNext;
		}
	}

	doc.InsertEndChild(object);
	TiXmlPrinter printer;
	doc.Accept(&printer);

	xml = printer.CStr();
}

bool PackageSerializer::serialize(Stream& stream, PackageMetadataSerializer* metadataSerializer)
{
	size_t offset = 0;

	// first, the package format version -- we always write the current version
	PackageHeader hdr;
	memset(&hdr, 0, sizeof(hdr));
	hdr.mVersion = PACKAGE_VERSION;
	hdr.mNumSections = metadataSerializer ? 4 : 3;

	stream.write(&hdr, sizeof(hdr));
	offset = stream.getPosition();

	DataSectionEntries dataEntries;
	DataSectionByID dataById;
	ObjectSectionEntries objectEntries;
	ObjectSectionByID objectById;
	int dataSectionSize = 0;
	int dataOrdinal = 0;
	int objectSectionSize = 0;
	int objectOrdinal = 0;

	//
	// then the objects in the package
	//
	const Objects& objs = mPkg->objects();
	for (Objects::const_iterator it = objs.begin(); it != objs.end(); ++it) {
		Reflection::Object* obj = *it;

		int dataOffset = -1;
		if (obj->getDerivedClassDef()->isA(Asset::getClassDef())) {
			// then it probably has a data blob associated with it
			Asset* asset = static_cast<Asset*>(obj);
			DataSectionEntry ent;
			ent.mAsset = asset;
			ent.mData = asset->data();
			ent.mLen = asset->length();
			ent.mOffset = dataSectionSize;
			ent.mOrdinal = dataOrdinal++;
			dataSectionSize += ent.mLen;

			dataEntries.push_back(ent);
			dataById[obj->getObjectId()] = ent;
			dataOffset = ent.mOffset;
		}

		ObjectSectionEntry ent;
		serializeObjectToXml(obj, ent.mXml, objectOrdinal);
		ent.mLen = ent.mXml.length() + 1; // include null terminator
		ent.mOffset = objectSectionSize;
		ent.mOrdinal = objectOrdinal++;
		objectSectionSize += ent.mLen;
		ent.mObj = obj;

		objectEntries.push_back(ent);
		objectById[obj->getObjectId()] = ent;
	}

	int nDataEntries = dataOrdinal;
	int nObjectEntries = objectOrdinal;

	// write the data section, header first
	SectionHeader dataHdr;
	dataHdr.mType = SECTION_DATA;
	dataHdr.mNumEntries = nDataEntries;
	dataHdr.mSectionSize = dataSectionSize + dataEntries.size() * sizeof(int);
	dataHdr.mNextSection = int(offset) + sizeof(SectionHeader) + dataSectionSize + dataEntries.size() * sizeof(int);
	stream.write(&dataHdr, sizeof(dataHdr));

	for (DataSectionEntries::iterator it = dataEntries.begin(); it != dataEntries.end(); ++it) {
		DataSectionEntry& ent = *it;
		stream.write(&ent.mLen, sizeof(ent.mLen));
		stream.write(ent.mData, ent.mLen);
	}

	offset = stream.getPosition();

	// write the object section, header first
	SectionHeader objHdr;
	objHdr.mType = SECTION_OBJECT;
	objHdr.mSectionSize = objectSectionSize + objectEntries.size() * sizeof(int);
	objHdr.mNumEntries = nObjectEntries;
	objHdr.mNextSection = int(offset) + sizeof(SectionHeader) + objectSectionSize + objectEntries.size() * sizeof(int);
	stream.write(&objHdr, sizeof(objHdr));

	for (ObjectSectionEntries::iterator it = objectEntries.begin(); it != objectEntries.end(); ++it) {
		ObjectSectionEntry& ent = *it;
		stream.write(&ent.mLen, sizeof(ent.mLen));
		stream.write((const char*)ent.mXml, ent.mLen);
	}

	offset = stream.getPosition();

	// write the object symbol table
	SectionHeader symTabHdr;
	symTabHdr.mType = SECTION_SYMTAB;
	// a symbol table entry is 40 bytes for the UUID string and 4 bytes for the object ordinal
	symTabHdr.mSectionSize = nObjectEntries * (40 + 4);
	symTabHdr.mNumEntries = nObjectEntries;
	symTabHdr.mNextSection = int(offset) + sizeof(SectionHeader) + symTabHdr.mSectionSize;
	stream.write(&symTabHdr, sizeof(symTabHdr));

	for (ObjectSectionEntries::iterator it = objectEntries.begin(); it != objectEntries.end(); ++it) {
		ObjectSectionEntry& ent = *it;

		String objId;
		int buf = 0;
		ent.mObj->getObjectId().toString(objId);
		stream.write((const char*)objId, 36);
		stream.write(&buf, sizeof(int));
		stream.write(&ent.mOrdinal, sizeof(ent.mOrdinal));
	}

	offset = stream.getPosition();

	if (metadataSerializer) {
		MemoryStream memStrm;
		metadataSerializer->serialize(mPkg, memStrm);

		SectionHeader metaHdr;
		metaHdr.mType = SECTION_METADATA;
		metaHdr.mNextSection = 0;
		metaHdr.mSectionSize = memStrm.length();
		metaHdr.mNumEntries = 0;
		stream.write(&metaHdr, sizeof(metaHdr));

		stream.write(memStrm.data(), memStrm.length());
	}

	return true;
}

struct DataEntry
{
	void* mData;
	int mLen;
};

static Reflection::Object* deserializeObjectFromXml(const char* xml, const std::vector<DataEntry>& dataEntries)
{
	TiXmlDocument doc;
	doc.Parse(xml);
	if (doc.Error())
		return 0;

	TiXmlElement* root = doc.RootElement();
	const char* id = root->Attribute("id");
	const char* type = root->Attribute("class");
	if (id && type) {
		Reflection::ClassDef* classDef = Reflection::ClassDef::findClassDef(type);
		if (classDef) {
			Reflection::Object* obj = classDef->createInstance();
			if (obj) {
				// check to see if there is a "data" value, if so, set up the object with the right one
				const char* ordinal = root->Attribute("data");
				if (ordinal) {
					Asset* asset = static_cast<Asset*>(obj);
					int ord;
					StringUtil::fromString(ordinal, ord);
					asset->setData(dataEntries[ord].mData, dataEntries[ord].mLen);
				}

				// set the object ID
				obj->setObjectId(id);

				// then populate the properties (first set the defaults on the object)
				obj->setupPropertyDefaults();

				TiXmlElement* propElem = root->FirstChildElement("property");
				while (propElem) {
					const char* name = propElem->Attribute("name");
					const char* value = propElem->Attribute("value");
					const Reflection::PropertyDef* prop = classDef->findProperty(name, true);
					if (prop && value) {
						prop->setDataFromString(obj, value);
					}

					propElem = propElem->NextSiblingElement("property");
				}
			}

			return obj;
		}
	}

	return 0;
}

struct SymTabEntry
{
	char mId[40];
	int mOrdinal;
};

bool PackageSerializer::deserialize(Stream& stream, PackageMetadataSerializer* metadataSerializer)
{
	// first read the header
	PackageHeader hdr;
	stream.read(&hdr, sizeof(hdr));

	if (hdr.mVersion != PACKAGE_VERSION) {
		// TODO: do something to invoke a legacy deserializer?
		return true;
	}

	assert(hdr.mNumSections >= 3);
	if (hdr.mNumSections < 3)
		return false;

	std::vector<DataEntry> dataEntries;
	std::vector<Reflection::Object*> objects;

	// read in the sections
	for (int s = 0; s < hdr.mNumSections; ++s) {
		SectionHeader secHdr;
		stream.read(&secHdr, sizeof(secHdr));

		if (secHdr.mType == SECTION_DATA) {
			// slurp it all into the package
			unsigned char* data = (unsigned char*)mPkg->createDataStorage(secHdr.mSectionSize);
			stream.read(data, secHdr.mSectionSize);

			// set up pointers to the data for later use
			dataEntries.resize(secHdr.mNumEntries);

			// and then "parse" the data blob
			for (int i=0; i<secHdr.mNumEntries; ++i) {
				int* pLen = (int*)data;
				dataEntries[i].mLen = *pLen++;
				dataEntries[i].mData = pLen;
				data = (unsigned char*)pLen;
				data += dataEntries[i].mLen;
			}
		}

		if (secHdr.mType == SECTION_OBJECT) {
			objects.resize(secHdr.mNumEntries, 0);

			for (int i=0; i<secHdr.mNumEntries; ++i) {
				int len;
				stream.read(&len, sizeof(len));

				char* buf = new char[len];
				stream.read(buf, len);
				objects[i] = deserializeObjectFromXml(buf, dataEntries);
				delete [] buf;
				mPkg->add(objects[i]);
			}
		}

		if (secHdr.mType == SECTION_SYMTAB) {
			for (int i=0; i<secHdr.mNumEntries; ++i) {
				SymTabEntry ent;
				stream.read(&ent, sizeof(ent));
				mPkg->addSymTabEntry(objects[ent.mOrdinal]);
			}
		}

		if (secHdr.mType == SECTION_METADATA) {
			if (metadataSerializer)
				metadataSerializer->deserialize(mPkg, stream);
		}
	}

	return true;
}
