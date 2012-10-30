/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "ObjectTreeView.h"
#include "ObjectTreeNode.h"
#include "MessageBus.h"
#include "Game/include/Scene.h"
#include "Game/include/Zone.h"
#include "Game/include/ZoneObject.h"
#include "Reflection/include/Reflection.h"
#include "Reflection/include/PropertyDef.h"

using namespace CoS;
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(ObjectTreeView, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(wxID_ANY, ObjectTreeView::onSelectionChanged)
END_EVENT_TABLE()
//---------------------------------------------------------------------------
ObjectTreeView::ObjectTreeView(wxWindow* pParent)
:	 wxTreeCtrl(pParent)
{
	m_pMsgBus = 0;
	clear();
}
//---------------------------------------------------------------------------
ObjectTreeView::~ObjectTreeView()
{
}
//---------------------------------------------------------------------------
void ObjectTreeView::connect(MessageBus* pMsgBus)
{
	m_pMsgBus = pMsgBus;

	if (pMsgBus)
	{
		pMsgBus->ObjectSelectionChanged.connect(
			this, &ObjectTreeView::selectedObjectChanged);
		pMsgBus->SceneLoadBegin.connect(
			this, &ObjectTreeView::onSceneLoadBegin);
		pMsgBus->SceneLoadEnd.connect(
			this, &ObjectTreeView::onSceneLoadEnd);
		pMsgBus->ZoneCreated.connect(
			this, &ObjectTreeView::onZoneCreated);
		pMsgBus->ZoneObjectAdded.connect(
			this, &ObjectTreeView::onZoneObjectAdded);
	}
}
//---------------------------------------------------------------------------
void ObjectTreeView::disconnect()
{
	if (m_pMsgBus)
	{
		m_pMsgBus->ObjectSelectionChanged.disconnect(
			this, &ObjectTreeView::selectedObjectChanged);
		m_pMsgBus->SceneLoadBegin.disconnect(
			this, &ObjectTreeView::onSceneLoadBegin);
		m_pMsgBus->SceneLoadEnd.disconnect(
			this, &ObjectTreeView::onSceneLoadEnd);
		m_pMsgBus->ZoneCreated.disconnect(
			this, &ObjectTreeView::onZoneCreated);
		m_pMsgBus->ZoneObjectAdded.disconnect(
			this, &ObjectTreeView::onZoneObjectAdded);
	}

	m_pMsgBus = 0;
}
//---------------------------------------------------------------------------
void ObjectTreeView::selectedObjectChanged(
	void *sender, Reflection::Object* pObj)
{
	if (sender == this)
		return;

	selectObject(pObj);
}
//---------------------------------------------------------------------------
void ObjectTreeView::clear()
{
	DeleteAllItems();
	wxTreeItemId root = AddRoot("Scene");
	ObjectTreeNode* pNode = new ObjectTreeNode(0);
	SetItemData(root, pNode);
}
//---------------------------------------------------------------------------
void ObjectTreeView::selectObject(Reflection::Object* pObject)
{
	ObjectLUT::iterator it = m_objLut.find(pObject);
	if (it != m_objLut.end())
	{
		// select this object in the tree
		SelectItem(it->second, true);
	}
	else
	{
		SelectItem(wxTreeItemId(), true);
	}
}
//---------------------------------------------------------------------------
ObjectTreeNode* ObjectTreeView::addZone(Zone* pZone)
{
	wxTreeItemId root = GetRootItem();
	if (!root.IsOk())
		return 0;

	wxTreeItemId child = AppendItem(root, (const char*)pZone->getName());

	ObjectTreeNode* pNode = new ObjectTreeNode(pZone);
	//pData->setName(pZone->getName());
	//pData->setZone(pZone);
	SetItemData(child, pNode);
	m_objLut[pZone] = child;

	return pNode;
}
//---------------------------------------------------------------------------
void ObjectTreeView::addCategory(ObjectTreeNode* pParent, const char* name)
{
	if (pParent)
	{
		wxTreeItemId id = pParent->GetId();
		AppendItem(id, name);
	}
}
//---------------------------------------------------------------------------
ObjectTreeNode* ObjectTreeView::addObject(
	ObjectTreeNode* pParentNode,
	ZoneObject* pObject, 
	const char* category)
{
	if (!pObject || !pParentNode)
		return 0;

	wxTreeItemId parent = pParentNode->GetId();

	if (category)
	{
		// look for a node named after the category param
		wxTreeItemIdValue cookie;
		wxTreeItemId child = GetFirstChild(parent, cookie);
		while (child.IsOk())
		{
			if (!GetItemText(child).Cmp(category))
			{
				// then we want to append the object to this node instead
				parent = child;
				break;
			}

			child = GetNextSibling(child);
		}
	}

	// append the object to whatever we ended up with for a parent node
	ObjectTreeNode* pNode = new ObjectTreeNode(pObject);
	//pObj->setName(pObject->toString());
	//pObj->setCategory(category);
	//pObj->setObject(pObject);

	wxTreeItemId newItem = AppendItem(parent, (const char*)pObject->getName());
	SetItemData(newItem, pNode);

	m_objLut[pObject] = newItem;

	return pNode;
}
//---------------------------------------------------------------------------
void ObjectTreeView::onSelectionChanged(wxTreeEvent& evt)
{
	if (!m_pMsgBus)
		return;

	wxTreeItemId item = evt.GetItem();

	if (item.IsOk())
	{
		wxTreeItemData* pData = GetItemData(item);
		ObjectTreeNode* pNode = static_cast<ObjectTreeNode*>(pData);
		Reflection::Object* pObj = *pNode;
		m_pMsgBus->ObjectSelectionChanged(
			this,	// sender
			pObj	// the new selected object
			);
	}
}
//---------------------------------------------------------------------------
void ObjectTreeView::UpdateNodeForObject(Reflection::Object* pObj)
{
	if (!pObj)
		return;

	ObjectLUT::iterator it = m_objLut.find(pObj);
	if (it != m_objLut.end())
	{
		SetItemText(it->second, (const char*)pObj->toString());
	}
}
//---------------------------------------------------------------------------
void ObjectTreeView::removeObject(ZoneObject* pObj)
{
	if (!pObj)
		return;

	ObjectLUT::iterator it = m_objLut.find(pObj);
	if (it != m_objLut.end())
	{
		Delete(it->second);
	}
}
//---------------------------------------------------------------------------
ObjectTreeNode* ObjectTreeView::getRootNode()
{
	wxTreeItemId item = GetRootItem();
	ObjectTreeNode* pNode = 0;

	if (item.IsOk())
	{
		wxTreeItemData* pData = GetItemData(item);
		pNode = static_cast<ObjectTreeNode*>(pData);
	}

	return pNode;
}
//---------------------------------------------------------------------------
void ObjectTreeView::onSceneLoadBegin(Scene* /*pScene*/)
{
	// clear out the tree
	clear();
}
//---------------------------------------------------------------------------
void ObjectTreeView::onSceneLoadEnd(Scene* /*pScene*/)
{
}
//---------------------------------------------------------------------------
void ObjectTreeView::onZoneCreated(Zone* pZone)
{
	addZone(pZone);
}
//---------------------------------------------------------------------------
void ObjectTreeView::onZoneObjectAdded(Zone* pZone, ZoneObject* pObj)
{
	ObjectLUT::iterator it = m_objLut.find(pZone);
	if (it != m_objLut.end())
	{
		wxTreeItemData* pData = GetItemData(it->second);
		if (pData)
		{
			ObjectTreeNode* pNode = static_cast<ObjectTreeNode*>(pData);
			addObject(pNode, pObj);
		}
	}
}
