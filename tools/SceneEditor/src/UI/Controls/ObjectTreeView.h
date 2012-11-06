/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTTREEVIEW_INCLUDED)
#define OBJECTTREEVIEW_INCLUDED

namespace Teardrop
{
	class Scene;
	class Zone;
	class ZoneObject;
}

class ObjectTreeNode;
class MessageBus;

/*
	We extend the functionality of wxTreeCtrl for the needs of 
	the "object" treeview (where the current scene objects are
	organized). This extension interacts with the main editor
	body via callbacks in order to manipulate (and react to) the 
	current selection, etc.
*/

class ObjectTreeView : public wxTreeCtrl
{
	DECLARE_EVENT_TABLE()

	typedef std::map<std::string, wxTreeItemId> CategoryLUT;
	CategoryLUT m_catLut;

	typedef std::map<Teardrop::Reflection::Object*, wxTreeItemId> ObjectLUT;
	ObjectLUT m_objLut;

public:
	ObjectTreeView(wxWindow* pParent);
	~ObjectTreeView();

	/*
		Clears all items in the treeview
	*/
	void clear();

	/*
		Add a zone to the treeview -- this will produce a new
		subtree for objects in this zone. Categories (below) must
		be unique only within a zone.
	*/
	ObjectTreeNode* addZone(Teardrop::Zone* pZone);

	/*
		Add a category to a zone -- this will produce a new
		subtree for objects of this type
	*/
	void addCategory(ObjectTreeNode* pParent, const char* name);

	/*
		Add an object to the treeview. Optionally provide a category
		name for the object. Objects with a valid category name will
		be placed under that category's node; objects with no category
		name, or with an invalid name, will be placed at the zone root.
	*/
	ObjectTreeNode* addObject(
		ObjectTreeNode* pParentNode, 
		Teardrop::ZoneObject* pObj, 
		const char* category=0);

	/*
		Select the indicated object in the tree.
	*/
	void selectObject(Teardrop::Reflection::Object* pObj);

	/*
		Remove the indicated object from the treeview
	*/
	void removeObject(Teardrop::ZoneObject* pObj);

	/*
		Update the node for the supplied object (if exists)
	*/
	void UpdateNodeForObject(Teardrop::Reflection::Object* pObj);

	/*
		Get the treeview's root node
	*/
	ObjectTreeNode* getRootNode();

	/*
		Called by the framework during initialization
	*/
	void connect(MessageBus* pMsgBus);
	void disconnect();

private:
	void onSelectionChanged(wxTreeEvent& evt);
	void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);
	void onSceneLoadBegin(Teardrop::Scene* pScene);
	void onSceneLoadEnd(Teardrop::Scene* pScene);
	void onZoneCreated(Teardrop::Zone* pZone);
	void onZoneObjectAdded(Teardrop::Zone* pZone, Teardrop::ZoneObject* pObj);

	MessageBus* m_pMsgBus;
};

#endif // OBJECTTREEVIEW_INCLUDED
