/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
