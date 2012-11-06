/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PLACEOBJECTTOOL_INCLUDED)
#define PLACEOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Gfx/GfxMeshInstance.h"
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
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	size_t getNumRenderables();
	bool getRenderable(/*out*/Teardrop::GfxMeshInstance& inst, /*in*/size_t index);
	void activate();
	void deactivate();

	// finds the actual intersection of the ray with the geometry (at the polygon level)
	bool findIntersection(
		/*in*/const Teardrop::Ray& ray,				// the ray with which to collide
		/*in*/const Teardrop::ZoneObject* pSelected, // the world geometry piece with which to intersect
		/*out*/Teardrop::Vector4& intersection		// on successful return, contains the exact intersection point
		);
	// place a copy of the currently selected object template at the indicated point
	void placeObject(
		/*in*/Teardrop::Reflection::Object* pTempl, // object template to clone and place
		/*in*/Teardrop::Scene* pScene,				// Scene in which to place the object
		/*in*/const Teardrop::Vector4& pos			// where to put the object
		);

	TD_DECLARE_ALLOCATOR();
protected:
};

#endif // PLACEOBJECTTOOL_INCLUDED
