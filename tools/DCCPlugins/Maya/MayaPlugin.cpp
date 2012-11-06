/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "MayaPlugin.h"


// Routine for registering the command within Maya
__declspec(dllexport) MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, "Teardrop Games", "8.5", "Any");
	status = plugin.registerCommand( "cosFixupMaterials", CoSFixupMaterials::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

// Routine for unregistering the command within Maya
__declspec(dllexport) MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );
	status = plugin.deregisterCommand( "cosFixupMaterials" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
