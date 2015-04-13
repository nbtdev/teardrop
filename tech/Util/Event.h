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
 
