/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
