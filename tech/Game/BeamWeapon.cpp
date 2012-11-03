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

#include "BeamWeapon.h"
#include "Component_Render.h"
#include "Component_Audio.h"
#include "Component_EquipmentSlot.h"
#include "Gfx/GfxMesh.h"
#include "Gfx/GfxSubMesh.h"
#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxIndexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Gfx/GfxMaterial.h"
#include "Gfx/GfxShaderConstantTable.h"
#include "Gfx/GfxShaderConstant.h"
#include "Gfx/GfxUtil.h"
#include "Math/Matrix44.h"
#include "Math/MathUtil.h"
#include "Util/Environment.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(BeamWeapon);
//---------------------------------------------------------------------------
BeamWeapon::BeamWeapon()
{
	m_pMountTransformConstant = 0;
	m_animationTimeRemaining = 0;
	m_recycleTimeRemaining = 0;
}
//---------------------------------------------------------------------------
BeamWeapon::~BeamWeapon()
{
}
//---------------------------------------------------------------------------
bool BeamWeapon::_initialize()
{
	// TODO: do this a different way
	if (Environment::get().isServer)
		return true;

	m_inst.m_pProceduralMesh = COS_NEW GfxMesh;
	m_inst.m_pProceduralMesh->initialize();

	GfxSubMesh* pSubmesh = m_inst.m_pProceduralMesh->createSubMesh();
	pSubmesh->setPrimitiveType(TRISTRIP);

	// beam vertex format: positions and single set of texcoord
	GfxVertexFormat fmt;
	GfxVertexFormat::Element elem;
	elem.setSource(0);
	elem.setUsage(0);
	elem.offset = 0;
	elem.semantic = POSITION;
	elem.type = FLOAT3;
	fmt.addElement(elem);
	elem.semantic = TEXCOORD;
	elem.type = FLOAT2;
	elem.offset = (unsigned char)GfxUtil::getSizeOf(FLOAT3);
	fmt.addElement(elem);

	// beam is in the shape of a cylinder, 18 quads (36 tris) total in a
	// tristrip, for a total of 38 verts
	m_pVertexData = COS_NEW Vertex[38];
	Vertex* pData = m_pVertexData;
	// organize the vertex data into a cylinder, around the Z axis,
	// 18 faces (so step around a circle by 20 degrees). Cylinder is
	// 1 unit in length (length is along the +Z axis), with a radius
	// of 0.1
	float oo360 = 1.f / 360.f;
	for (float d=360; d>=0; d-=20)
	{
		float i = d * oo360;
		float r = i * MathUtil::PI;
		float s = MathUtil::sin(r);
		float c = MathUtil::cos(r);

		float x = c * 0.1f;
		float y = s * 0.1f;

		// "near" point
		pData->pz = 0;
		pData->px = x;
		pData->py = y;
		pData->tx = i;
		pData->ty = 0;
		pData++;

		// "far" point
		pData->pz = -float(getRange());
		pData->px = x;
		pData->py = y;
		pData->tx = i;
		pData->ty = 1;
		pData++;
	}

	size_t stream;
	pSubmesh->createVertexData(
		stream,
		Environment::get(),
		fmt.getVertexSize(),
		38,
		(GfxVertexData::CreationFlags)(GfxVertexData::STATIC|GfxVertexData::WRITE_ONLY),
		m_pVertexData);

	pSubmesh->setVertexFormat(Environment::get(), fmt);

	// make a basic material for this mesh
	GfxMaterial* pMtl = COS_NEW GfxMaterial;
	pMtl->initialize();
	pSubmesh->setMaterial(pMtl, true);
	pMtl->setEmissive(GfxUtil::makePackedColor(0,1,0,1));

	// set up shader constant(s) on the RenderComponent
	RenderComponent* pComp;
	if (findComponents(RenderComponent::getClassDef(), (Component**)&pComp))
	{
		GfxShaderConstantTable& constants = pComp->getShaderConstants();
		constants.begin();
		m_pMountTransformConstant = constants.addFloatConstant("mountTransform", 4);
		constants.end();
	}
	
	return true;
}
//---------------------------------------------------------------------------
bool BeamWeapon::_destroy()
{
	if (m_inst.m_pProceduralMesh)
	{
		m_inst.m_pProceduralMesh->destroy();
		delete m_inst.m_pProceduralMesh;
		m_inst.m_pProceduralMesh = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool BeamWeapon::_update(float deltaT)
{
	m_recycleTimeRemaining -= deltaT;
	m_animationTimeRemaining -= deltaT;

	// do double-duty here -- update our components and at the same time, look
	// for a RenderComponent
	RenderComponent* pComp=0;
	for (Components::iterator it = m_components.begin(); 
		it != m_components.end(); ++it)
	{
		if (it->second->getDerivedClassDef() == RenderComponent::getClassDef())
		{
			pComp = static_cast<RenderComponent*>(it->second);
			// turn off rendering if we are below zero in anim time remaining
			pComp->setEnabled(m_animationTimeRemaining > 0);
		}

		// update component
		it->second->update(deltaT);
	}

	if (m_animationTimeRemaining < 0 && m_recycleTimeRemaining < 0)
	{
		// early-out, we are entirely idle
		return true;
	}

	if (m_pMountTransformConstant)
	{
		const Matrix44& xform = m_pSlot->getTransform();

		Matrix44 tmp;
		xform.transpose(tmp);

		// update shader constant(s) on the RenderComponent
		m_pMountTransformConstant->setValue((Vector4*)&tmp);
	}

	if (pComp)
	{
		m_inst.setTransform(getTransformWS());
		pComp->setMeshInstance(m_inst);
	}

	return true;
}
//---------------------------------------------------------------------------
void BeamWeapon::fire()
{
	// only bother if we are recycled and ready
	if (m_recycleTimeRemaining < 0)
	{
		// then we can turn on rendering by restarting these timers
		m_recycleTimeRemaining = getRecycleTime();
		m_animationTimeRemaining = getAnimationTime();

		// and then play the internal fire sfx
		AudioComponent* pAudio;
		if (findComponents(AudioComponent::getClassDef(), (Component**)&pAudio))
		{
			// play the fire sound
			pAudio->play2D(getFireSfx_Internal());
			// and also line up the recycle sound
			pAudio->play2D(getRecycleSfx(), getRecycleTime(), true);
		}
	}
}
