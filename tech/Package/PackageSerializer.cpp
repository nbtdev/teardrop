/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PackageSerializer.h"
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
	int mReserved; // pad this struct to 16 bytes
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
	object.SetAttribute("ID", objectId);
	object.SetAttribute("Class", classDef->getName());

	// get all props from all superclasses as well
	while (classDef) {
		const Reflection::PropertyDef* prop = classDef->getProps();
		classDef = classDef->getBaseClass();

		while (prop) {
			TiXmlElement elem("property");
			elem.SetAttribute("Name", prop->getName());

			String sVal;
			prop->getDataAsString(obj, sVal);
			elem.SetAttribute("Value", sVal);

			object.InsertEndChild(elem);
			prop = prop->m_pNext;
		}
	}

	doc.InsertEndChild(object);
	TiXmlPrinter printer;
	doc.Accept(&printer);

	xml = printer.CStr();
}

bool PackageSerializer::serialize(Stream& stream)
{
	size_t offset = 0;

	// first, the package format version -- we always write the current version
	PackageHeader hdr;
	memset(&hdr, 0, sizeof(hdr));
	hdr.mVersion = PACKAGE_VERSION;

	stream.write(&hdr, sizeof(hdr));
	offset += stream.getPosition();

	DataSectionEntries dataEntries;
	DataSectionByID dataById;
	ObjectSectionEntries objectEntries;
	ObjectSectionByID objectById;
	int dataSectionOffset = 0;
	int dataOrdinal = 0;
	int objectSectionOffset = 0;
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
			ent.mOffset = dataSectionOffset;
			ent.mOrdinal = dataOrdinal++;
			dataSectionOffset += ent.mLen;

			dataEntries.push_back(ent);
			dataById[obj->getObjectId()] = ent;
			dataOffset = ent.mOffset;
		}

		ObjectSectionEntry ent;
		serializeObjectToXml(obj, ent.mXml, dataOffset);
		ent.mLen = ent.mXml.length() + 1; // include null terminator
		ent.mOffset = dataSectionOffset;
		ent.mOrdinal = objectOrdinal++;
		dataSectionOffset += ent.mLen;
		ent.mObj = obj;

		objectEntries.push_back(ent);
		objectById[obj->getObjectId()] = ent;
	}

	int dataSectionSize = dataSectionOffset;
	int nDataEntries = dataOrdinal;
	int objectSectionSize = objectSectionOffset;
	int nObjectEntries = objectOrdinal;

	// write the data section, header first
	SectionHeader dataHdr;
	dataHdr.mType = SECTION_DATA;
	dataHdr.mNumEntries = nDataEntries;
	dataHdr.mSectionSize = dataSectionSize;
	dataHdr.mReserved = 0;
	stream.write(&dataHdr, sizeof(dataHdr));

	for (DataSectionEntries::iterator it = dataEntries.begin(); it != dataEntries.end(); ++it) {
		DataSectionEntry& ent = *it;
		stream.write(&ent.mLen, sizeof(ent.mLen));
		stream.write(ent.mData, ent.mLen);
	}

	// write the object section, header first
	SectionHeader objHdr;
	objHdr.mType = SECTION_OBJECT;
	objHdr.mReserved = 0;
	objHdr.mSectionSize = objectSectionSize;
	objHdr.mNumEntries = nObjectEntries;
	stream.write(&objHdr, sizeof(objHdr));

	for (ObjectSectionEntries::iterator it = objectEntries.begin(); it != objectEntries.end(); ++it) {
		ObjectSectionEntry& ent = *it;
		stream.write(&ent.mLen, sizeof(ent.mLen));
		stream.write((const char*)ent.mXml, ent.mLen);
	}

	return true;
}

bool PackageSerializer::deserialize(Stream& stream)
{
	return false;
}
