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
#include "SceneDataWriter.h"
#include "tinyxml.h"
#include "Util/StringUtil.h"

using namespace CoS;
//---------------------------------------------------------------------------
SceneDataWriter::SceneDataWriter()
{
}
//---------------------------------------------------------------------------
SceneDataWriter::~SceneDataWriter()
{
}
//---------------------------------------------------------------------------
bool SceneDataWriter::write(
	const SceneEntries& entries, const char* dataFileName)
{
	TiXmlDocument doc;
	doc.InsertEndChild(TiXmlDeclaration());
	doc.InsertEndChild(TiXmlElement("scene"));

	TiXmlElement* pScene = doc.RootElement();
	TiXmlElement initZone("initial_zone");
	initZone.SetAttribute("id", "12345678");
	pScene->InsertEndChild(initZone); // todo: get id

	TiXmlElement zone("zone");
	zone.SetAttribute("name", "name");
	zone.SetAttribute("id", "12345678");
	
	TiXmlElement dims("dimensions");
	dims.SetAttribute("x", 5000);
	dims.SetAttribute("y", 800);
	dims.SetAttribute("z", 5000);
	
	TiXmlElement origin("origin");
	origin.SetAttribute("x", 0);
	origin.SetAttribute("y", 0);
	origin.SetAttribute("z", 0);

	zone.InsertEndChild(dims);
	zone.InsertEndChild(origin);
	zone.InsertEndChild(TiXmlElement("neighbors"));

	TiXmlElement objects("objects");

	for (SceneEntries::const_iterator it = entries.begin();
		it != entries.end(); ++it)
	{
		TiXmlElement prop("prop");
		TiXmlElement position("position");
		TiXmlElement orientation("orientation");
		TiXmlElement angle("angle");
		TiXmlElement axis("axis");
		TiXmlElement scale("scale");
		TiXmlElement render("render");
		TiXmlElement aabb("AABB");
		TiXmlElement minCorner("min");
		TiXmlElement maxCorner("max");

		char buf[32];
		StringUtil::toString(it->trans[0], buf, 32);
		position.SetAttribute("x", buf);
		StringUtil::toString(it->trans[1], buf, 32);
		position.SetAttribute("y", buf);
		StringUtil::toString(it->trans[2], buf, 32);
		position.SetAttribute("z", buf);

		StringUtil::toString(it->scale[0], buf, 32);
		scale.SetAttribute("x", buf);
		StringUtil::toString(it->scale[1], buf, 32);
		scale.SetAttribute("y", buf);
		StringUtil::toString(it->scale[2], buf, 32);
		scale.SetAttribute("z", buf);

		StringUtil::toString(it->axis[0], buf, 32);
		axis.SetAttribute("x", buf);
		StringUtil::toString(it->axis[1], buf, 32);
		axis.SetAttribute("y", buf);
		StringUtil::toString(it->axis[2], buf, 32);
		axis.SetAttribute("z", buf);
		StringUtil::toString(it->angle, buf, 32);
		angle.SetAttribute("rot", buf);

		StringUtil::toString(it->minCorner[0], buf, 32);
		minCorner.SetAttribute("x", buf);
		StringUtil::toString(it->minCorner[1], buf, 32);
		minCorner.SetAttribute("y", buf);
		StringUtil::toString(it->minCorner[2], buf, 32);
		minCorner.SetAttribute("z", buf);

		StringUtil::toString(it->maxCorner[0], buf, 32);
		maxCorner.SetAttribute("x", buf);
		StringUtil::toString(it->maxCorner[1], buf, 32);
		maxCorner.SetAttribute("y", buf);
		StringUtil::toString(it->maxCorner[2], buf, 32);
		maxCorner.SetAttribute("z", buf);

		aabb.InsertEndChild(maxCorner);
		aabb.InsertEndChild(minCorner);

		render.SetAttribute("asset", it->name.c_str());
		orientation.InsertEndChild(axis);
		orientation.InsertEndChild(angle);
		prop.InsertEndChild(position);
		prop.InsertEndChild(orientation);
		prop.InsertEndChild(scale);
		prop.InsertEndChild(aabb);
		prop.InsertEndChild(render);
		objects.InsertEndChild(prop);
	}

	zone.InsertEndChild(objects);
	pScene->InsertEndChild(zone);

	// write out the XML
	doc.SaveFile(dataFileName);

	return true;
}
