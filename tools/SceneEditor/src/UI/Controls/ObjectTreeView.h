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

#if !defined(OBJECTTREEVIEW_INCLUDED)
#define OBJECTTREEVIEW_INCLUDED

namespace CoS
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

	typedef std::map<CoS::Reflection::Object*, wxTreeItemId> ObjectLUT;
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
	ObjectTreeNode* addZone(CoS::Zone* pZone);

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
		CoS::ZoneObject* pObj, 
		const char* category=0);

	/*
		Select the indicated object in the tree.
	*/
	void selectObject(CoS::Reflection::Object* pObj);

	/*
		Remove the indicated object from the treeview
	*/
	void removeObject(CoS::ZoneObject* pObj);

	/*
		Update the node for the supplied object (if exists)
	*/
	void UpdateNodeForObject(CoS::Reflection::Object* pObj);

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
	void selectedObjectChanged(void* sender, CoS::Reflection::Object* pObj);
	void onSceneLoadBegin(CoS::Scene* pScene);
	void onSceneLoadEnd(CoS::Scene* pScene);
	void onZoneCreated(CoS::Zone* pZone);
	void onZoneObjectAdded(CoS::Zone* pZone, CoS::ZoneObject* pObj);

	MessageBus* m_pMsgBus;
};

#endif // OBJECTTREEVIEW_INCLUDED
