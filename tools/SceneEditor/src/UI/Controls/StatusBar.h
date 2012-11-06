/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STATUSBAR_INCLUDED)
#define STATUSBAR_INCLUDED

#include "FastDelegate.h"

/*
	Wrapper class for the underlying windowing system statusbar implementation;
	it's OK if it exposes whatever the underlying system is, the rest of the 
	editor app doesn't deal with those available APIs
*/

class wxStatusBar;

class StatusBar
{
	wxStatusBar* m_pStatus;

public:
	StatusBar(wxStatusBar* statusBar);
	~StatusBar();
};

#endif // STATUSBAR_INCLUDED
