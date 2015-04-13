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

#if !defined(FOLDERDRAGDROPDATA_INCLUDED)
#define FOLDERDRAGDROPDATA_INCLUDED

#include "DragDropData.h"
#include "Util/_String.h"

// Data class for Reflection::Object items being dragged around the editor

namespace Teardrop
{
	class Package;

	namespace Tools
	{
		class Folder;
		class FolderDragDropData : public DragDropData
		{
		public:
			FolderDragDropData(Folder* folder, const String& path, Package* pkg);
			~FolderDragDropData();

			Type type();
			Folder* folder();
			const String& path();
			Package* package();

		protected:
			Folder* mFolder;
			String mPath; // folder virtual path in its package (metadata)
			Package* mPackage; // package to which this folder belongs 
		};
	}
}

#endif // FOLDERDRAGDROPDATA_INCLUDED
