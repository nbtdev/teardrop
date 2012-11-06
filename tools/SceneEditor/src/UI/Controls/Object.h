/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECT_INCLUDED)
#define OBJECT_INCLUDED

namespace Teardrop
{
	class ZoneObject;

	class SceneObject
		: public Teardrop::Reflection::Object,
		public wxTreeItemData // so it can be attached to wxTreeCtrl nodes
	{
		std::string m_name;
		std::string m_category;
		Teardrop::ZoneObject* m_pObject;

	public:
		TD_CLASS(SceneObject, Object);
		SceneObject();
		~SceneObject();

		const char* getName() const;
		const char* getCategory() const;
		Teardrop::ZoneObject* getObject();

		void setName(const char* name);
		void setCategory(const char* category);
		void setObject(Teardrop::ZoneObject* pObject);
	};
}

#endif // OBJECT_INCLUDED
