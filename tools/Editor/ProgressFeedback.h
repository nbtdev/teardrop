/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROGRESSFEEDBACK_INCLUDED)
#define PROGRESSFEEDBACK_INCLUDED

namespace Teardrop 
{
	namespace Tools
	{
		class ProgressFeedback
		{
		public:
			ProgressFeedback();
			virtual ~ProgressFeedback();
			virtual void updateProgress(int progress, const char* infoText = 0) = 0;
			virtual void updateTitle(const char* infoText) = 0;
		};
	}
}

#endif // PROGRESSFEEDBACK_INCLUDED
