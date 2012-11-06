/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
