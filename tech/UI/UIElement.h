/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIELEMENT_INCLUDED)
#define UIELEMENT_INCLUDED

#include "UI/UIDefs.h"
#include "Math/Vector2.h"
#include "Gfx/GfxMeshInstance.h"
#include "Util/RefCounted.h"

namespace Teardrop
{
	namespace UI
	{
		/*
			Element defines the smallest unit of UI rendering that can be
			positioned, sized or rotated independently. This can range from an
			element a single pixel in size, to an element that covers the entire
			screen. Elements are contained within a layer, and layers are composed
			to make up a given UI. Elements can be used on more than one layer at
			a time -- viewport pixel dimensions are calculated on demand at time 
			of render. Note that this means that changes made to the inherent
			size and position of an element are reflected in all layers that use
			the element.
		*/

		class Renderer;
		class IElementNotifications;

		class Element// : public RefCounted
		{
		public:
			Element();
			virtual ~Element();

			//! width and height are in normalized layer coordinates
			virtual void getSize(float& width, float& height, bool& abs) const;
			//! width and height are in normalized layer coordinates
			virtual const Vector2& getSize(bool& abs) const;
			//! x and y are in normalized layer coordinates
			virtual void getPosition(float& x, float& y, bool& abs) const;
			//! returns normalized layer coordinates
			virtual const Vector2& getPosition(bool& abs) const;
			//! used by the UI renderer
			const GfxMeshInstance& getMeshInstance() const { return m_inst; }
			//! get horizontal and vertical alignment types
			HAlign getHAlign() const { return m_hAlign; }
			VAlign getVAlign() const { return m_vAlign; }
			bool isVisible() const { return m_bVisible; }

			//! invoke a code/script function attached to the element
			virtual void call(const char* function, const _variant_t* argv=0, int argc=0) = 0;

			//! width and height are in normalized layer coordinates
			virtual void setSize(float width, float height, bool abs=false);
			//! width and height are in normalized layer coordinates
			virtual void setSize(const Vector2& size, bool abs=false);
			//! x and y are in normalized layer coordinates
			virtual void setPosition(float x, float y, bool abs=false);
			//! takes in normalized layer coordinates
			virtual void setPosition(const Vector2& pos, bool abs=false);
			//! set horizontal and vertical alignment types
			void setAlignment(HAlign hAlign = HALIGN_NOTSET, VAlign vAlign = VALIGN_NOTSET);
			//! called to reset the element dimensions/position
			virtual void onResize(float parentWidth, float parentHeight);
			//! set element notifications (UI actions) callback
			void setNotificationHandler(IElementNotifications* pHandler);
			//! set element visibility (non-visible elements do not render or update)
			void setVisible(bool isVisible) { m_bVisible = isVisible; }
			//! toggle input processing on this element
			void processInput(bool bProcess) { m_bProcessInput = bProcess; }

			/*
				User input
			*/
			virtual bool injectMouseMove(int x, int y) = 0;
			virtual bool injectMouseDown(int button, int x, int y) = 0;
			virtual bool injectMouseUp(int button, int x, int y) = 0;
			virtual bool injectKeyDown(int keyCode, int keyChar) = 0;
			virtual bool injectKeyUp(int keyCode, int keyChar) = 0;
			virtual bool injectKeyEvent(size_t msg, size_t wParam, size_t lParam) = 0;

			//! update the state of the element
			virtual bool update(float deltaT);
			//! render the element 
			virtual bool render(Renderer* pRenderer);

			TD_DECLARE_ALLOCATOR();

		protected:
			IElementNotifications* m_pNotify;
			GfxMeshInstance m_inst;
			Vector2 m_position;
			Vector2 m_positionCache; // used to cache the abs screen pos
			Vector2 m_size;
			size_t m_bAbsPos;
			size_t m_bAbsSize;
			HAlign m_hAlign;
			VAlign m_vAlign;
			bool m_bVisible;
			bool m_bProcessInput;

		private:
			Element(const Element&); // not implemented
			Element& operator=(const Element&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIELEMENT_INCLUDED
