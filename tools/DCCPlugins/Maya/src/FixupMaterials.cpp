/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "MayaPlugin.h"
#include <sstream>
//#define TIXML_USE_STL
#include "tinyxml.h"

//---------------------------------------------------------------------------
CoSFixupMaterials::CoSFixupMaterials()
{
}
//---------------------------------------------------------------------------
CoSFixupMaterials::~CoSFixupMaterials()
{
}
//---------------------------------------------------------------------------
void* CoSFixupMaterials::creator()
{
	return new CoSFixupMaterials();
}
//---------------------------------------------------------------------------
MStatus CoSFixupMaterials::doIt(const MArgList& args)
{
	MStatus status;

	MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &status);

	if ( !status) {
		status.perror("MItDag constructor");
		return status;
	}
	
	int objectCount = 0;
	for ( ; !dagIterator.isDone(); dagIterator.next() ) {

		MDagPath dagPath;

		status = dagIterator.getPath(dagPath);
		if ( !status ) {
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if ( !status ) {
			status.perror("MFnDagNode constructor");
			continue;
		}

		//if (!quiet)
			cout << dagNode.name() << ": " << dagNode.typeName() << endl;

		//if (!quiet)
			cout << "  dagPath: " << dagPath.fullPathName() << endl;

		objectCount += 1;
		if (dagPath.hasFn(MFn::kMesh)) {
			MFnMesh mesh (dagPath, &status);
			if ( !status ) {
				status.perror("MFnMesh constructor");
				continue;
			}

			// get the connected shaders
			MObjectArray shaders;
			MIntArray indices;
			mesh.getConnectedShaders(0, shaders, indices);
			cout << "Found " << shaders.length() << " connected shaders\n";

			for (unsigned int i=0; i<shaders.length(); ++i)
			{
				MFnDependencyNode shadingGroup(shaders[i]);
				MPlug plug = shadingGroup.findPlug("surfaceShader");

				MPlugArray plugArray;
				plug.connectedTo(plugArray, true, false, &status); 

				for (unsigned int j=0; j<plugArray.length(); ++j)
				{
					if (plugArray[j].node().hasFn(MFn::kLambert)
						|| plugArray[j].node().hasFn(MFn::kLambertMaterial)
						|| plugArray[j].node().hasFn(MFn::kPhong)
						|| plugArray[j].node().hasFn(MFn::kPhongMaterial)
						|| plugArray[j].node().hasFn(MFn::kBlinn)
						|| plugArray[j].node().hasFn(MFn::kBlinnMaterial)
						|| plugArray[j].node().hasFn(MFn::kSurfaceShader)
						|| plugArray[j].node().hasFn(MFn::kPluginHwShaderNode)
						)
					{
						MFnDependencyNode shader(plugArray[j].node());
						cout << "Found shader: " << shader.name().asChar() << endl;

						fixup(shader);
					}
				}
			}
		}
	}
	return MS::kSuccess;
}
//---------------------------------------------------------------------------
bool CoSFixupMaterials::isUndoable() const
{
	MGlobal::displayInfo("Command is not undoable");
	return false;
}
//---------------------------------------------------------------------------
void CoSFixupMaterials::fixup(MFnDependencyNode& shader)
{
	MStatus stat;

	// check to see if we already have an hkType attribute
	MObject hkType = shader.attribute("hkType", &stat);
	if (hkType.isNull())
	{
		// and if not, make one
		MFnTypedAttribute attr;
		hkType = attr.create(
			"hkType", 
			"hkType", 
			MFnData::kString, 
			MObject::kNullObj, 
			&stat);

		if (MS::kSuccess != stat)
		{
			cerr << "Could not create hkType material attribute on shader " << shader.name().asChar() << endl;
			return;
		}

		stat = shader.addAttribute(hkType);
	}

	cout << "Added hkType material attribute on shader " << shader.name().asChar() << endl;

	// now do the same for the ArcadiaMaterial "sub-attribute"
	MObject hkAM = shader.attribute("CoSMaterial", &stat);
	MFnTypedAttribute attrAM;
	if (hkAM.isNull())
	{
		// and if not, make one
		hkAM = attrAM.create(
			"CoSMaterial", 
			"CoSMaterial", 
			MFnData::kString, 
			MObject::kNullObj, 
			&stat);

		if (MS::kSuccess != stat)
		{
			cerr << "Could not create CoSMaterial material attribute on shader " << shader.name().asChar() << endl;
			return;
		}

		stat = shader.addAttribute(hkAM);
	}

	cout << "Added CoSMaterial material attribute on shader " << shader.name().asChar() << endl;
	
	// create the arcadia material description in this attribute's value
	std::string s;
	createMaterialDef(shader, s);
	shader.findPlug("CoSMaterial").setString(s.c_str());
}
//---------------------------------------------------------------------------
static void addLambert(TiXmlElement& elem, MFnDependencyNode& shader)
{
	char buf[32];
	TiXmlElement lambert("lambert");
	MFnLambertShader s(shader.object());

	MPlugArray plugs;
	s.findPlug("transparency").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		lambert.SetAttribute("transparent", "true");
	}

	// diffuse color -- alpha is based on transparency setting
	TiXmlElement diffuse("diffuse");
	MColor diff = s.color();
	float transparent = s.transparency().r;

	s.findPlug("color").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		diff = MColor(1,1,1,1);
	}

	sprintf_s(buf, 32, "%f", diff.r);
	diffuse.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", diff.g);
	diffuse.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", diff.b);
	diffuse.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", 1 - transparent);
	diffuse.SetAttribute("a", buf);
	lambert.InsertEndChild(diffuse);

	// ambient color
	TiXmlElement ambient("ambient");
	MColor amb = s.ambientColor();
	sprintf_s(buf, 32, "%f", amb.r);
	ambient.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", amb.g);
	ambient.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", amb.b);
	ambient.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", amb.a);
	ambient.SetAttribute("a", buf);
	lambert.InsertEndChild(ambient);

	// emissive color
	TiXmlElement emissive("emissive");
	MColor em = s.incandescence();
	sprintf_s(buf, 32, "%f", em.r);
	emissive.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", em.g);
	emissive.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", em.b);
	emissive.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", em.a);
	emissive.SetAttribute("a", buf);
	lambert.InsertEndChild(emissive);

	elem.InsertEndChild(lambert);
}
//---------------------------------------------------------------------------
static void addPhong(TiXmlElement& elem, MFnDependencyNode& shader)
{
	char buf[32];
	TiXmlElement phong("phong");
	MFnPhongShader s(shader.object());

	// diffuse color -- alpha is based on transparency setting
	TiXmlElement diffuse("diffuse");
	MColor diff = s.color();
	float transparent = s.transparency().r;

	MPlugArray plugs;
	s.findPlug("transparency").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		phong.SetAttribute("transparent", "true");
	}

	s.findPlug("color").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		diff = MColor(1,1,1,1);
	}

	sprintf_s(buf, 32, "%f", diff.r);
	diffuse.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", diff.g);
	diffuse.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", diff.b);
	diffuse.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", 1 - transparent);
	diffuse.SetAttribute("a", buf);
	phong.InsertEndChild(diffuse);

	// ambient color
	TiXmlElement ambient("ambient");
	MColor amb = s.ambientColor();
	sprintf_s(buf, 32, "%f", amb.r);
	ambient.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", amb.g);
	ambient.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", amb.b);
	ambient.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", amb.a);
	ambient.SetAttribute("a", buf);
	phong.InsertEndChild(ambient);

	// emissive color
	TiXmlElement emissive("emissive");
	MColor em = s.incandescence();
	sprintf_s(buf, 32, "%f", em.r);
	emissive.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", em.g);
	emissive.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", em.b);
	emissive.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", em.a);
	emissive.SetAttribute("a", buf);
	phong.InsertEndChild(emissive);

	// specular color
	TiXmlElement specular("specular");
	MColor spec = s.specularColor();
	sprintf_s(buf, 32, "%f", spec.r);
	specular.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", spec.g);
	specular.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", spec.b);
	specular.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", s.cosPower());
	specular.SetAttribute("power", buf);
	phong.InsertEndChild(specular);

	elem.InsertEndChild(phong);
}
//---------------------------------------------------------------------------
static void addBlinn(TiXmlElement& elem, MFnDependencyNode& shader)
{
	char buf[32];
	TiXmlElement blinn("blinn");
	MFnBlinnShader s(shader.object());

	// diffuse color -- alpha is based on transparency setting
	TiXmlElement diffuse("diffuse");
	MColor diff = s.color();
	float transparent = s.transparency().r;

	MPlugArray plugs;
	s.findPlug("transparency").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		blinn.SetAttribute("transparent", "true");
	}

	s.findPlug("color").connectedTo(plugs, true, false);
	if (plugs.length() && plugs[0].node().hasFn(MFn::kFileTexture))
	{
		diff = MColor(1,1,1,1);
	}

	sprintf_s(buf, 32, "%f", diff.r);
	diffuse.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", diff.g);
	diffuse.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", diff.b);
	diffuse.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", 1 - transparent);
	diffuse.SetAttribute("a", buf);
	blinn.InsertEndChild(diffuse);

	// ambient color
	TiXmlElement ambient("ambient");
	MColor amb = s.ambientColor();
	sprintf_s(buf, 32, "%f", amb.r);
	ambient.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", amb.g);
	ambient.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", amb.b);
	ambient.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", amb.a);
	ambient.SetAttribute("a", buf);
	blinn.InsertEndChild(ambient);

	// emissive color
	TiXmlElement emissive("emissive");
	MColor em = s.incandescence();
	sprintf_s(buf, 32, "%f", em.r);
	emissive.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", em.g);
	emissive.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", em.b);
	emissive.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", em.a);
	emissive.SetAttribute("a", buf);
	blinn.InsertEndChild(emissive);

	// specular color
	TiXmlElement specular("specular");
	MColor spec = s.specularColor();
	sprintf_s(buf, 32, "%f", spec.r);
	specular.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", spec.g);
	specular.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", spec.b);
	specular.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", s.eccentricity());
	specular.SetAttribute("eccentricity", buf);
	blinn.InsertEndChild(specular);

	elem.InsertEndChild(blinn);
}
//---------------------------------------------------------------------------
static void addSurfaceShader(TiXmlElement& elem, MFnDependencyNode& shader)
{
	char buf[32];
	TiXmlElement ss("surface");

	// diffuse color -- alpha is based on transparency setting
	TiXmlElement diffuse("diffuse");
	float transparent = 0;
	shader.findPlug("outTransparencyR").getValue(transparent);

	MPlugArray transPlugs;
	shader.findPlug("outTransparencyR").connectedTo(transPlugs, true, false);
	if (transPlugs.length() && transPlugs[0].node().hasFn(MFn::kFileTexture))
	{
		ss.SetAttribute("transparent", "true");
	}

	MColor diff = MColor(1, 1, 1, 1);
	shader.findPlug("outColorR").getValue(diff.r);
	shader.findPlug("outColorG").getValue(diff.g);
	shader.findPlug("outColorB").getValue(diff.b);
	sprintf_s(buf, 32, "%f", diff.r);
	diffuse.SetAttribute("r", buf);
	sprintf_s(buf, 32, "%f", diff.g);
	diffuse.SetAttribute("g", buf);
	sprintf_s(buf, 32, "%f", diff.b);
	diffuse.SetAttribute("b", buf);
	sprintf_s(buf, 32, "%f", 1 - transparent);
	diffuse.SetAttribute("a", buf);
	ss.InsertEndChild(diffuse);

	elem.InsertEndChild(ss);
}
//---------------------------------------------------------------------------
static TiXmlNode* addTexture(TiXmlElement& elem, MFnDependencyNode& tex)
{
	char buf[32];
	TiXmlElement t("texture");

	// first, get the path info that we can
	MString fname, absFname;
	MRenderUtil::exactFileTextureName(tex.object(), absFname);
	fname = absFname.substring(absFname.rindex('/')+1, absFname.length()-1);
	MString cmd = "toNativePath(\"";
	cmd += absFname;
	cmd += "\")";
	MGlobal::executeCommand(cmd,absFname);

	TiXmlElement filename("filename");
	TiXmlText fnameText(fname.asChar());
	filename.InsertEndChild(fnameText);
	TiXmlElement pathname("fullpath");
	TiXmlText pathnameText(absFname.asChar());
	pathname.InsertEndChild(pathnameText);

	t.InsertEndChild(filename);
	t.InsertEndChild(pathname);

	// get texcoord set usage, if defined
	MPlugArray srcPlugs;
	MString texSetName = "";
	tex.findPlug("uvCoord").connectedTo(srcPlugs, true, false);
	for (unsigned int i=0; i<srcPlugs.length(); ++i)
	{
		if (srcPlugs[i].node().hasFn(MFn::kPlace2dTexture))
		{
			// drill down
			MPlugArray placeSrcPlugs;
			MFnDependencyNode placeNode(srcPlugs[i].node());
			placeNode.findPlug("uvCoord").connectedTo(placeSrcPlugs, true, false);
			
			// tex coord set
			for (unsigned int j=0; j<placeSrcPlugs.length(); ++j)
			{
				if (placeSrcPlugs[j].node().hasFn(MFn::kUvChooser))
				{
					MFnDependencyNode chooser(placeSrcPlugs[j].node());
					MPlug uvSetPlug = chooser.findPlug("uvSets");

					MPlugArray uvsetSrcPlugs;
					for (unsigned int k=0; k<uvSetPlug.evaluateNumElements(); ++k)
					{
						uvSetPlug[k].connectedTo(uvsetSrcPlugs, true, false);
						for (unsigned int l=0; l<uvsetSrcPlugs.length(); ++l)
						{
							if (uvsetSrcPlugs[l].node().hasFn(MFn::kMesh))
							{
								MFnMesh mesh(uvsetSrcPlugs[l].node());
								int index = 0;
								uvsetSrcPlugs[l].getValue(texSetName);
								MStringArray setNames;
								mesh.getUVSetNames(setNames);
								for (unsigned int n=0; n<setNames.length(); ++n)
								{
									if (setNames[n] == texSetName)
									{
										sprintf_s(buf, 32, "%d", n);
										t.SetAttribute("set", buf);
										break;
									}
								}

								break;
							}
						}
					}
				}
			}

			// tex stage setup: addressing
			TiXmlElement addressing("addressing");
			bool wrap, mirror;
			placeNode.findPlug("wrapU").getValue(wrap);
			placeNode.findPlug("mirrorU").getValue(mirror);

			if (mirror)
				addressing.SetAttribute("u", "mirror");
			else if (wrap)
				addressing.SetAttribute("u", "wrap");
			else
				addressing.SetAttribute("u", "clamp");

			placeNode.findPlug("wrapV").getValue(wrap);
			placeNode.findPlug("mirrorV").getValue(mirror);

			if (mirror)
				addressing.SetAttribute("v", "mirror");
			else if (wrap)
				addressing.SetAttribute("v", "wrap");
			else
				addressing.SetAttribute("v", "clamp");

			t.InsertEndChild(addressing);

			// texture matrix setup - scale
			TiXmlElement matrix("matrix");
			TiXmlElement scale("scale");
			TiXmlElement rot("rot");
			TiXmlElement trans("trans");

			double u, v, r;
			placeNode.findPlug("coverageU").getValue(u);
			placeNode.findPlug("coverageV").getValue(v);
			if (fabs(u) < 0.00005)
				u = 0;
			if (fabs(v) < 0.00005)
				v = 0;

			sprintf_s(buf, 32, "%f", u);
			scale.SetAttribute("u", buf);
			sprintf_s(buf, 32, "%f", v);
			scale.SetAttribute("v", buf);

			placeNode.findPlug("translateFrameU").getValue(u);
			placeNode.findPlug("translateFrameV").getValue(v);
			if (fabs(u) < 0.00005)
				u = 0;
			if (fabs(v) < 0.00005)
				v = 0;

			sprintf_s(buf, 32, "%f", u);
			trans.SetAttribute("u", buf);
			sprintf_s(buf, 32, "%f", v);
			trans.SetAttribute("v", buf);

			placeNode.findPlug("rotateFrame").getValue(r);
			if (fabs(r) < 0.00005)
				r = 0;

			sprintf_s(buf, 32, "%f", r);
			rot.SetAttribute("angle", buf);

			matrix.InsertEndChild(scale);
			matrix.InsertEndChild(rot);
			matrix.InsertEndChild(trans);
			t.InsertEndChild(matrix);
		}
	}

	return elem.InsertEndChild(t);
}
//---------------------------------------------------------------------------
void CoSFixupMaterials::createMaterialDef(
	MFnDependencyNode& shader, std::string& s)
{
	TiXmlDocument doc;

	TiXmlElement root("cos_material");
	root.SetAttribute("name", shader.name().asChar());

	if (shader.object().hasFn(MFn::kLambert) 
		|| shader.object().hasFn(MFn::kLambertMaterial))
	{
		addLambert(root, shader);
	}
	else if (shader.object().hasFn(MFn::kPhong) 
		|| shader.object().hasFn(MFn::kPhongMaterial))
	{
		addPhong(root, shader);
	}
	else if (shader.object().hasFn(MFn::kBlinn) 
		|| shader.object().hasFn(MFn::kBlinnMaterial))
	{
		addBlinn(root, shader);
	}
	else 
	{
		addSurfaceShader(root, shader);
	}

	// add texture sampling
	// TODO: support different map types (specularColor, bumpMapping, incandescence, etc)
	MPlugArray texturePlugs;
	if (shader.object().hasFn(MFn::kSurfaceShader))
		shader.findPlug("outColor").connectedTo(texturePlugs, true, false);
	else
		shader.findPlug("color").connectedTo(texturePlugs, true, false);

	for (unsigned int i=0; i<texturePlugs.length(); ++i)
	{
		if (texturePlugs[i].node().hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode tex(texturePlugs[i].node());
			TiXmlElement* pElement = static_cast<TiXmlElement*>(addTexture(root, tex));
			TiXmlElement blend("blend");
			blend.SetAttribute("type", "modulate");
			pElement->InsertEndChild(blend);

			// add the texture index as a hint...
			pElement->SetAttribute("type", "diffuse");
		}
		else if (texturePlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			// todo: support layered textures
		}
	}

	shader.findPlug("incandescence").connectedTo(texturePlugs, true, false);
	for (unsigned int i=0; i<texturePlugs.length(); ++i)
	{
		if (texturePlugs[i].node().hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode tex(texturePlugs[i].node());
			TiXmlElement* pElement = static_cast<TiXmlElement*>(addTexture(root, tex));
			TiXmlElement blend("blend");
			blend.SetAttribute("type", "modulate");
			pElement->InsertEndChild(blend);

			// add the texture index as a hint...
			pElement->SetAttribute("type", "lightmap");
		}
		else if (texturePlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			// todo: support layered textures
		}
	}

	shader.findPlug("ambient color").connectedTo(texturePlugs, true, false);
	for (unsigned int i=0; i<texturePlugs.length(); ++i)
	{
		if (texturePlugs[i].node().hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode tex(texturePlugs[i].node());
			TiXmlElement* pElement = static_cast<TiXmlElement*>(addTexture(root, tex));
			TiXmlElement blend("blend");
			blend.SetAttribute("type", "modulate");
			pElement->InsertEndChild(blend);

			// add the texture index as a hint...
			pElement->SetAttribute("type", "lightmap");
		}
		else if (texturePlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			// todo: support layered textures
		}
	}

	shader.findPlug("bump mapping").connectedTo(texturePlugs, true, false);
	for (unsigned int i=0; i<texturePlugs.length(); ++i)
	{
		if (texturePlugs[i].node().hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode tex(texturePlugs[i].node());
			TiXmlElement* pElement = static_cast<TiXmlElement*>(addTexture(root, tex));
			TiXmlElement blend("blend");
			blend.SetAttribute("type", "modulate");
			pElement->InsertEndChild(blend);

			// add the texture index as a hint...
			pElement->SetAttribute("type", "normalmap");
		}
		else if (texturePlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			// todo: support layered textures
		}
	}

	doc.InsertEndChild(root);

	TiXmlPrinter p;
	doc.Accept(&p);
	s.assign(p.CStr());
}