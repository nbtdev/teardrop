/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(HKXRIGTOOL_INCLUDED)
#define HKXRIGTOOL_INCLUDED

class hkRootLevelContainer;
class hkxScene;

namespace Teardrop
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
