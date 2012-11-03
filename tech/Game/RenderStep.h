/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(RENDERSTEP_INCLUDED)
#define RENDERSTEP_INCLUDED

#include "Memory/Allocators.h"
#include <vector>

namespace CoS
{
	class GfxRenderer;
	class GfxCamera;
	class ZoneObject;
	class Scene;

	class RenderStep
	{
	public:
		typedef std::vector<ZoneObject*> VisibleObjects;

		RenderStep();
		virtual ~RenderStep();

		virtual void render(
			const VisibleObjects& visibleObjects,	// objects visible through the main scene camera
			GfxRenderer* pRenderer,					// the renderer to use 
			Scene* pScene							// the scene we are rendering (some steps may use it for obtaining different sets of visible objects, for instance)
			) = 0;

		virtual void setCamera(GfxCamera* pCamera);

		COS_DECLARE_ALLOCATOR();

	protected:
		GfxCamera* m_pCamera;
	};
}

#endif // RENDERSTEP_INCLUDED
