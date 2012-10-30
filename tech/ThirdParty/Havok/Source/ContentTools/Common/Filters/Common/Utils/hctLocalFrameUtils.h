/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HCT_HANDLE_UTILS_H
#define HCT_HANDLE_UTILS_H

class hkLocalFrame;
class hkLocalFrameGroup;


template <class Key, class Value, typename Allocator> class hkPointerMap;
template <typename Value, typename Allocator> class hkStringMap;

	/// Utilities for constructing local frames.
class hctLocalFrameUtils
{
	public:

			/// Create a hierarchy of local frames for a node.
		static hkLocalFrame* createLocalFrames(	const class hkxNode* node,
												const hkxScene* scene,
												bool isRoot,
												hkPointerMap<const hkxNode*,int>* ignoreNodes,
												hkStringMap<hkLocalFrameGroup*, hkContainerHeapAllocator>& createdGroups );
};

#endif

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20091222)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
