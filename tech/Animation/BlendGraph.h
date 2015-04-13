/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
