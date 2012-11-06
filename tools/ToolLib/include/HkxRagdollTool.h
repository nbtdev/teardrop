/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
