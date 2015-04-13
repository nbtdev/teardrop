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

#if !defined(HKXANIMTOOL_INCLUDED)
#define HKXANIMTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
{
	class Animation;
	class Stream;

	struct HkxAnimToolParams
	{
		HkxAnimToolParams()
		{
			bVerbose = false;
		}

		bool bVerbose;
	};

	class HkxAnimTool
	{
		const HkxAnimToolParams& m_params;

		// processing statistics

	public:
		//! normal c'tor (cannot fail)
		HkxAnimTool(const HkxAnimToolParams& params);
		//! d'tor (cannot fail)
		~HkxAnimTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize();
		//! destroy
		bool destroy();
		//! process using the existing settings
		bool process(Animation& anim, Stream& src);
		
		/** accessors
		*/

		/** mutators
		*/

	private:

		//! NOT IMPLEMENTED
		HkxAnimTool(const HkxAnimTool& other);
		HkxAnimTool& operator=(const HkxAnimTool& other);
	};
}

#endif // HKXANIMTOOL_INCLUDED
