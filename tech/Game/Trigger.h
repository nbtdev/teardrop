/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TRIGGER_INCLUDED)
#define TRIGGER_INCLUDED

#include "FastDelegate.h"

namespace Teardrop
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
