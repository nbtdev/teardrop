/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MANIFEST_INCLUDED)
#define MANIFEST_INCLUDED

#include <map>
#include <vector>
#include <list>
#include "Util/_String.h"
#include "Memory/Allocators.h"

class TiXmlElement;

namespace Teardrop
{
	struct Environment;
	class ComponentHost;

	typedef std::list<String> StringList;

	namespace Reflection
	{
		class ClassDef;
		class Object;
	}

	class Manifest
	{
	public:
		struct ManifestEntry
		{
			String val;
			Reflection::ClassDef* pClassDef;
			size_t id;
		};
		typedef std::map<String, ManifestEntry> NVP;
		typedef std::vector<ManifestEntry> EntryList;
		typedef std::map<String, EntryList> GroupedNVP;

		struct Property
		{
			String m_name;
			String m_value;

			// for collection properties
			typedef std::vector<String> Values;
			Values m_values;
			bool m_bIsCollection;

			Property() { m_bIsCollection = false; }
			Property(const Property& other) { *this = other; }
			Property& operator=(const Property& other)
			{
				m_name = other.m_name;
				m_value = other.m_value;
				m_values = other.m_values;
				m_bIsCollection = other.m_bIsCollection;
				return *this;
			}

			TD_DECLARE_ALLOCATOR();
		};
		typedef std::vector<Property> Properties;

		struct Component
		{
			Reflection::ClassDef* m_pClassDef;
			Properties m_properties;

			Component() {}
			Component(const Component& other) { *this = other; }
			Component& operator=(const Component& other)
			{
				m_pClassDef = other.m_pClassDef;
				m_properties = other.m_properties;
				return *this;
			}

			TD_DECLARE_ALLOCATOR();
		};
		typedef std::vector<Component> Components;

		struct ComponentHost
		{
			Reflection::ClassDef* m_pClassDef;
			String m_name;
			String m_category;
			String m_type;
			String m_assetPath;
			Properties m_properties;
			Components m_components;

			ComponentHost() {}
			ComponentHost(const ComponentHost& other) { *this = other; }
			ComponentHost& operator=(const ComponentHost& other)
			{
				m_pClassDef = other.m_pClassDef;
				m_name = other.m_name;
				m_category = other.m_category;
				m_type = other.m_type;
				m_properties = other.m_properties;
				m_components = other.m_components;
				m_assetPath = other.m_assetPath;
				return *this;
			}

			TD_DECLARE_ALLOCATOR();
		};
		typedef std::multimap<const Reflection::ClassDef*, ComponentHost*> ComponentHostTable;

		// variants and player/avatar slot data
		struct Equipment
		{
			String m_name;
			ManifestEntry* m_pEntry;

			Equipment() {}
			Equipment(const Equipment& other) { *this = other; }
			Equipment& operator=(const Equipment& other)
			{
				m_name = other.m_name;
				m_pEntry = other.m_pEntry;
				return *this;
			}

			TD_DECLARE_ALLOCATOR();
		};
		typedef std::vector<Equipment> EquipmentList;

		struct Slot
		{
			int m_ordinal;
			EquipmentList m_equipment;

			Slot() {}
			Slot(const Slot& other) { *this = other; }
			Slot& operator=(const Slot& other)
			{
				m_ordinal = other.m_ordinal;
				m_equipment = other.m_equipment;
				return *this;
			}
		};
		typedef std::vector<Slot> Slots;

		struct Variant
		{
			Reflection::ClassDef* m_pClass;
			String m_name;
			int m_id;
			const ComponentHost* m_pTarget;
			Slots m_slots;

			Variant() {}
			Variant(const Variant& other) { *this = other; }
			Variant& operator=(const Variant& other)
			{
				m_pClass = other.m_pClass;
				m_name = other.m_name;
				m_id = other.m_id;
				m_pTarget = other.m_pTarget;
				m_slots = other.m_slots;
				return *this;
			}
		};
		typedef std::list<const Variant*> Variants;
		typedef std::multimap<const Reflection::ClassDef*, Variant> VariantTable;
		typedef std::map<unsigned int, Variant*> VariantIDLookupTable;

		Manifest();
		virtual ~Manifest();

		bool initialize();
		bool destroy();

		// temp -- we should provide an iterator for this later
		const NVP& getDefinitions() const { return m_nvp; }
		bool getDefinitionsByCategory(const String& category, EntryList& list) const;
		bool getCategories(StringList& categories) const;

		// Variant database
		void findVariants(/*in*/const Reflection::ClassDef* pClassDef, /*out*/Variants& variants) const;
		const Variant* findVariant(const Reflection::ClassDef* pClassDef, const String& name) const;
		const Variant* findVariant(unsigned int id) const;

		// ComponentHost database and factory
		const ComponentHost* findComponentHost(const Reflection::ClassDef* pClassDef, const String& name) const;
		Teardrop::ComponentHost* createInstance(const Manifest::ComponentHost* pDef);

		TD_DECLARE_ALLOCATOR();

	private:
		NVP m_nvp;
		GroupedNVP m_grouped;
		ComponentHostTable m_compHostDefs;
		VariantTable m_variants;
		VariantIDLookupTable m_variantIdTable;

		void initializeDefinitions();
		void initializeVariants(const String& path);

		void deserializeComponentHost(TiXmlElement*, const String&);
		void deserializeProperties(Properties&, TiXmlElement*);
		void deserializeComponents(Components&, TiXmlElement*);

		void deserializeVariant(TiXmlElement*);
		void deserializeSlots(Slots&, TiXmlElement*);
	};
}

#endif // MANIFEST_INCLUDED
