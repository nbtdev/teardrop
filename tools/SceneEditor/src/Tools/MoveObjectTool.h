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

#if !defined(MOVEOBJECTTOOL_INCLUDED)
#define MOVEOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Math/Vector4.h"

namespace CoS
{
	class TransformGizmo;
}

/*
	class MoveObjectTool

	Tool used to move an object in the scene, usually with a visible
	and co-located active transform gizmo.
*/

class MoveObjectTool
	: public Tool
{
	CoS::TransformGizmo& m_gizmo;
	CoS::Vector4 m_transScaleStart;
	CoS::Vector4 m_transOriginal;
	bool m_bLMB;
	bool m_bMoving;

public:
	MoveObjectTool(CoS::TransformGizmo& pGizmo);
	virtual ~MoveObjectTool();

	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const CoS::InputEvent& event, Data* pData);
	size_t getNumRenderables();
	bool getRenderable(/*out*/CoS::GfxMeshInstance& inst, /*in*/size_t index);
	void activate();
	void deactivate();

	COS_DECLARE_ALLOCATOR();

protected:
	void _onViewChanged();
	void selectedObjectChanged(void* sender, CoS::Reflection::Object* pObj);
	void translate(int rx, int ry);
};

#endif // MOVEOBJECTTOOL_INCLUDED