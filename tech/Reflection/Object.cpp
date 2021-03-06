/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#include "Object.h"
#include "ClassDef.h"
#include "PropertyDef.h"

using namespace Teardrop;
using namespace Reflection;
//-----------------------------------------------------------------------------
void Object::setBroadcastPropertyChangedFn(BroadcastPropertyChanged /*fn*/)
{
}
//-----------------------------------------------------------------------------
Object::Object()
{
}
//-----------------------------------------------------------------------------
Object::~Object()
{
}
//-----------------------------------------------------------------------------
void Object::onInstanceCreated()
{
}
//-----------------------------------------------------------------------------
void Object::setupPropertyDefaults()
{
    // initialize properties with their default values
    const ClassDef* pDef = getDerivedClassDef();
    if (!pDef)
        return;

    // start with the most derived class and work towards the most base class
    const PropertyDef* pProp = pDef->getProps();
    while (pProp)
    {
        if (!pProp->isCollection() && !pProp->isNested())
            pProp->setDataFromString(this, String(pProp->getDefault()));

        pProp = pProp->m_pNext;
    }

    const ClassDef* pBaseClass = pDef->getBaseClass();
    while (pBaseClass)
    {
        const PropertyDef* pProp = pBaseClass->getProps();
        while (pProp)
        {
            pProp->setDataFromString(this, String(pProp->getDefault()));
            pProp = pProp->m_pNext;
        }

        pBaseClass = pBaseClass->getBaseClass();
    }
}
//-----------------------------------------------------------------------------
bool Object::initialize()
{
    return true;
}
//-----------------------------------------------------------------------------
bool Object::destroy()
{
    return true;
}
//-----------------------------------------------------------------------------
const UUID& Object::getObjectId()
{
    return mId;
}
//-----------------------------------------------------------------------------
void Object::setObjectId(const UUID& id)
{
    mId = id;
}
//-----------------------------------------------------------------------------
void Object::setObjectId(const String& id)
{
    mId.fromString(id);
}
//-----------------------------------------------------------------------------
void Object::notifyPropertyChanged(const PropertyDef* pPropDef)
{
    notifyPropertyChangedLocal(pPropDef);

    //if (s_propChangedFn)
    //	s_propChangedFn(this, pPropDef);

    // preferred way to set up notifications
    //if (!PropertyChanged.empty())
    //	PropertyChanged(pPropDef);
    PropertyChanged.raise(pPropDef);
}
//-----------------------------------------------------------------------------
void Object::notifyPropertyChanging(const PropertyDef* pPropDef)
{
    notifyPropertyChangingLocal(pPropDef);

    //if (s_propChangedFn)
    //	s_propChangedFn(this, pPropDef);

    // preferred way to set up notifications
    //if (!PropertyChanged.empty())
    //	PropertyChanged(pPropDef);
    PropertyChanging.raise(pPropDef);
}
//-----------------------------------------------------------------------------
void Object::notifyPropertyChangedLocal(const PropertyDef* /*pPropDef*/)
{
    // intentionally empty/stub dummy definition
}
//-----------------------------------------------------------------------------
void Object::notifyPropertyChangingLocal(const PropertyDef* /*pPropDef*/)
{
    // intentionally empty/stub dummy definition
}
//-----------------------------------------------------------------------------
const ClassDef* Object::getDerivedClassDef() const
{
    return 0;
}
//-----------------------------------------------------------------------------
void Object::toString(/*inout*/ String& str) const
{
    const ClassDef* pClass = getDerivedClassDef();
    if (pClass)
        str = pClass->getName();
    else
        str = "Object";
}
//-----------------------------------------------------------------------------
String Object::toString() const
{
    String rtn;
    toString(rtn);
    return rtn;
}
//---------------------------------------------------------------------------
Object* Object::clone() const
{
    // we can handle single-level component definitions; all we are doing is
    // making a new one of whatever it is, and filling in its first-level
    // properties, and sending it on its way. Any components that have
    // deeper cloning needs should override this method
    const ClassDef* pClassDef = getDerivedClassDef();
    Object* pRtn = pClassDef->createInstance();

    while (pClassDef)
    {
        // go through the properties and copy their values
        const PropertyDef* pOtherProp = pClassDef->getProps();
        while (pOtherProp)
        {
            pOtherProp->copyTo(pRtn, this);
            pOtherProp = pOtherProp->m_pNext;
        }

        pClassDef = pClassDef->getBaseClass();
    }

    // setHost() and initialize() calls are left to the caller, since we do
    // not know in here what the host actually will be (nor do we want to know)
    return pRtn;
}
