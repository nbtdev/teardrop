/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIDEFS_INCLUDED)
#define UIDEFS_INCLUDED

namespace Teardrop
{
	namespace UI
	{
		enum ElementType
		{
			ET_FLASH,
			ET_BASIC,
		};

		enum HAlign
		{
			HALIGN_NOTSET,
			HALIGN_LEFT,
			HALIGN_CENTER,
			HALIGN_RIGHT,
		};

		enum VAlign
		{
			VALIGN_NOTSET,
			VALIGN_TOP,
			VALIGN_MIDDLE,
			VALIGN_BOTTOM,
		};

		/*
			UI element creation parameter block; this is provided on the stack
			so the pointers contained within are valid only during
			the creation processing.
		*/
		struct ElementParams
		{
			ElementType type;
			const char* elementName;	// user-defined
			const char* resourceName;	// generally used by Flash/SWF types
			const char* textureName;	// generally used by Basic types
			HAlign hAlign;				// horizontal alignment, if applicable
			VAlign vAlign;				// vertical alignment, if applicable
			float x;					// element position (normalized), if applicable
			float y;					//
			float w;					// element size (normalized), if applicable
			float h;					//
			float u;					// element U texture coordinate, if applicable
			float v;					// element V texture coordinate, if applicable
		};

		//! callback interface for element notifications (user actions, etc)
		class IElementNotifications
		{
		public:
			virtual void onCommand(const char* cmd, const char* args) = 0;
			virtual void onCommand(const char* cmd) = 0;
		};

	} // namespace UI
} // namespace Teardrop

#endif // UIDEFS_INCLUDED
