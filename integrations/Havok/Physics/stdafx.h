// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>


// TODO: reference additional headers your program requires here
#include "Memory/Memory.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/Allocator/hkMemoryAllocator.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>

// Register Havok classes.
#include <Common/Base/KeyCode.h>
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#include <Common/Base/keycode.cxx>

#include "Common/Base/hkBase.h"
#include "Common/Base/Memory/Allocator/hkMemoryAllocator.h"
#include "Common/Base/Memory/System/Util/hkMemoryInitUtil.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Compat/Deprecated/hkSerializeDeprecated.h>
#include "Physics/Dynamics/World/hkpWorld.h"
#include "Physics/Dynamics/World/hkpWorldCInfo.h"
#include "Physics/Dynamics/World/hkpPhysicsSystem.h"
#include "Physics/Dynamics/Entity/hkpRigidBody.h"
#include "Physics/Dynamics/Entity/hkpRigidBodyCInfo.h"
#include "Physics/Collide/Dispatch/hkpAgentRegisterUtil.h"
#include "Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h"
#include "Physics/Collide/Filter/Group/hkpGroupFilter.h"
#include "Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h"
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"

// display geometry
#include "Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h"
#include "Common/Visualize/Shape/hkDisplayGeometry.h"
#include "Common/Base/Types/Geometry/hkGeometry.h"

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>

// multithreaded step
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>