/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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