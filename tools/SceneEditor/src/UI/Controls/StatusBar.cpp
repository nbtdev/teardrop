/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "StatusBar.h"
#include <wx/statusbr.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
StatusBar::StatusBar(wxStatusBar* pStatus)
{
	m_pStatus = pStatus;
}
//---------------------------------------------------------------------------
StatusBar::~StatusBar()
{
}
