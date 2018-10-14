/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(TDHKXPRUNE_INCLUDED)
#define TDHKXPRUNE_INCLUDED

class hkRootLevelContainer;

namespace Teardrop
{
	class HkxPrune
	{
		hkRootLevelContainer* m_pHkx;

	public:
		//! normal c'tor (cannot fail)
		HkxPrune();
		//! d'tor (cannot fail)
		~HkxPrune();

		typedef enum
		{
			NONE				= 0,	// prune nothing
			ENVIRONMENTDATA		= 1,	// prune everything under top-level Environment node (inclusive)
			SCENEDATA			= 2,	// prune everything under top-level SceneData node (inclusive)
		} TypeMask;

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize(hkRootLevelContainer* container);
		//! destroy
		bool destroy();
		//! prune the provided types; will copy everything but what's in the mask, to dest
		bool prune(hkRootLevelContainer& dest, size_t mask);
		
		/** accessors
		*/

		/** mutators
		*/

	private:
		//! NOT IMPLEMENTED
		HkxPrune(const HkxPrune& other);
		HkxPrune& operator=(const HkxPrune& other);
	};
}

#endif // TDHKXPRUNE_INCLUDED
