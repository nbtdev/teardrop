/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#pragma once

#include "Asset/MeshAsset.h"
#include "Gfx/Material.h"
#include "Gfx/ShaderConstantTable.h"
#include "Gfx/Renderable.h"
#include "Memory/Allocators.h"
#include "Game/Component.h"
#include "Util/SharedPointer.h"
#include <vector>
#include <list>

namespace Teardrop {

struct Environment;
class ZoneObject;
class Scene;
class Matrix44;

namespace Gfx {
    class RenderableProvider;
    class RenderQueue;
    class Light;
}

class RenderComponent
    : public Component
{
public:

    TD_CLASS(RenderComponent, Component);
    TD_CLASS_CREATABLE();

    TD_POINTER_PROPERTY(MeshAsset, "Name of mesh asset file to use", MeshAsset, 0);
    TD_POINTER_PROPERTY(Material, "Material override for this component", Gfx::Material, 0);
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

//		GfxMeshInstance& getMeshInstance() { return m_meshInst; }
//		void setMeshInstance(const GfxMeshInstance& inst); // call with caution
    void updateMatrixPalette(const Matrix44* pPalette, size_t sz);

    //! dynamic light support -- lightweight call if object not lit
    void updateLightList(Scene* pScene);

    // return mutable reference to our shader constant table
    Gfx::ShaderConstantTable& getShaderConstants() { return m_constants; }

    // queue all mesh instances for rendering
    void queueForRendering(Gfx::RenderQueue* pRenderQueue);

    // add/remove mesh instance providers
    void addRenderableProvider(Gfx::RenderableProvider*);
    void removeRenderableProvider(Gfx::RenderableProvider*);

    TD_DECLARE_ALLOCATOR();

protected:
    Gfx::Renderable mRenderable;
    Gfx::ShaderConstantTable m_constants;

    typedef std::list<Gfx::RenderableProvider*> RenderableProviders;
    RenderableProviders m_renderableProviders;

    // call this if you change the "lit" properties of the materials for the mesh instance
    void recalculateLighting();

    typedef std::vector<Gfx::Light*> LightList;
    typedef Teardrop::SharedPointer<LightList> LightListPtr;
    LightListPtr m_pLights;

    bool m_bNeedLightsUpdated;


private:
    void onInstanceCreated();
};

} // namespace Teardrop
