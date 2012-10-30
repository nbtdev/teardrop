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

#include "Config.h"
#include "ToolLib/include/HkxRigTool.h"
#include "Animation/src/Havok/RigHavok.h"
#include "Serialization/include/ResourceSerializer.h"
#include "Stream/include/FileStream.h"
#include "hkxc.h"

using namespace CoS;

//---------------------------------------------------------------------------
bool doRig(
	const char* inputFilename, 
	const RCParams& params, 
	const StringSet& options,
	Stream& outStrm)
{
	HkxRigToolParams rigParams;
	rigParams.bVerbose = params.bVerbose;

	HkxRigTool rigTool(rigParams);
	rigTool.initialize();

	FileStream fs;
	if (!fs.open(inputFilename, READ|BINARY))
	{
		// TODO: log it?
		return false;
	}

	RigHavok rig;
	if (rigTool.process(rig, fs))
	{
		// save out the mesh file
		ResourceSerializer ser(outStrm);
		unsigned __int64 id = params.resid;
		ser.setId(id);
		rig.serialize(ser);
	}
	rig.destroy();
	rigTool.destroy();

	return true;
}
