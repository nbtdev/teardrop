/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTLIBRARY_INCLUDED)
#define OBJECTLIBRARY_INCLUDED

#include <map>
#include "Util/_String.h"
#include "Memory/Allocators.h"

class TiXmlElement;

namespace Teardrop
{
	struct Environment;
	class ZoneObject;
	class IPlayerControlled;
	class Stream;
	class Zone;

	namespace Reflection
	{
		class ClassDef;
	}

	class ObjectLibrary
	{
	public:
		ObjectLibrary();
		virtual ~ObjectLibrary();

		enum Category
		{
			VEHICLE,
			PROP,
			ZONE,
			UI,
			VFX,
			WEAPON,

			MAX_TYPE
		};

		static ObjectLibrary* instance();
		bool initialize();
		bool destroy();

		//! retrieve object by type and name
		ZoneObject* getByName(Category type, const char* name);
		//! retrieve object by type and name
		ZoneObject* getByName(Reflection::ClassDef* pClass, const char* name);
		//! clone a template into a ready object
		ZoneObject* clone(ZoneObject* pTemplate);
		//! create and initialize an instance of the specified type and name
		ZoneObject* create(
			const String& objectClass,
			const String& objectTypeName,
			IPlayerControlled** ppPC=0);

		TD_DECLARE_ALLOCATOR();

	protected:
		typedef std::map<String, ZoneObject*> NameLUT;
		typedef std::map<String, Category> CategoryByNameLUT;
		typedef std::map<Category, NameLUT> CategoryLUT;
		CategoryLUT m_lut;
		CategoryByNameLUT m_categoryByNameLut;

	private:
		//! create an instance of whatever is defined in the stream
		ZoneObject* create(
			Stream& strm,
			const char* path);
	};
}

#endif // OBJECTLIBRARY_INCLUDED
