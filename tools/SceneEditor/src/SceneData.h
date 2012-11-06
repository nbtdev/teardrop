/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENEDATA_INCLUDED)
#define SCENEDATA_INCLUDED

#include "Data.h"
#include "Game/LoadListener.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Scene;
}

class MessageBus;

class SceneData
	: public Data,
	public Teardrop::LoadListener
{
public:
	static const unsigned int TYPE;

	SceneData(MessageBus* pMsgbus);
	virtual ~SceneData();

	Teardrop::Scene* getScene();

	// Data implementation
	unsigned int getType() const;
	bool load(const char* filepath);
	bool save(const char* filepath);
	bool update(float deltaT);

	//! LoadListener implementation
	void onSceneLoadBegin(Teardrop::Scene* pScene);
	void onZoneCreated(Teardrop::Zone* pZone);
	void onZoneObjectCreated(Teardrop::ZoneObject* pObject);
	void onZoneObjectAdded(Teardrop::Zone* pZone, Teardrop::ZoneObject* pObject);
	void onSceneLoadEnd(Teardrop::Scene* pScene);

	TD_DECLARE_ALLOCATOR();

protected:
	Teardrop::Scene* m_pScene;
	MessageBus* m_pMsgBus;
};

#endif // SCENEDATA_INCLUDED
