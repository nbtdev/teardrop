/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EVENT_INCLUDED)
#define EVENT_INCLUDED

#include "FastDelegate.h"
#include <list>

namespace Teardrop
{
	template< typename T >
	class Event
	{
	public:
		Event(){}
		~Event(){}

		typedef fastdelegate::FastDelegate0<> tDelegate;
		void bind(tDelegate callback)
		{
			mDelegates.push_back(callback);
		}

		void unbind(tDelegate callback)
		{
			Delegates::iterator it = std::find(mDelegates.begin(), mDelegates.end(), callback);
			mDelegates.erase(it);
		}

		void raise() {}

	protected:
		typedef std::list< tDelegate > Delegates;
		Delegates mDelegates;
	};

	template< typename T >
	class Event1
	{
	public:
		Event1(){}
		~Event1(){}

		typedef fastdelegate::FastDelegate1<T> tDelegate;
		void bind(tDelegate callback)
		{
			mDelegates.push_back(callback);
		}

		void unbind(tDelegate callback)
		{
			Delegates::iterator it = std::find(mDelegates.begin(), mDelegates.end(), callback);
			if (it != mDelegates.end())
				mDelegates.erase(it);
		}

		void raise(T p0)
		{
			for (Delegates::iterator it = mDelegates.begin(); it != mDelegates.end(); ++it) {
				if (*it)
					(*it)(p0);
			}
		}

	protected:
		typedef std::list< tDelegate > Delegates;
		Delegates mDelegates;
	};

	template< typename T0, typename T1 >
	class Event2
	{
	public:
		Event2(){}
		~Event2(){}

		typedef fastdelegate::FastDelegate2<T0, T1> tDelegate;
		void bind(tDelegate callback)
		{
			mDelegates.push_back(callback);
		}

		void unbind(tDelegate callback)
		{
			Delegates::iterator it = std::find(mDelegates.begin(), mDelegates.end(), callback);
			mDelegates.erase(it);
		}

	protected:
		typedef std::list< tDelegate > Delegates;
		Delegates mDelegates;
	};

	template< typename T0, typename T1, typename T2 >
	class Event3
	{
	public:
		Event3(){}
		~Event3(){}

		typedef fastdelegate::FastDelegate2<T0, T1, T2> tDelegate;
		void bind(tDelegate callback)
		{
			mDelegates.push_back(callback);
		}

		void unbind(tDelegate callback)
		{
			Delegates::iterator it = std::find(mDelegates.begin(), mDelegates.end(), callback);
			mDelegates.erase(it);
		}

	protected:
		typedef std::list< tDelegate > Delegates;
		Delegates mDelegates;
	};
}

#endif // EVENT_INCLUDED
