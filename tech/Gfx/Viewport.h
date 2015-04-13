/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(VIEWPORT_INCLUDED)
#define VIEWPORT_INCLUDED

#include "Gfx/Allocators.h"
#include "Math/Vector2.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class RenderTarget;

		class Viewport
		{
		public:
			Viewport(RenderTarget* rt);
			virtual ~Viewport();

			virtual void updateDimensions();

			/*
				Accessors
			*/
			const Vector2& getPosition(bool normalized=false) const;
			const Vector2& getSize(bool normalized=false) const;
			const RenderTarget* getRenderTarget() const;
			bool isEnabled() const;
			bool getClearEachFrame() const;

			/*
				Mutators
			*/
			void setPosition(const Vector2& pos, bool normalized);
			void setSize(const Vector2& size, bool normalized);
			void translate(const Vector2& pos, bool normalized);
			void enable(bool enabled);
			void setClearEachFrame(bool clear);
			
			TD_DECLARE_ALLOCATOR();

		protected:
			Vector2 mPos;
			Vector2 mSize;
			Vector2 mPosNorm;
			Vector2 mSizeNorm;
			RenderTarget* mRT;
			bool mEnabled;
			bool mClearEachRender;
		};
	}
}

#endif // VIEWPORT_INCLUDED
