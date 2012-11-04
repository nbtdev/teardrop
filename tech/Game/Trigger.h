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

#if !defined(TRIGGER_INCLUDED)
#define TRIGGER_INCLUDED

#include "FastDelegate.h"

namespace CoS
{
	/*
		Abstract base class for all Trigger types. Triggers start "disabled"
		so the caller needs to enable them manually.
	*/
	class Trigger
	{
	public:
		Trigger();
		virtual ~Trigger();

		fastdelegate::FastDelegate1<Trigger*> Fire;

		//! returns false if the trigger is disabled~
		virtual bool update(float deltaT) = 0;

		void setEnabled(bool bEnable) { m_enabled = bEnable; }

	protected:
		bool m_enabled;
	};
}

#endif // TRIGGER_INCLUDED