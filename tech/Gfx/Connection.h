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

#if !defined(CONNECTION_INCLUDED)
#define CONNECTION_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Gfx 
	{
		struct Attribute;
		class MaterialExpression;
		class Material;

		class Connection : public Reflection::Object 
		{
		public:
			TD_CLASS(Connection, Object);
			TD_POINTER_PROPERTY(FromExpression, "Source MaterialExpression", MaterialExpression, "Hidden");
			TD_PROPERTY(FromAttribute, "Source Attribute Name", String, "", "Hidden");
			TD_POINTER_PROPERTY(ToExpression, "Destination MaterialExpression", MaterialExpression, "Hidden");
			TD_PROPERTY(ToAttribute, "Destination Attribute Name", String, "", "Hidden");
			TD_POINTER_PROPERTY(Parent, "Parent Material", Material, "Hidden");

			Connection();
			~Connection();

			bool initialize();

			Attribute* input();
			Attribute* output();

			TD_DECLARE_ALLOCATOR();

		protected:
			Attribute* mInput;
			Attribute* mOutput;
		};
	}
}

#endif // CONNECTION_INCLUDED
