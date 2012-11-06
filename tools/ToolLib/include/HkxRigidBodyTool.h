/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HKXRIGIDBODYTOOL_INCLUDED)
#define HKXRIGIDBODYTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
{
	class Body;
	class Stream;

	struct HkxRigidBodyToolParams
	{
		HkxRigidBodyToolParams()
		{
			bVerbose = false;
		}

		bool bVerbose;
	};

	class HkxRigidBodyTool
	{
		const HkxRigidBodyToolParams& m_params;

		// processing statistics

	public:
		//! normal c'tor (cannot fail)
		HkxRigidBodyTool(const HkxRigidBodyToolParams& params);
		//! d'tor (cannot fail)
		~HkxRigidBodyTool();

		//! initialize (anything that can fail), with specified index count and size and optional buffer pointer
		bool initialize();
		//! destroy
		bool destroy();
		//! process using the existing settings
		bool process(Body& rig, Stream& src);
		
		/** accessors
		*/

		/** mutators
		*/

	private:

		//! NOT IMPLEMENTED
		HkxRigidBodyTool(const HkxRigidBodyTool& other);
		HkxRigidBodyTool& operator=(const HkxRigidBodyTool& other);
	};
}

#endif // HKXRIGIDBODYTOOL_INCLUDED
