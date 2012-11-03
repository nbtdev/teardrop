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
#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Animation/hkaAnimationBinding.h>
#include <Common/Base/Types/Geometry/LocalFrame/hkLocalFrame.h>

// Vertex Deformation
#include <Animation/Animation/Deform/Skinning/hkaMeshBinding.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Animation/Animation/Rig/hkaPose.h>

//#include <Common/Base/KeyCode.h>
//#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkKeyCodeClasses.h>
//#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
//
//// Register versioning information.
//#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
//#include <Common/Compat/hkCompat_All.cxx>

#include "Memory/include/Memory.h"
#include "Memory/include/MemoryRegion.h"
#include "Stream/include/FileStream.h"
#include "Util/include/Hash.h"
