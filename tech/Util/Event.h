/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(EVENT_INCLUDED)
#define EVENT_INCLUDED

#include <functional>
#include <list>

namespace Teardrop
{
	template<typename... Args>
	class Event
	{
	public:
		Event(){}
		~Event(){}

		typedef std::function<void(Args...)> tDelegate;
		void bind(const tDelegate& callback) {
			mDelegates.push_back(callback);
		}

		void unbind(tDelegate callback) {
            for (auto d = mDelegates.begin(); d != mDelegates.end(); ++d) {
                auto f1 = (*d).template target<void(*)(Args...)>();
                auto f2 = callback.template target<void(*)(Args...)>();
                
                if (!f1 || (f1 && f2 && *f1 == *f2)) {
                    d = mDelegates.erase(d);

					// HACK: VC2013 (VC12) doesn't seem to implement the loop properly (and 
					// tries to do the pre-increment above prior to checking the termination 
					// condition). Hackaround: check here (benign on other, working, compilers)
					if (d == mDelegates.end()) break;
                }
            }
		}

        void raise(Args... params) const {
            for (auto d : mDelegates) {
                d(params...);
            }
        }

	protected:
		typedef std::list< tDelegate > Delegates;
		Delegates mDelegates;
	};
}

#endif // EVENT_INCLUDED
 
