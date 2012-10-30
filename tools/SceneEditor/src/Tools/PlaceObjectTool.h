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

#if !defined(PLACEOBJECTTOOL_INCLUDED)
#define PLACEOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Gfx/include/GfxMeshInstance.h"
#include <vector>

/*
	class PlaceObjectTool

	Tool used to place an object in the scene.
*/

class PlaceObjectTool
	: public Tool
{

public:
	PlaceObjectTool();
	virtual ~PlaceObjectTool();

	// Tool implementation
	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const CoS::InputEvent& event, Data* pData);
	size_t getNumRenderables();
	bool getRenderable(/*out*/CoS::GfxMeshInstance& inst, /*in*/size_t index);
	void activate();
	void deactivate();

	// finds the actual intersection of the ray with the geometry (at the polygon level)
	bool findIntersection(
		/*in*/const CoS::Ray& ray,				// the ray with which to collide
		/*in*/const CoS::ZoneObject* pSelected, // the world geometry piece with which to intersect
		/*out*/CoS::Vector4& intersection		// on successful return, contains the exact intersection point
		);
	// place a copy of the currently selected object template at the indicated point
	void placeObject(
		/*in*/CoS::Reflection::Object* pTempl, // object template to clone and place
		/*in*/CoS::Scene* pScene,				// Scene in which to place the object
		/*in*/const CoS::Vector4& pos			// where to put the object
		);

	COS_DECLARE_ALLOCATOR();
protected:
};

#endif // PLACEOBJECTTOOL_INCLUDED
