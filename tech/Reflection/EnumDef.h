/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined (ENUMDEF_INCLUDED)
#define ENUMDEF_INCLUDED

namespace Teardrop
{
	namespace Reflection
	{
		class EnumValue;

		class EnumDef
		{
		public:
			EnumDef(const char* name, const char* description);
			~EnumDef();

			const char* name() const;
			const char* description() const;
			const EnumValue* values() const;
			const EnumValue* findByValue(int value) const;
			int numValues() const;
			void addValue(EnumValue* value);

		private:
			friend class ClassDef;
			EnumDef* mNext;
			EnumValue* mValues;
			const char* mName;
			const char* mDesc;
			int mNumValues;
		};

		class EnumValue
		{
		public:
			EnumValue(const char* id, const char* desc, int value);
			~EnumValue();

			const char* id() const;
			const char* description() const;
			int value() const;
			const EnumValue* next() const { return mNext; }

		private:
			friend class EnumDef;
			EnumValue* mNext;
			const char* mId;
			const char* mDescription;
			int mValue;
		};
	}
}
#endif // ENUMDEF_INCLUDED
