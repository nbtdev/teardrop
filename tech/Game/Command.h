/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
		void getExtra(float& val, size_t idx) { val = f[idx]; }
		void getExtra(int& val, size_t idx) { val = i[idx]; }
		void setVerb(Verb verb) { m_verb = verb; }
		Verb getVerb() { return m_verb; }

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
