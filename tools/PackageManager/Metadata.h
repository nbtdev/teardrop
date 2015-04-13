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

#if !defined(METADATA_INCLUDED)
#define METADATA_INCLUDED

#include "PackageManager/Thumbnail.h"
#include "Reflection/Reflection.h"
#include "Memory/Memory.h"
#include "Util/Event.h"

namespace Teardrop
{
	namespace Tools
	{
		class Thumbnail;

		class Metadata : public Reflection::Object
		{
		public:
			TD_CLASS(Metadata, Object);
			TD_PROPERTY(Name, "Object Name", String, "(undefined)", 0);
			TD_PROPERTY(ID, "Object ID", String, "(undefined)", "ReadOnly");

			Metadata();
			~Metadata();

			// default implementation just uses the default Teardrop icon
			virtual void generateThumbnail();
			const Thumbnail& thumbnail();

            Event<const char*> NameChanged;

			Reflection::Object* object();

			TD_DECLARE_ALLOCATOR();

		protected:
			void notifyPropertyChangedLocal(const Reflection::PropertyDef* prop);
			Thumbnail mThumb;

			friend class PackageMetadata;
			Reflection::Object* mObject;
		};
	}
}

#endif // METADATA_INCLUDED
