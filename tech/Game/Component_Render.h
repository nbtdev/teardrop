/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COMPONENT_RENDER_INCLUDED)
#define COMPONENT_RENDER_INCLUDED

#include "Gfx/GfxMeshInstance.h"
#include "Gfx/GfxShaderConstantTable.h"
#include "Memory/Allocators.h"
#include "Game/Component.h"
#include "Util/SharedPointer.h"
#include <vector>
#include <list>

namespace Teardrop
{
	struct Environment;
	class ZoneObject;
	class Scene;
	class IMeshInstanceProvider;

	class RenderComponent 
		: public Component
	{
		GfxMeshInstance m_meshInst;
		GfxShaderConstantTable m_constants;
		
		typedef std::list<IMeshInstanceProvider*> MeshInstanceProviders;
		MeshInstanceProviders m_meshInstProviders;

	public:
		TD_CLASS(RenderComponent, Component);
		TD_CLASS_CREATABLE();

		TD_PROPERTY(MeshName, "Name of mesh asset file to use", String, "(undefined)", FileChooser);
		TD_PROPERTY(ShaderName, "Name of shader to use on this asset (overrides value exported with asset)", String, "", 0);
		TD_PROPERTY(ShadowCaster, "Whether or not this object casts shadows", bool, true, 0);
		TD_PROPERTY(ShadowReceiver, "Whether or not this object receives shadows", bool, true, 0);
		TD_PROPERTY(Lit, "Whether or not this object is dynamically lit", bool, true, 0);
		TD_PROPERTY(Enabled, "Whether or not this component queues renderables", bool, true, 0);

		RenderComponent();
		~RenderComponent();

		//! Object implementation
		bool initialize();
		bool destroy();

		bool update(float deltaT);
		void updateTransform(const Transform& xform);

		GfxMeshInstance& getMeshInstance() { return m_meshInst; }
		void setMeshInstance(const GfxMeshInstance& inst); // call with caution
		void updateMatrixPalette(const Matrix44* pPalette, size_t sz);

		//! dynamic light support -- lightweight call if object not lit
		void updateLightList(Scene* pScene);

		// return mutable reference to our shader constant table
		GfxShaderConstantTable& getShaderConstants() { return m_constants; }

		// queue all mesh instances for rendering
		void queueForRendering(GfxRenderer* pRenderer);

		// add/remove mesh instance providers
		void addMeshInstanceProvider(IMeshInstanceProvider*);
		void removeMeshInstanceProvider(IMeshInstanceProvider*);

		TD_DECLARE_ALLOCATOR();

	protected:
		// call this if you change the "lit" properties of the materials for the mesh instance
		void recalculateLighting();

		typedef std::vector<GfxLight*> LightList;
		typedef Teardrop::SharedPointer<LightList> LightListPtr;
		LightListPtr m_pLights;

		bool m_bNeedLightsUpdated;

	private:
		void onInstanceCreated();
	};
}

#endif // COMPONENT_RENDER_INCLUDED
