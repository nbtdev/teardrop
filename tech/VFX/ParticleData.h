/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PARTICLEDATA_INCLUDED)
#define PARTICLEDATA_INCLUDED

#include "Math/Vector4.h"
#include "Memory/Allocators.h"
#include "Memory/Pool.h"
#include <list>

namespace Teardrop
{
	struct ParticleData
	{
		Vector4 m_position;     // particles have a position in system space 
		Vector4 m_velocity;     // particles have a 3D velocity 
		Vector4 m_acceleration; // particles have a 3D acceleration
		float m_scale;          // particle quads are defined at unit size
		float m_time;           // time this particle has been "alive"
		float m_ttl;            // time this particle is allowed to live

		TD_DECLARE_ALLOCATOR();
	};

	typedef Pool<ParticleData> ParticleDataPool;
	typedef std::list<ParticleData*> ParticleList;
}

#endif // PARTICLEDATA_INCLUDED
