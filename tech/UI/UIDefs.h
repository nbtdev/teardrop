/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
