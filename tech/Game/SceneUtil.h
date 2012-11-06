/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SCENEUTIL_INCLUDED)
#define SCENEUTIL_INCLUDED

class TiXmlElement;

namespace Teardrop
{
	class AABB;
	class Transform;
	class Component;
	class ZoneObject;
	class Scene;
	class Stream;
	class LoadListener;

	struct SceneUtil
	{
		static void deserializeTransform(
			/*in*/	TiXmlElement* pXformNode,
			/*out*/	Transform& xform);

		static void deserializeAABB(
			/*in*/	TiXmlElement* pAABB,
			/*out*/	AABB& aabb);

		static void serializeAABB(
			/*in*/	TiXmlElement& aabbElem,
			/*out*/	const AABB& aabb);

		static void createComponentsFromXml(
			/*in*/	TiXmlElement* pComponentsNode,
			/*in*/	const char* pAssetPath,
			/*in*/	ZoneObject* pHost);

		static bool saveScene(
			/*in*/	const Scene* pScene,
			/*in*/	const char* filepath);

		static bool loadScene(
			/*in*/	Scene* pScene,
			/*in*/	const char* filepath,
			/*in*/	LoadListener* pListener=0);
	};
}

#endif // SCENEUTIL_INCLUDED
