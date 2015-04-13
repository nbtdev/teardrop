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

#if !defined(OBJECTDRAGDROPDATA_INCLUDED)
#define OBJECTDRAGDROPDATA_INCLUDED

#include "DragDropData.h"
#include "Util/_String.h"

// Data class for Reflection::Object items being dragged around the editor

namespace Teardrop
{
	class Package;

	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class ObjectDragDropData : public DragDropData
		{
		public:
			ObjectDragDropData(Reflection::Object* obj, const String& path, Package* pkg);
			~ObjectDragDropData();

			Type type();
			Reflection::Object* object();
			const String& path();
			Package* package();

		protected:
			Reflection::Object* mObject;
			String mPath; // object virtual path in its package (metadata)
			Package* mPackage; // package to which this object belongs 
		};
	}
}

#endif // OBJECTDRAGDROPDATA_INCLUDED
