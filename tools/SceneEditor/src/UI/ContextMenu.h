/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CONTEXTMENU_INCLUDED)
#define CONTEXTMENU_INCLUDED

/*
	struct ContextMenu

	Provides the definition for a context-sensitive menu. This struct
	typically is defined in static data, hence the lack of constructors 
	and destructors.
*/

struct MenuEntry
{
	const char* label;
	MenuEntry* subMenu;
	size_t id;
	size_t isSeparator;
};

struct ContextMenu
{
	MenuEntry* menu;
};

#endif // CONTEXTMENU_INCLUDED
