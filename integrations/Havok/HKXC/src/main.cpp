/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <set>
#include <map>
#include <string>
#include <algorithm>

#include "Config.h"
#include "HkxMeshTool.h"
#include "Stream/FileStream.h"
#include "Util/Hash.h"
#include "Gfx/GfxCommon.h"
#include "Physics/PhysicsSystem.h"
#include "Animation/AnimationSystem.h"
#include "Util/_String.h"
#include "Util/Logger.h"

#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include "Common/Serialize/Util/hkSerializeUtil.h"
#include "Common/Serialize/Util/hkLoader.h"
#include "Memory/Memory.h"
#include "Memory/MemoryRegion.h"
#include "Stream/FileStream.h"
#include "Util/Environment.h"
#include "Util/Hash.h"

#include "hkxc.h"

using namespace Teardrop;

static const char* pUsage=
"usage: hkxc <input.hkx> <output.xxx> [flags]\n"
"\n"
"  where 'xxx' is the extension of the file to create; file contents will be\n"
"  determined from this extension. Current valid extensions:\n"
"\n"
"    msh         Mesh asset\n"
"    xml         Scene\n"
"    rig         Animation rig (skeleton)\n"
"    rbd         Rigid body / collision shape\n"
"    rag         Ragdoll\n"
"    anm         Animation\n"
"\n"
"  (Optional) flags appear after filename arguments. Current valid flags:\n"
"\n"
"    GENERAL FLAGS\n"
"    ----------------------------------------------------------------------\n"
"    -v             Verbose output\n"
"\n"
"    MESH FLAGS\n"
"    ----------------------------------------------------------------------\n"
"    --merge        Merge submeshes by material\n"
"\n"
"    SCENE FLAGS\n"
"    ----------------------------------------------------------------------\n"
"    --outputDir=<name>        Name of directory to store meshes\n"
"\n"
;

//---------------------------------------------------------------------------
TD_DEFINE_MEMORY_REGION(DEFAULT, 0, 1024);
TD_ALLOCATOR_IMPL(DEFAULT, CrtAllocator, DEFAULT, -1, -1)
TD_ALLOCATOR_IMPL(CRT, CrtAllocator, DEFAULT, -1, -1)
//---------------------------------------------------------------------------
typedef enum
{
	TYPE_UNKNOWN,
	TYPE_MESH,
	TYPE_SCENE,
	TYPE_RIG,
	TYPE_RIGIDBODY,
	TYPE_ANIM,
	TYPE_RAGDOLL,
} OutputType;
//---------------------------------------------------------------------------
static void errorReportFunction(const char* str, void* errorOutputObject)
{
    printf("%s", str);
}
//---------------------------------------------------------------------------
void usage()
{
	fprintf(stderr, pUsage);
}
//---------------------------------------------------------------------------
char my32kbuf[2 * 1024 * 1024];
//---------------------------------------------------------------------------
static StringSet s_flagsLUT;
static StringSet s_validInputExtensions;
typedef std::map<std::string, OutputType> OutputExtensions;
static OutputExtensions s_validOutputExtensions;
//---------------------------------------------------------------------------
static void getFileExtension(const std::string& filename, std::string& ext)
{
	ext = filename.substr(filename.find_last_of('.') + 1);
}
//---------------------------------------------------------------------------
static bool validateInputFilename(const std::string& filename)
{
	std::string ext;
	getFileExtension(filename, ext);
	int (*pf)(int)=tolower; 
	transform(ext.begin(), ext.end(), ext.begin(), pf); 
	return (s_validInputExtensions.find(ext) != s_validInputExtensions.end());
}
//---------------------------------------------------------------------------
static OutputType validateOutputFilename(const std::string& filename)
{
	std::string ext;
	getFileExtension(filename, ext);
	int (*pf)(int)=tolower; 
	transform(ext.begin(), ext.end(), ext.begin(), pf); 

	OutputExtensions::iterator it = s_validOutputExtensions.find(ext);
	if (it != s_validOutputExtensions.end())
	{
		return it->second;
	}

	return TYPE_UNKNOWN;
}
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	Environment::get().isOffline = 1;
	//Teardrop::setGfxAllocator(GetCRTAllocator());
	Teardrop::String::setAllocator(GetCRTAllocator());

	if (argc < 3)
	{
		usage();
		return 1;
	}

	Teardrop::Integration::Havok::Physics::System physics;
	Teardrop::Integration::Havok::Animation::System animation;
	physics.setAllocator(GetCRTAllocator());
	animation.setAllocator(GetCRTAllocator());

	std::string files[2];

	// scan input for flags
	for (int i=1; i<argc; ++i)
	{
		if (*argv[i] == '-')
		{
			s_flagsLUT.insert(std::string(argv[i]));
		}
		else
		{
			files[i-1] = argv[i];
		}
	}

	s_validInputExtensions.insert("hkt");
	s_validOutputExtensions["msh"] = TYPE_MESH;
	s_validOutputExtensions["xml"] = TYPE_SCENE;
	s_validOutputExtensions["rig"] = TYPE_RIG;
	s_validOutputExtensions["rbd"] = TYPE_RIGIDBODY;
	s_validOutputExtensions["anm"] = TYPE_ANIM;
	s_validOutputExtensions["rag"] = TYPE_RAGDOLL;

	// check file extensions -- first should be hkx, second should be a supported type
	if (!validateInputFilename(files[0]))
	{
		usage(); 
		return 3;
	}

	OutputType outputType = validateOutputFilename(files[1]);
	if (TYPE_UNKNOWN == outputType)
	{
		usage(); 
		return 4;
	}

	RCParams params;
	params.bVerbose = (s_flagsLUT.find("-v") != s_flagsLUT.end());

	physics.initialize();

	{
		hkLoader loader;
		hkRootLevelContainer* container = loader.load(argv[1]);

		switch (outputType)
		{
		case TYPE_MESH:
			{
				// open an output filestream
				FileStream strm;
				if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
				{
					// todo: log it
					return 2;
				}

				params.resid = hashString64(files[1].c_str());

				doMesh(container, params, s_flagsLUT, strm);
				strm.close();
			}
			break;

		case TYPE_SCENE:
			doScene(container, params, s_flagsLUT, files[1].c_str());
			break;

		case TYPE_RIG:
			{
				// open an output filestream
				FileStream strm;
				if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
				{
					// todo: log it
					return 2;
				}

				params.resid = hashString64(files[1].c_str());

				doRig(files[0].c_str(), params, s_flagsLUT, strm);
				strm.close();
			}
			break;

		case TYPE_RIGIDBODY:
			{
				// open an output filestream
				FileStream strm;
				if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
				{
					// todo: log it
					return 2;
				}

				params.resid = hashString64(files[1].c_str());

				doRigidBody(files[0].c_str(), params, s_flagsLUT, strm);
				strm.close();
			}
			break;

		case TYPE_ANIM:
			{
				// open an output filestream
				FileStream strm;
				if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
				{
					// todo: log it
					return 2;
				}

				params.resid = hashString64(files[1].c_str());

				doAnim(files[0].c_str(), params, s_flagsLUT, strm);
				strm.close();
			}
			break;

		case TYPE_RAGDOLL:
			{
				// open an output filestream
				FileStream strm;
				if (!strm.open(files[1].c_str(), WRITE|TRUNCATE))
				{
					// todo: log it
					return 2;
				}

				params.resid = hashString64(files[1].c_str());

				doRagdoll(files[0].c_str(), params, s_flagsLUT, strm);
				strm.close();
			}
			break;
		}
	}

	physics.shutdown();
	return 0;
}