/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDINPUTFILTER_INCLUDED)
#define TDINPUTFILTER_INCLUDED

namespace Teardrop
{
	union InputEvent;

	/*
		Interface for handling/translating input events
	*/
	class InputFilter
	{
		bool m_bEnabled;
		char pad[3]; // compiler will 4-byte-align this anyway

	public:
		InputFilter();
		virtual ~InputFilter();
		bool isEnabled();
		void enable();
		void disable();

		virtual bool filterEvent(const InputEvent& event) = 0;
	};
}

#endif // TDINPUTFILTER_INCLUDED
