/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TYPECHOOSER_INCLUDED)
#define TYPECHOOSER_INCLUDED

#include <QtWidgets/QDialog>

class QComboBox;
class QDialogButtonBox;

namespace Teardrop 
{
	namespace Reflection
	{
		class ClassDef;
	}

	namespace Tools
	{
		class TypeChooser : public QDialog
		{
		public:
			TypeChooser(QWidget* parent, const Reflection::ClassDef* baseClass);
			~TypeChooser();

			const Reflection::ClassDef* chosenClass();
			void accept();

		protected:
			const Reflection::ClassDef* mBaseClass;
			const Reflection::ClassDef* mChosenClass;
			QComboBox* mTypeList;
			QDialogButtonBox* mButtonBox;
		};
	}
}

#endif // TYPECHOOSER_INCLUDED
