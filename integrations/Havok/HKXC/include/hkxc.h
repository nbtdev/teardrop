/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/
#if !defined(RC_INCLUDED)
#define RC_INCLUDED

#include <set>
#include <string>

typedef std::set<std::string> StringSet;

namespace Teardrop
{
	class Stream;
}

struct RCParams
{
	RCParams()
	{
		bVerbose = false;
		resid = 0;
	}

	bool bVerbose;
	unsigned __int64 resid;
};

bool doMesh(
	hkRootLevelContainer* container, 
	const RCParams& params, 
	const StringSet& options,
	Teardrop::Stream& outStrm
	);

bool doScene(
	hkRootLevelContainer* container, 
	const RCParams& params, 
	const StringSet& options,
	const char* outFile
	);

bool doRig(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Teardrop::Stream& outStrm
	);

bool doAnim(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Teardrop::Stream& outStrm
	);

bool doRigidBody(
	 const char* inputFilename, 
	 const RCParams& params, 
	 const StringSet& options,
	 Teardrop::Stream& outStrm
	 );

bool doRagdoll(
	 const char* inputFilename, 
	 const RCParams& params, 
	 const StringSet& options,
	 Teardrop::Stream& outStrm
	 );

#endif // RC_INCLUDED
