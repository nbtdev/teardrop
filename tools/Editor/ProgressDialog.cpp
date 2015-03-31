/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "ProgressDialog.h"

using namespace Teardrop;
using namespace Tools;

ProgressDialog::ProgressDialog(QWidget* parent/* =0 */)
	: QProgressDialog(parent)
{
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::updateProgress(int progress, const char* infoText)
{
	setValue(progress);

	if (infoText)
		setLabelText(infoText);
	else
		setLabelText(QString());
}

void ProgressDialog::updateTitle(const char* infoText)
{
	if (infoText)
		setLabelText(infoText);
}
