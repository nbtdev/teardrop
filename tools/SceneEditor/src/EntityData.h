/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ENTITYDATA_INCLUDED)
#define ENTITYDATA_INCLUDED

#include "SceneData.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Scene;
	class ZoneObject;
}

class MessageBus;

class EntityData
	: public SceneData
{
	Teardrop::ZoneObject* m_pObject;
	// simple way to enable/disable entity animation playback, as
	// well as support playback at different speeds
	float m_animationSpeed;
	float m_prevAnimationSpeed;

public:
	static const unsigned int TYPE;

	EntityData(MessageBus* pMsgbus);
	virtual ~EntityData();

	Teardrop::ZoneObject* getObject();

	// Data implementation
	unsigned int getType() const;
	bool load(const char* filepath);
	bool save(const char* filepath);
	bool update(float deltaT);

	TD_DECLARE_ALLOCATOR();

private:
	void OnStartAnimationPlayback();
	void OnStopAnimationPlayback();
};

#endif // ENTITYDATA_INCLUDED
