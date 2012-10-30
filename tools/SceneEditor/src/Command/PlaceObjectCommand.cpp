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
#include "PlaceObjectCommand.h"
#include "DeleteObjectCommand.h"
#include "Reflection/include/ClassDef.h"
#include "Game/include/Zone.h"
#include "Game/include/ZoneObject.h"

using namespace Editor;
//---------------------------------------------------------------------------
PlaceObjectCommand::PlaceObjectCommand( 
	CoS::Reflection::ClassDef* pClassDef, 
	CoS::Zone* pZone, 
	const CoS::Transform& xform,
	unsigned int id
	)
	: m_pClassDef(pClassDef), m_pZone(pZone), m_transform(xform), m_objId(id)
{
	m_pDeleteCmd = 0;
}
//---------------------------------------------------------------------------
PlaceObjectCommand::~PlaceObjectCommand()
{
	delete m_pDeleteCmd;
}
//---------------------------------------------------------------------------
void PlaceObjectCommand::execute()
{
	// validate data members
	if (!m_pClassDef || !m_pZone)
		return; // TODO: exception?

	// then see if this is something we can put in a Zone
	if (!m_pClassDef->isA(CoS::ZoneObject::getClassDef()))
		return; // TODO: exception

	// finally, we can create one...
	CoS::ZoneObject* pZoneObject = 
		static_cast<CoS::ZoneObject*>(m_pClassDef->createInstance(m_objId));
	pZoneObject->initialize();

	// ...and put it in the zone...
	m_pZone->addObject(pZoneObject);

	// ...and place it where it goes...
	pZoneObject->setTransform(m_transform);

	// ...and finally make the reverse command for this
	m_pDeleteCmd = COS_NEW DeleteObjectCommand(pZoneObject, m_pZone);
}
//---------------------------------------------------------------------------
void PlaceObjectCommand::undo()
{
	if (m_pDeleteCmd)
		m_pDeleteCmd->execute();
}