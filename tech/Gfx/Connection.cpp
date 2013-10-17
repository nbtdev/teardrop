/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Connection.h"
#include "MaterialExpression.h"
#include "Material.h"

using namespace Teardrop;
using namespace Gfx;

TD_CLASS_IMPL(Connection);

Connection::Connection()
	: mInput(0)
	, mOutput(0)
{

}

Connection::~Connection()
{
}

bool Connection::initialize()
{
	MaterialExpression* expr = getFromExpression();
	if (expr) {
		mOutput = expr->findOutputAttribute(getFromAttribute());
	}

	expr = getToExpression();
	if (expr) {
		mInput = expr->findInputAttribute(getToAttribute());
	}

	// attach ourselves to our "parent" Material
	Material* mtl = getParent();
	if (mtl) {
		mtl->addConnection(this);
	}

	return true;
}

Attribute* Connection::input()
{
	return mInput;
}

Attribute* Connection::output()
{
	return mOutput;
}
