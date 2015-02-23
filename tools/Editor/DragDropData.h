/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DRAGDROPDATA_INCLUDED)
#define DRAGDROPDATA_INCLUDED

// Base class for things that can be dragged around the editor
#include <QtCore/QObject>

namespace Teardrop
{
	namespace Tools
	{
		class DragDropData : public QObjectUserData
		{
		public:
			enum Type {
				DDD_PACKAGE,
				DDD_FOLDER,
				DDD_OBJECT,
			};

			DragDropData();
			virtual ~DragDropData();

			virtual Type type() = 0;
		};
	}
}

#endif // DRAGDROPDATA_INCLUDED
