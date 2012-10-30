// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

// TODO: reference additional headers your program requires here
#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// display geometry
#include "Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h"
#include "Common/Visualize/Shape/hkDisplayGeometry.h"
#include "Common/Base/Types/Geometry/hkGeometry.h"

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>
#include <Animation/Ragdoll/Utils/hkaRagdollUtils.h>
#include <Animation/Ragdoll/Controller/RigidBody/hkaRagdollRigidBodyController.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

//#include <Common/Base/KeyCode.h>
//#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
//#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
//
//// Register versioning information.
//#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
//#include <Common/Compat/hkCompat_All.cxx>
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"


#include "Memory/include/Memory.h"
#include "Memory/include/MemoryRegion.h"
#include "Stream/include/FileStream.h"
#include "Util/include/Hash.h"
