/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BLENDGRAPH_INCLUDED)
#define BLENDGRAPH_INCLUDED

#include "Util/_String.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	class BlendGraph
	{
	public:
		BlendGraph();

		struct AnimationDef
		{
			String name;
			String resource;
		};
		typedef std::vector<AnimationDef> Animations;
		Animations m_animations;

		struct AnimationWeight
		{
			AnimationDef* pAnimation;
			float weight;
			float speed;
			int loop;
		};
		typedef std::vector<AnimationWeight> AnimationWeights;

		struct State
		{
			String name;
			AnimationWeights anims;
		};
		typedef std::vector<State> States;
		States m_states;

		enum Curve
		{
			BLENDCURVE_STEP,
			BLENDCURVE_LINEAR,
			BLENDCURVE_CUBIC,
		};

		struct Transition
		{
			State* pFrom;
			State* pTo;
			float duration;
			Curve curve;
		};
		typedef std::vector<Transition> Transitions;
		Transitions m_transitions;

		String m_initState;
		void reset();

	private:
		BlendGraph(const BlendGraph& other);
		BlendGraph& operator=(const BlendGraph& other);
	};
}

#endif // BLENDGRAPH_INCLUDED
