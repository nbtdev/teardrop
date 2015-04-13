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

#if !defined(VARIANT_INCLUDED)
#define VARIANT_INCLUDED

namespace Teardrop
{
	struct Variant
	{
		typedef enum
		{
			VT_NONE,
			VT_INT,
			VT_BOOL,
			VT_FLOAT,
			VT_STRING,
			VT_USER
		} Type;

		union Val
		{
			int i;
			float f;
			const char* s;
			void* p;
			bool b;
		};

		Val v;
		Type type;

		void setInt(int i);
		void setFloat(float f);
		void setString(const char* s);
		void setBool(bool b);
		void setUser(void* p);
	};
}

#endif // VARIANT_INCLUDED
