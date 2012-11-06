/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ObjectTreeNode.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ObjectTreeNode::ObjectTreeNode(Reflection::Object* pObj)
{
	m_pObject = pObj;
}
//---------------------------------------------------------------------------
ObjectTreeNode::~ObjectTreeNode()
{
}
