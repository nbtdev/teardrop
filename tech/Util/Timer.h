/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDTIMER_INCLUDED)
#define TDTIMER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class Timer
	{
		struct Impl;
		Impl* m_pImpl;
		float m_elapsed;

	public:
		Timer();
		~Timer();

		float getCurrentTime() const;
		float getElapsedTime() const { return m_elapsed; }
		float saveElapsedTime(); // resets "lastTime" value
		float reset(); // resets all to zero, returns previous current time
		void update(); // advances current time

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDENVIRONMENT_INCLUDED
