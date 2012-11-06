/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTTREENODE_INCLUDED)
#define OBJECTTREENODE_INCLUDED

#include <wx/treectrl.h>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}
}

class ObjectTreeNode
	: public wxTreeItemData
{
	Teardrop::Reflection::Object* m_pObject;

public:
	ObjectTreeNode(Teardrop::Reflection::Object* pObj);
	~ObjectTreeNode();

	operator Teardrop::Reflection::Object*() { return m_pObject; }
};

#endif // OBJECTTREENODE_INCLUDED
