// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
//#include <windows.h>



// TODO: reference additional headers your program requires here
#include <maya/MAngle.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MItDag.h>
#include <maya/MFnCamera.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnStringData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MArgList.h>
#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnIkJoint.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MDagPathArray.h>
#include <maya/MPointArray.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MAnimUtil.h>
#include <maya/MRenderUtil.h>
#include <maya/MQuaternion.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MBoundingBox.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>

// standard libraries
#include <math.h>
#include <vector>
#include <set>
#include <cassert>
#include <fstream>
#include <iostream>
