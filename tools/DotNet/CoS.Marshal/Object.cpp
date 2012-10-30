/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgements in Readme.txt

This software is provided free-of-charge for non-commercial use, under the 
GNU Lesser General Public License as published by the Free Software Foundation; 
either version 2 of the License, or (at your option) any later version. This 
program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

For commercial applications, contact the Clash Of Steel Team at 
info@clashofsteel.net for commercial licensing terms and information.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Object.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Transform.h"
#include "Matrix44.h"

#include "Reflection/include/Reflection.h"
#include "Reflection/include/PropertyDef.h"

#include <msclr/marshal.h>
using namespace CoS;
using namespace Reflection;
using namespace System::Reflection;
using namespace msclr::interop;
//---------------------------------------------------------------------------
CoS::Object::Object()
{
	m_pObj = 0;
}
//---------------------------------------------------------------------------
CoS::Object::~Object()
{
}
//---------------------------------------------------------------------------
CoS::Object::!Object()
{
}
//---------------------------------------------------------------------------
// hacky -- can we find a different way to do this?
static System::Object^ convertVoidPointer(
	const void* p, System::Reflection::PropertyInfo^ pi)
{
	if (pi->PropertyType == System::Single::typeid)
	{
		return gcnew System::Single(*((float*)p));
	}
	if (pi->PropertyType == System::Int32::typeid)
	{
		return gcnew System::Int32(*((int*)p));
	}
	if (pi->PropertyType == System::Boolean::typeid)
	{
		return gcnew System::Boolean(*((bool*)p));
	}
	if (pi->PropertyType == System::String::typeid)
	{
		return gcnew System::String(*((CoS::String*)p));
	}
	if (pi->PropertyType == Math::Transform::typeid)
	{
		return gcnew Math::Transform(*((CoS::Transform*)p));
	}
	if (pi->PropertyType == Math::Vector4::typeid)
	{
		return gcnew Math::Vector4(*((CoS::Vector4*)p));
	}
	if (pi->PropertyType == Math::Quaternion::typeid)
	{
		return gcnew Math::Quaternion(*((CoS::Quaternion*)p));
	}
	return nullptr;
}
//---------------------------------------------------------------------------
void CoS::Object::_setRuntimeObject(Reflection::Object* pObj)
{
	m_pObj = pObj;

	// update the CLR properties with those from the object just set
	if (pObj)
	{
		ClassDef* pClass = pObj->getDerivedClassDef();
		
		// our system (currently) does not get all properties in a hierarchy;
		// so we need to dive down to grab them all for setting
		while (pClass)
		{
			const PropertyDef* pProp = pClass->getProps();
			while (pProp)
			{
				// find the corresponding property on the CLR type and
				// set its value to match
				System::Reflection::PropertyInfo^ pi = GetType()->GetProperty(
					gcnew System::String(pProp->getName()));

				if (pi != nullptr)
				{
					// how to come up with "val"?
					const void* pVal = pProp->getDataPointer(pObj);
					System::Object^ val = convertVoidPointer(pVal, pi);
					pi->SetValue(this, val, nullptr);
				}

				pProp = pProp->m_pNext;
			}

			// this stops at Reflection::Object which has no ClassDef
			pClass = ClassDef::findClassDef(pClass->getBaseClassName());
		}
	}
}
//---------------------------------------------------------------------------
Reflection::Object* CoS::Object::_getRuntimeObject()
{
	return m_pObj;
}
//---------------------------------------------------------------------------
// hacky -- can we find a different way to do this?
static void propagateSystemObject(
	System::String^ propName, System::Object^ o, Reflection::Object* pObj)
{
	System::Type^ type = o->GetType();

	marshal_context^ ctx = gcnew marshal_context();
	const char* pPropName = ctx->marshal_as<const char*>(propName);
	const PropertyDef* pProp = 
		pObj->getDerivedClassDef()->findProperty(pPropName, true);
	delete ctx;

	if (!pProp)
		return;

	if (type == System::Single::typeid)
	{
		float f = (System::Single)o;
		pProp->setData(pObj, &f);
	}
	if (type == System::Int32::typeid)
	{
		int i = (System::Int32)o;
		pProp->setData(pObj, &i);
	}
	if (type == System::Boolean::typeid)
	{
		bool b = (System::Boolean)o;
		pProp->setData(pObj, &b);
	}
	if (type == System::String::typeid)
	{
		marshal_context^ ctx = gcnew marshal_context();
		const char* s = ctx->marshal_as<const char*>(o->ToString());
		{
			CoS::String str(s);
			pProp->setData(pObj, &str);
		}
		delete ctx;
	}
	if (type == Math::Transform::typeid)
	{
		Math::Transform^ t = (Math::Transform^)o;
		const CoS::Transform& xform = t->_getInternal();
		pProp->setData(pObj, &xform);
	}
	if (type == Math::Vector2::typeid)
	{
		Math::Vector2^ v = (Math::Vector2^)o;
		const CoS::Vector2& vec = v->_getInternal();
		pProp->setData(pObj, &vec);
	}
	if (type == Math::Vector4::typeid)
	{
		Math::Vector4^ v = (Math::Vector4^)o;
		const CoS::Vector4& vec = v->_getInternal();
		pProp->setData(pObj, &vec);
	}
	if (type == Math::Quaternion::typeid)
	{
		Math::Quaternion^ q = (Math::Quaternion^)o;
		const CoS::Quaternion& quat = q->_getInternal();
		pProp->setData(pObj, &quat);
	}
	if (type == Math::Matrix44::typeid)
	{
		Math::Matrix44^ m = (Math::Matrix44^)o;
		const CoS::Matrix44& mat = m->_getInternal();
		pProp->setData(pObj, &mat);
	}

	// TODO: enums? collections? pointers? oh my...
}
//---------------------------------------------------------------------------
// this whole concept is hacky...
void CoS::Object::PropertyChanged(System::String^ propName)
{
	// find the changed property
	PropertyInfo^ pi = GetType()->GetProperty(propName);
	if (pi != nullptr)
	{
		// get the new value of the .NET property
		System::Object^ o = pi->GetValue(this, nullptr);

		// propagate the new value back out to the C++ object
		propagateSystemObject(propName, o, m_pObj);
	}
}
