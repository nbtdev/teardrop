/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(OBJECTLIBRARY_INCLUDED)
#define OBJECTLIBRARY_INCLUDED

#include <map>
#include "Util/include/_String.h"
#include "Memory/include/Allocators.h"

class TiXmlElement;

namespace CoS
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

		COS_DECLARE_ALLOCATOR();

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
