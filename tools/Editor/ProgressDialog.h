/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROGRESSDIALOG_INCLUDED)
#define PROGRESSDIALOG_INCLUDED

#include <QProgressDialog>
#include "ProgressFeedback.h"
#include "FastDelegate.h"

namespace Teardrop 
{
	namespace Tools
	{
		class ProgressDialog 
			: public QProgressDialog
			, public ProgressFeedback
		{
		public:
			ProgressDialog(QWidget* parent=0);
			~ProgressDialog();

			// ProgressFeedback implementation
			void updateProgress(int progress, const char* infoText=0);
		};
	}
}

#endif // PROGRESSDIALOG_INCLUDED
