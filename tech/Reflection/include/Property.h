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

#if !defined (PROPERTY_INCLUDED)
#define PROPERTY_INCLUDED

#include "Util/include/StringUtil.h"
#include "Util/include/_String.h"
#include <vector>
#include <assert.h>

namespace CoS
{
	namespace Reflection
	{
		class Object;

		/*
			Base templated property definition class. This is where the actual
			field is declared inside the containing class (via the PROPERTY family
			of macros).
		*/
		template <class _T>
		class PropertyDefImpl
		{
		public:
			PropertyDefImpl()
			{
			}

			~PropertyDefImpl()
			{
			}

			void toString(String& sVal)
			{
				StringUtil::toString(m_val, sVal);
			}

			void fromString(const String& sVal)
			{
				StringUtil::fromString(sVal, m_val);
			}

			operator _T& () { return m_val; }
			operator const _T& () const { return m_val; }
			_T& operator=(const _T& other) { m_val = other.m_val; return m_val; }
			bool operator==(_T& other) { return *this == other; }
			bool operator==(const _T& other) const { return m_val == other; }
			bool operator!=(_T& other) { return !(*this == other); }
			bool operator!=(const _T& other) const { return !(*this == other); }
			_T& operator=(_T& other) { m_val = other; return *this; }
			_T& get() { return m_val; }
			void set(_T val) { m_val = val; }

		protected:
			_T m_val;
		};


		/*
			Base templated pointer property definition class. This is where the actual
			pointer field is declared inside the containing class (via the PROPERTY family
			of macros). 
		*/
		template<class T>
		class PointerPropertyType
		{
			T* m_p;

		public:
			PointerPropertyType()
			{
				m_p = 0;
			}

			PointerPropertyType(T* pP)
			{
				m_p = pP;
			}

			PointerPropertyType(const PointerPropertyType<T>& other)
			{
				*this = other;
			}

			T* operator=(T* other)
			{
				m_p = other;
				return m_p;
			}

			T& operator=(T& other)
			{
				m_p = other.m_p;
				return *this;
			}

			operator T*()
			{
				return m_p;
			}

			operator T const *()
			{
				return m_p;
			}

			T* operator->()
			{
				return m_p;
			}

			bool isNull()
			{
				return m_p == 0;
			}

			T& operator*()
			{
				assert(m_p);
				return *m_p;
			}
		};

		class CollectionPropertyBase
		{
		public:
			virtual bool setAtFromString(size_t index, const String& strVal) = 0;
			virtual void resize(size_t count) = 0;
		};

		template <class T>
		class CollectionPropertyType : public CollectionPropertyBase
		{
		public:
			typedef std::vector<T> ContainerType;
		private:
			ContainerType _coll;

		public:
			CollectionPropertyType()
			{
			}

			ContainerType& getContainer()
			{
				return _coll;
			}

			void add(const T& val)
			{
				_coll.push_back(val);
			}

			void clear()
			{
				_coll.clear();
			}

			size_t getCount()
			{
				return _coll.size();
			}

			size_t getCapacity()
			{
				return _coll.capacity();
			}

			void resize(size_t count)
			{
				_coll.resize(count);
			}

			void reserve(size_t count)
			{
				_coll.reserve(count);
			}

			bool getAt(size_t index, T& /*inout*/ val)
			{
				assert(index < _coll.size());
				if (index >= _coll.size())
					return false;

				val = _coll[index];
				return true;
			}

			bool setAt(size_t index, const T& /*in*/ val)
			{
				assert(index < _coll.capacity());
				if (index >= _coll.capacity())
					return false;

				_coll[index] = val;
				return true;
			}

			bool setAtFromString(size_t index, const String& strVal)
			{
				T val;
				StringUtil::fromString(strVal, val);
				return setAt(index, val);
			}
		};

		template<typename T>
		class PointerCollectionPropertyType : public CollectionPropertyType<T*>
		{
		public:
			bool setAtFromString(size_t /*index*/, const String& /*strVal*/)
			{
				return false;
			}
		};
	} // namespace Reflection
} // namespace CoS

#endif // PROPERTY_INCLUDED
