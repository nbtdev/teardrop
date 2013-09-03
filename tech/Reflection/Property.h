/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined (PROPERTY_INCLUDED)
#define PROPERTY_INCLUDED

#include "Util/StringUtil.h"
#include "Util/_String.h"
#include <vector>
#include <assert.h>

namespace Teardrop
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
			_T& operator=(_T& other) { m_val = other.m_val; return m_val; }
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

		class PointerPropertyBase
		{
		public:
			virtual bool isNull() = 0;
		};

		template<class T>
		class PointerPropertyType : public PointerPropertyBase
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
} // namespace Teardrop

#endif // PROPERTY_INCLUDED
