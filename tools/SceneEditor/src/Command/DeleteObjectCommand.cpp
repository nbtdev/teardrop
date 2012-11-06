/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "DeleteObjectCommand.h"
#include "PlaceObjectCommand.h"
#include "Game/include/Zone.h"
#include "Game/include/ZoneObject.h"

using namespace Editor;
//---------------------------------------------------------------------------
DeleteObjectCommand::DeleteObjectCommand(
	CoS::ZoneObject* pZoneObject,
	CoS::Zone* pZone)
	: m_pZoneObject(pZoneObject), m_pZone(pZone)
{
	m_pPlaceCommand = 0;
}
//---------------------------------------------------------------------------
DeleteObjectCommand::~DeleteObjectCommand()
{
	delete m_pPlaceCommand;
}
//---------------------------------------------------------------------------
void DeleteObjectCommand::execute()
{
	// validate data members
	if (!m_pZoneObject)
		return; // TODO: exception?

	// then we need to remove this object from its zone
	m_pZone->removeObject(m_pZoneObject);

	// make an undo step in case we need it
	m_pPlaceCommand = COS_NEW PlaceObjectCommand(
		m_pZoneObject->getDerivedClassDef(), 
		m_pZone, 
		m_pZoneObject->getTransform(),
		m_pZoneObject->getObjectId());

	// and then destroy the object
	m_pZoneObject->destroy();
	delete m_pZoneObject;
}
//---------------------------------------------------------------------------
void DeleteObjectCommand::undo()
{
	if (m_pPlaceCommand)
		m_pPlaceCommand->execute();
}