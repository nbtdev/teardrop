/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ShadowRenderStep.h"
#include "Scene.h"
#include "Component_Render.h"
#include "TerrainPatch.h"
#include "TerrainZone.h"
#include "SpawnPoint.h"
#include "DynamicLight.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxCamera.h"
#include "Gfx/GfxLight.h"
#include "Gfx/GfxRenderTarget.h"
#include "Gfx/GfxRenderTexture.h"
#include "Reflection/Reflection.h"
#include "Reflection/ClassDef.h"
#include "Math/AABB.h"
#include "Math/Matrix44.h"
#include "Math/Vector4.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static void renderShadowTexture(
	GfxRenderer*, Scene*, const ShadowRenderStep::VisibleObjects&, GfxLight*);
//---------------------------------------------------------------------------
ShadowRenderStep::ShadowRenderStep()
{
}
//---------------------------------------------------------------------------
ShadowRenderStep::~ShadowRenderStep()
{
}
//---------------------------------------------------------------------------
void ShadowRenderStep::render(
	const VisibleObjects& objects, GfxRenderer* pRend, Scene* pScene)
{
	// update shadow textures first, if any (naive implementation, btw, just getting it working first)
	Zone* pZone = pScene->getCurrentZone();
	size_t numLights = pZone->getNumShadowCastingLights();

	if (numLights)
		pRend->setRenderMode(GfxRenderer::RENDER_SHADOW_CASTERS);

	for (size_t i=0; i<numLights; ++i)
	{
		DynamicLight* pLight = pZone->getShadowCastingLight(i);
		renderShadowTexture(pRend, pScene, objects, pLight);
	}
}
//---------------------------------------------------------------------------
static void getShadowCastersAABB(
	/*in*/const ShadowRenderStep::VisibleObjects& visibleObjects,
	/*out*/AABB& aabb)
{
	// merge the AABB for everything in the list that is not a terrain patch
	aabb.extents = Vector4::POSITIVE_INFINITY; // indicates invalid AABB
	for (ZoneObjects::const_iterator it = visibleObjects.begin();
		it != visibleObjects.end(); ++it)
	{
		const Reflection::ClassDef* pClassDef = (*it)->getDerivedClassDef();
		if (
			TerrainPatch::getClassDef() == pClassDef
			|| SpawnPoint::getClassDef() == pClassDef
			|| pClassDef->isA(DynamicLight::getClassDef()))
			continue;

		// else, merge this one's AABB 
		aabb.merge((*it)->getBoundingBox());
	}
}
//---------------------------------------------------------------------------
static bool focusCamera(
	const ShadowRenderStep::VisibleObjects& objects, GfxCamera* pCam)
{
	AABB aabb;
	getShadowCastersAABB(objects, aabb);

	// check to see if anything showed up, just in case
	if (aabb.extents.getLength2() < 1)
		return false;

	// otherwise, fixate the camera on the AABB. Assume (for now) an aspect
	// of 1, and that we'll orient the camera towards the center of the line
	// described by the nearest "left" and farthest "right" extents of the AABB
	// in light-cam space
	// for now, just use 90-degree fov and 1 aspect
	pCam->setAspect(1);
	pCam->setFovY(MathUtil::HALF_PI/2);
	pCam->setLookAt(aabb.center);
	pCam->setNearClip(1);
	pCam->setFarClip(100000);
	pCam->update();

	// figure out what depth to use for this, by looking at the AABB from view space
	Matrix44 view(pCam->getViewMatrix());
	Vector4 corners[8];
	aabb.getCorners(corners);

	float nearClip = MathUtil::POSITIVE_INFINITY;
	float farClip = MathUtil::NEGATIVE_INFINITY;
	Vector4 camPos(pCam->getPosition());

	for (int i=0; i<8; ++i)
	{
		Vector4 tmp(view * corners[i]);
		float depth = MathUtil::abs(tmp.z);

		if (depth > farClip)
			farClip = depth;
		if (depth < nearClip)
			nearClip = depth;

		//float distSq = (camPos - corners[i]).getLength2();
		//if (distSq > farClip)
		//	farClip = distSq;
		//if (distSq < nearClip)
		//	nearClip = distSq;
	}

	//pCam->setNearClip(nearClip);
	//pCam->setFarClip(farClip);
	pCam->setNearClip(5);
	pCam->setFarClip(500);
	//pCam->setNearClip(MathUtil::sqrt(nearClip));
	//pCam->setFarClip(MathUtil::sqrt(farClip));
	pCam->update();

	return true;
}
//---------------------------------------------------------------------------
static void renderShadowTexture(
	GfxRenderer* pRend, 
	Scene* pScene, 
	const ShadowRenderStep::VisibleObjects& objects, 
	GfxLight* pLight)
{
	if (!objects.size())
		return;

	// pCam is largely used only to find out if a light/object shadow will
	// intersect this camera's frustum; each light uses its own camera for 
	// rendering its shadow texture

	GfxRenderTarget* pRT = pLight->getShadowTexture();
	if (!pRT)
		return;

	GfxCamera* pShadowCam = pLight->getShadowCamera();
	if (!pShadowCam)
	{
		pShadowCam = TD_NEW GfxCamera;
		pShadowCam->initialize();
		pLight->setShadowCamera(pShadowCam);
	}

	// update existing shadow camera
	pShadowCam->setPosition(pLight->getPosition());

	// get the bounding box of all shadow casters in the scene (non-terrain)
	if (!focusCamera(objects, pShadowCam))
		return;

	GfxRenderTexture* pRTex;
	pRT->queryInterface(GfxRenderTexture::IID, (void**)&pRTex);

	if (pRTex)
	{
		pRend->setColorWrite(true);
		pRend->setRenderTarget(pRT);
		pRend->clearRenderTarget();

		ZoneObjects objs;
		if (pScene->getVisibleObjects(pShadowCam->getFrustumPlanes(), objs))
		{
			pRend->beginScene(pShadowCam, pRTex->getFullTargetViewport());

			// go through the list and make another of things we need to render
			for(ZoneObjects::iterator it = objs.begin();
				it != objs.end(); ++it)
			{
				ZoneObject* pObj = const_cast<ZoneObject*>(*it);
				RenderComponent* pRenderable = 0;
				if (pObj->findComponents(
					RenderComponent::getClassDef(), (Component**)&pRenderable))
				{
					if (pRenderable->getShadowCaster())
						pRenderable->queueForRendering(pRend);
				}
			}

			pRend->endScene();
		}
	}
}