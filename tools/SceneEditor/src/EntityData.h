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

#if !defined(ENTITYDATA_INCLUDED)
#define ENTITYDATA_INCLUDED

#include "SceneData.h"
#include "Memory/include/Memory.h"

namespace CoS
{
	class Scene;
	class ZoneObject;
}

class MessageBus;

class EntityData
	: public SceneData
{
	CoS::ZoneObject* m_pObject;
	// simple way to enable/disable entity animation playback, as
	// well as support playback at different speeds
	float m_animationSpeed;
	float m_prevAnimationSpeed;

public:
	static const unsigned int TYPE;

	EntityData(MessageBus* pMsgbus);
	virtual ~EntityData();

	CoS::ZoneObject* getObject();

	// Data implementation
	unsigned int getType() const;
	bool load(const char* filepath);
	bool save(const char* filepath);
	bool update(float deltaT);

	COS_DECLARE_ALLOCATOR();

private:
	void OnStartAnimationPlayback();
	void OnStopAnimationPlayback();
};

#endif // ENTITYDATA_INCLUDED
