/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "BlendGraph.h"
#include "Util/StringUtil.h"
#include "tinyxml.h"
#include <map>

using namespace Teardrop;
//---------------------------------------------------------------------------
BlendGraph::BlendGraph()
{
}
//---------------------------------------------------------------------------
bool BlendGraph::load(const String& filename, BlendGraph& graph)
{
	graph.reset();

	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
		return false;

	// populate a BlendGraph definition from the XML
	TiXmlElement* pRoot = doc.RootElement();
	if (String("blendgraph") != pRoot->Value())
		return false;

	TiXmlElement* pAnims = pRoot->FirstChildElement("animations");
	if (!pAnims)
		return false;

	TiXmlElement* pStates = pRoot->FirstChildElement("states");
	if (!pStates)
		return false;

	const char* pInitState = pStates->Attribute("initial");
	if (!pInitState) // need to have an initial state declared
		return false;

	graph.m_initState = pInitState;

	TiXmlElement* pTransitions = pRoot->FirstChildElement("transitions");
	if (!pTransitions)
		return false;

	// extract the directory name of the blend graph file (it is in the 
	// same directory as the animations)
	String path(filename.substr(0, filename.findLast('/')+1));

	// get the animation names and resource files
	TiXmlElement* pAnim = pAnims->FirstChildElement("animation");
	while (pAnim)
	{
		const char* pName = pAnim->Attribute("name");
		const char* pRes = pAnim->GetText();

		if (pName && pRes)
		{
			BlendGraph::AnimationDef anim;
			anim.name = pName;
			anim.resource = path;
			anim.resource += pRes;
			graph.m_animations.push_back(anim);
		}

		pAnim = pAnim->NextSiblingElement("animation");
	}

	typedef std::map<String, BlendGraph::AnimationDef*> AnimLUT;
	AnimLUT animLUT;
	for (size_t i=0; i<graph.m_animations.size(); ++i)
	{
		animLUT[graph.m_animations[i].name] = &(graph.m_animations[i]);
	}

	TiXmlElement* pState = pStates->FirstChildElement("state");
	while (pState)
	{
		const char* pName = pState->Attribute("name");
		TiXmlElement* pAnim = pState->FirstChildElement("animation");
		if (pName && pAnim)
		{
			BlendGraph::State state;
			state.name = pName;

			while (pAnim)
			{
				const char* pName = pAnim->Attribute("name");
				if (pName)
				{
					BlendGraph::AnimationWeight anim;
					AnimLUT::iterator it = animLUT.find(String(pName));
					if (it != animLUT.end())
					{
						anim.pAnimation = it->second;
						anim.loop = 0;
						anim.speed = 1;
						anim.weight = 1;

						const char* pAttr = pAnim->Attribute("weight");
						if (pAttr)
						{
							float f;
							StringUtil::fromString(pAttr, f);
							anim.weight = f;
						}

						pAttr = pAnim->Attribute("speed");
						if (pAttr)
						{
							float f;
							StringUtil::fromString(pAttr, f);
							anim.speed = f;
						}

						pAttr = pAnim->Attribute("loop");
						if (pAttr)
						{
							int i;
							StringUtil::fromString(pAttr, i);
							anim.loop = i;
						}

						state.anims.push_back(anim);
					}
				}

				pAnim = pAnim->NextSiblingElement("animation");
			}

			if (state.anims.size())
			{
				graph.m_states.push_back(state);
			}
		}

		pState = pState->NextSiblingElement("state");
	}

	typedef std::map<String, BlendGraph::State*> StateLUT;
	StateLUT stateLUT;

	for (size_t i=0; i<graph.m_states.size(); ++i)
	{
		stateLUT[graph.m_states[i].name] = &(graph.m_states[i]);
	}

	// and finally, the transitions
	TiXmlElement* pTrans = pTransitions->FirstChildElement("transition");
	while (pTrans)
	{
		const char* pFrom = pTrans->Attribute("from");
		const char* pTo = pTrans->Attribute("to");
		const char* pCurve = pTrans->Attribute("curve");

		if (pFrom && pTo)
		{
			BlendGraph::Transition trans;
			trans.curve = BlendGraph::BLENDCURVE_LINEAR; // default
			trans.duration = 0; // default

			if (String("step") == pCurve)
				trans.curve = BlendGraph::BLENDCURVE_STEP;
			if (String("cubic") == pCurve)
				trans.curve = BlendGraph::BLENDCURVE_CUBIC;

			StateLUT::iterator from = stateLUT.find(String(pFrom));
			StateLUT::iterator to = stateLUT.find(String(pTo));

			if (from != stateLUT.end() && to != stateLUT.end())
			{
				trans.pFrom = from->second;
				trans.pTo = to->second;

				const char* pDuration = pTrans->Attribute("duration");
				if (pDuration)
				{
					float d;
					StringUtil::fromString(pDuration, d);
					trans.duration = d;
				}
			}

			graph.m_transitions.push_back(trans);
		}

		pTrans = pTrans->NextSiblingElement("transition");
	}

	return true;
}
//---------------------------------------------------------------------------
void BlendGraph::reset()
{
	m_animations.clear();
	m_states.clear();
	m_transitions.clear();
}