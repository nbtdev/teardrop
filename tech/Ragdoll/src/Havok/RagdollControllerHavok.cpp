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

#include "stdafx.h"
#include "RagdollControllerHavok.h"
#include "RagdollHavok.h"
#include "Physics/src/Havok/WorldHavok.h"
#include "Physics/include/CollisionFilter.h"
#include "Animation/include/AnimationBlender.h"
#include "Animation/src/Havok/SkeletonInstanceHavok.h"
#include "Resource/include/ResourceManager.h"
#include "Math/include/Transform.h"
#include "Util/include/Environment.h"

using namespace CoS;
//---------------------------------------------------------------------------
RagdollControllerHavok::RagdollControllerHavok()
{
	m_pController = 0;
	m_pInstance = 0;
	m_pUD = 0;
}
//---------------------------------------------------------------------------
RagdollControllerHavok::~RagdollControllerHavok()
{
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::initialize(HResource hRagdoll)
{
	return RagdollController::initialize(hRagdoll);
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::destroy()
{
	delete m_pController;
	return RagdollController::destroy();
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::drive(
	float timeStep, AnimationBlender* pBlender, const Transform& world)
{
	if (timeStep < 0.001)
		return true;

	RagdollHavok* pRagdoll = AutoResPtr<RagdollHavok>(m_hRagdoll);

	if (!pRagdoll)
		return true;

	if (!m_pInstance)
		m_pInstance = pRagdoll->cloneRagdollInstance();

	bool bNeedInitialPose = false;
	if (!m_pController)
	{
		m_pController = new hkaRagdollRigidBodyController(m_pInstance);

		//hkaRagdollUtils::reorderAndAlignForRagdoll(
		//	m_pInstance->m_rigidBodies, m_pInstance->m_constraints);
		//hkUint32 systemGroup = hkpGroupFilter::getSystemGroupFromFilterInfo(
		//	m_pInstance->m_rigidBodies[0]->getCollidable()->getCollisionFilterInfo());
		//hkpGroupFilterUtil::disableCollisionsBetweenConstraintBodies(
		//	m_pInstance->m_constraints.begin(), m_pInstance->m_constraints.getSize(), systemGroup);
		bNeedInitialPose = true;

		for (int i=0; i<m_pInstance->getNumBones(); ++i)
		{
			hkpRigidBody* pRB = m_pInstance->getRigidBodyOfBone(i);
			if (pRB)
			{
				pRB->setMotionType(hkpMotion::MOTION_KEYFRAMED);
				pRB->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(COLLISION_LAYER_RAGDOLL, COLLISION_GROUP_CHARACTER));

				void * pUD = (void *)pRB->getUserData();
				if (pUD != m_pUD)
					pRB->setUserData(hkUlong(m_pUD));
			}
		}
	}

	SkeletonInstanceHavok* pSkel = static_cast<SkeletonInstanceHavok*>(
		pBlender->getSkeletonInstance());
	hkaAnimatedSkeleton* pAnimSkel = pSkel->getAnimatedSkeleton();

	hkaPose animPose(pAnimSkel->getSkeleton());
	hkaPose ragdollPose(m_pInstance->getSkeleton());
	ragdollPose.setToReferencePose();

	hkQsTransform xform;
	xform.setTranslation((hkVector4&)world.trans);
	xform.setRotation((hkQuaternion&)world.rot);
	xform.setScale((hkVector4&)world.scale);

	pAnimSkel->sampleAndCombineAnimations(
		animPose.accessUnsyncedPoseLocalSpace().begin(),
		HK_NULL
		);

	if (bNeedInitialPose)
	{
		pRagdoll->getHighToLowSkeletonMapper()->mapPose(
			animPose.getSyncedPoseModelSpace().begin(),
			m_pInstance->getSkeleton()->m_referencePose.begin(),
			ragdollPose.accessUnsyncedPoseModelSpace().begin(),
			hkaSkeletonMapper::CURRENT_POSE);

		m_pInstance->setPoseModelSpace(ragdollPose.getSyncedPoseModelSpace().begin(), xform);
	}

	//pAnimSkel->sampleAndCombineAnimations(
	//	animPose.accessUnsyncedPoseLocalSpace().begin(),
	//	animPose.getFloatSlotValues().begin()
	//	);

	pRagdoll->getHighToLowSkeletonMapper()->mapPose(
		animPose.getSyncedPoseModelSpace().begin(),
		m_pInstance->getSkeleton()->m_referencePose.begin(),
		ragdollPose.accessUnsyncedPoseModelSpace().begin(),
		hkaSkeletonMapper::CURRENT_POSE);

	hkLocalArray<hkaKeyFrameHierarchyUtility::Output> output(m_pInstance->m_rigidBodies.getSize());
	output.setSize(m_pInstance->m_rigidBodies.getSize());
	//m_pController->driveToPose(
	//	timeStep,
	//	ragdollPose.accessSyncedPoseLocalSpace().begin(),
	//	xform,
	//	output.begin());
	m_pInstance->setPoseModelSpace(
			ragdollPose.accessSyncedPoseModelSpace().begin(),
			xform
		);

	return true;
}
//---------------------------------------------------------------------------
RagdollController* RagdollController::createController()
{
	return COS_NEW RagdollControllerHavok;
}
//---------------------------------------------------------------------------
void RagdollController::destroyController(RagdollController* pController)
{
	delete pController;
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::addToWorld(World* pWorld)
{
	if (!pWorld)
		return false;

	if (!m_pController)
		return false;

	WorldHavok* pHavok = static_cast<WorldHavok*>(pWorld);
	return (HK_SUCCESS == 
		m_pController->getRagdollInstance()->addToWorld(
			pHavok->getHavokWorld(), true));
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::removeFromWorld(World* pWorld)
{
	if (!pWorld)
		return false;

	if (!m_pController)
		return false;

	WorldHavok* pHavok = static_cast<WorldHavok*>(pWorld);
	return (HK_SUCCESS == 
		m_pController->getRagdollInstance()->removeFromWorld());
}
//---------------------------------------------------------------------------
void RagdollControllerHavok::setUserData(void *pC)
{
	// because we usually do not have a live ragdoll controller/instance at
	// the time of this call, we want to cache the user data now and set it
	// on the ragdoll bodies later
	m_pUD = pC;
}
//---------------------------------------------------------------------------
bool RagdollControllerHavok::getDisplayGeometry(DisplayGeometries& geom)
{
	hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
	hkpShapeDisplayBuilder builder(env);

	hkaPose ragdollPose(m_pInstance->getSkeleton());
	ragdollPose.setToReferencePose();

	const hkArray<hkpRigidBody*> &rba = m_pController->getRagdollInstance()->getRigidBodyArray();
	size_t numRB = m_pController->getRagdollInstance()->m_rigidBodies.getSize();

	for (size_t i=0; i<numRB; ++i)
	{
		hkpRigidBody *rb = rba[i];
		const hkpCollidable *pColl = rb->getCollidable();
		const hkpShape *pShape = pColl->getShape();

		hkArray<hkDisplayGeometry*> displayGeom;
		//hkQsTransform xform;
		//m_pController->getRagdollInstance()->getPoseModelSpace(m_pController->getRagdollInstance()->getBoneIndexOfRigidBody(rb), xform);
		
		hkQsTransform &boneXform(ragdollPose.accessUnsyncedPoseModelSpace()[i]);
		hkTransform xform(boneXform.getRotation(), boneXform.getTranslation());
		//xform.setInverse(pColl->getTransform());
		//builder.buildShapeDisplay(pShape, hkTransform::getIdentity(), displayGeom);
		builder.buildShapeDisplay(pShape, xform, displayGeom);
		displayGeom[0]->buildGeometry();

		hkArray<hkVector4> lines;
		displayGeom[0]->getWireframeGeometry(lines);

		if (lines.getSize() == 0)
		{
			// then we have to build the wireframe geom ourselves; do it brute-force
			// and duplicate edges if needed

			hkGeometry* pGeom = displayGeom[0]->m_geometry;
			int numTris = pGeom->m_triangles.getSize();
			int numLines = numTris * 3 * 2;
			lines.setSize(numLines);

			int line = 0;
			for (int t=0; t<numTris; ++t)
			{
				hkGeometry::Triangle& tri = pGeom->m_triangles[t];
				lines[line++] = pGeom->m_vertices[tri.m_a];
				lines[line++] = pGeom->m_vertices[tri.m_b];
				lines[line++] = pGeom->m_vertices[tri.m_b];
				lines[line++] = pGeom->m_vertices[tri.m_c];
				lines[line++] = pGeom->m_vertices[tri.m_c];
				lines[line++] = pGeom->m_vertices[tri.m_a];
			}
		}

		DisplayGeometry dg;
		dg.reserve(lines.getSize());
		dg.resize(lines.getSize());
		geom.push_back(dg);

		hkVector4* pDst = (hkVector4*)&(geom.back()[0]);
		memcpy(pDst, lines.begin(), sizeof(hkVector4) * lines.getSize());

		// yay, thanks Havok -- now I get to go fix your damn .w=0 problem...
		for (int v=0; v<lines.getSize(); ++v)
		{
			hkVector4& vec = *(pDst + v);
			vec(3) = 1;
		}
	}

	return true;
}