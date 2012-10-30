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
#if !defined(RC_INCLUDED)
#define RC_INCLUDED

#include <set>
#include <string>

typedef std::set<std::string> StringSet;

namespace CoS
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
	CoS::Stream& outStrm
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
	CoS::Stream& outStrm
	);

bool doAnim(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	CoS::Stream& outStrm
	);

bool doRigidBody(
	 const char* inputFilename, 
	 const RCParams& params, 
	 const StringSet& options,
	 CoS::Stream& outStrm
	 );

bool doRagdoll(
	 const char* inputFilename, 
	 const RCParams& params, 
	 const StringSet& options,
	 CoS::Stream& outStrm
	 );

#endif // RC_INCLUDED
