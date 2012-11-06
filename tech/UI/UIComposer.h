/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UICOMPOSER_INCLUDED)
#define UICOMPOSER_INCLUDED

#include "Memory/Memory.h"
#include <map>

namespace Teardrop
{
	namespace UI
	{
		/*
			Composer is an ordered group of Layer instances, managing 
			the Z-order of the layers in controls. Composer controls the 
			lifetime of the Layer instances within. By default, Layers are
			rendered bottom-to-top, but (in the future) the caller can 
			override this behavior for more fine-grained control.
		*/

		class Layer;
		class Renderer;

		class Composer
		{
		public:
			Composer();
			virtual ~Composer();

			enum LayerRenderOrder
			{
				LRO_BOTTOMTOTOP=0, // default
				LRO_TOPTOBOTTOM,
			};

			/*! add a new layer to the composer; zOrder defaults to "on top of all others".
			*/
			Layer* addLayer(int zOrder = -1);

			/*! remove (and delete) a layer from the composer; decrements refcount on
				Elements within the layer, so if you otherwise need an Element to live
				after this Layer is destroyed, you need to have another reference on
				the Element prior to calling this method.
			*/
			void removeLayer(Layer* pLayer);
			
			//! remove all layers (clears the composer)
			void clear();

			//! get the render ordering for layers in this composer
			LayerRenderOrder getRenderOrder() const;

			//! set the render ordering for layers in this composer
			void setRenderOrder(LayerRenderOrder order);

			//! update the composer
			virtual bool update(float deltaT);
			//! render the composer
			virtual bool render(Renderer* pRenderer);
			//! absolute (pixel) dimensions
			void resize(float width, float height);

			/*
				User input
			*/
			bool injectMouseMove(int x, int y);
			bool injectMouseDown(int button, int x, int y);
			bool injectMouseUp(int button, int x, int y);
			bool injectKeyDown(int keyCode, int keyChar);
			bool injectKeyUp(int keyCode, int keyChar);
			bool injectKeyEvent(size_t msg, size_t wParam, size_t lParam);

			TD_DECLARE_ALLOCATOR();

		protected:
			typedef std::multimap<size_t, Layer*> Layers;
			Layers m_layers;
			LayerRenderOrder m_renderOrder;

		private:
			Composer(const Composer&); // not implemented
			Composer& operator=(const Composer&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UICOMPOSER_INCLUDED
