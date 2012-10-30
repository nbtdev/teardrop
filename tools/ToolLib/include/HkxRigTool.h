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

#if !defined(HKXRIGTOOL_INCLUDED)
#define HKXRIGTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace CoS
{
	class Rig;
	class Stream;

	struct HkxRigToolParams
	{
		HkxRigToolParams()
		{
			bVerbose = false;
		}

		bool bVerbose;
	};

	class HkxRigTool
	{
		const HkxRigToolParams& m_params;

		// processing statistics

	public:
		//! normal c'tor (cannot fail)
		HkxRigTool(const HkxRigToolParams& params);
		//! d'tor (cannot fail)
		~HkxRigTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize();
		//! destroy
		bool destroy();
		//! process using the existing settings
		bool process(Rig& rig, Stream& src);
		
		/** accessors
		*/

		/** mutators
		*/

	private:

		//! NOT IMPLEMENTED
		HkxRigTool(const HkxRigTool& other);
		HkxRigTool& operator=(const HkxRigTool& other);
	};
}

#endif // HKXRIGTOOL_INCLUDED
