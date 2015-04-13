/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#if !defined(HKXRAGDOLLTOOL_INCLUDED)
#define HKXRAGDOLLTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
{
	class Ragdoll;
	class Stream;

	struct HkxRagdollToolParams
	{
		HkxRagdollToolParams()
		{
			bVerbose = false;
		}

		bool bVerbose;
	};

	class HkxRagdollTool
	{
		const HkxRagdollToolParams& m_params;

		// processing statistics

	public:
		//! normal c'tor (cannot fail)
		HkxRagdollTool(const HkxRagdollToolParams& params);
		//! d'tor (cannot fail)
		~HkxRagdollTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize();
		//! destroy
		bool destroy();
		//! process using the existing settings
		bool process(Ragdoll& rig, Stream& src);
		
		/** accessors
		*/

		/** mutators
		*/

	private:

		//! NOT IMPLEMENTED
		HkxRagdollTool(const HkxRagdollTool& other);
		HkxRagdollTool& operator=(const HkxRagdollTool& other);
	};
}

#endif // HKXRAGDOLLTOOL_INCLUDED
