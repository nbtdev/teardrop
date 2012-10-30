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

#if !defined(PARTICLEDATA_INCLUDED)
#define PARTICLEDATA_INCLUDED

#include "Math/include/Vector4.h"
#include "Memory/include/Allocators.h"
#include "Memory/include/Pool.h"
#include <list>

namespace CoS
{
	struct ParticleData
	{
		Vector4 m_position;     // particles have a position in system space 
		Vector4 m_velocity;     // particles have a 3D velocity 
		Vector4 m_acceleration; // particles have a 3D acceleration
		float m_scale;          // particle quads are defined at unit size
		float m_time;           // time this particle has been "alive"
		float m_ttl;            // time this particle is allowed to live

		COS_DECLARE_ALLOCATOR();
	};

	typedef Pool<ParticleData> ParticleDataPool;
	typedef std::list<ParticleData*> ParticleList;
}

#endif // PARTICLEDATA_INCLUDED
