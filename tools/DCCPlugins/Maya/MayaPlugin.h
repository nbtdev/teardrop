/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#pragma once

class CoSFixupMaterials : public MPxCommand
{
public:
	CoSFixupMaterials();
	virtual ~CoSFixupMaterials();

	static void* creator();
	MStatus			doIt(const MArgList& args);
	//MStatus			parseArgs(const MArgList& args);
	//MStatus			redoIt();
	//MStatus			undoIt();
	bool			isUndoable() const;

private:
	void fixup(MFnDependencyNode& shader);
	void createMaterialDef(MFnDependencyNode& shader, std::string& s);
};
