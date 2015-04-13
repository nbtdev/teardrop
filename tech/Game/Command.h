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

#if !defined(COMMAND_INCLUDED)
#define COMMAND_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	/* 
		Currently this class is a bit of a temp hack
		while I figure out how I want to do this in a 
		more generic sort of way
	*/

	class Command
	{
	public:
		enum Verb
		{
			NONE,
			MOVE,
			TURN,
			TWIST,
			PITCH,
			FIRE,
		};

		Command() { m_verb = NONE; }
		virtual ~Command(){}

		void setExtra(float val, size_t idx) { f[idx] = val; }
		void setExtra(int val, size_t idx) { i[idx] = val; }
		void getExtra(float& val, size_t idx) const { val = f[idx]; }
		void getExtra(int& val, size_t idx) const { val = i[idx]; }
		void setVerb(Verb verb) { m_verb = verb; }
		Verb getVerb() const { return m_verb; }

		TD_DECLARE_ALLOCATOR();

	protected:
		Verb m_verb;

		union
		{
			float f[4];
			unsigned int i[4];
		};
	};
}

#endif // COMMAND_INCLUDED
