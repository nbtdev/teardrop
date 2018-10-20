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

#include "LandscapeScene.h"

#include "Asset/LandscapeAsset.h"
#include "Game/SceneRenderer.h"
#include "Game/SceneRenderStep.h"
#include "Game/ZoneObjects.h"
#include "Gfx/Camera.h"
#include "Gfx/IndexBuffer.h"
#include "Gfx/Mesh.h"
#include "Gfx/SubMesh.h"
#include "Gfx/Renderable.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Gfx/Viewport.h"
#include "Math/AABB.h"
#include "Math/MathUtil.h"
#include "Math/Vector4.h"

namespace {

using namespace Teardrop;

struct TerrainRenderable : public Gfx::Renderable
{
    TerrainRenderable(){}
    TerrainRenderable(TerrainRenderable&& other) {
        mMesh = other.mMesh;
        mMaterial = other.mMaterial;
        mTransform = std::move(other.mTransform);

        other.mMesh = nullptr;
        other.mMaterial = nullptr;
    }

    ~TerrainRenderable() {
        delete mMesh;
        delete mMaterial;
    }

    Gfx::Mesh* mMesh;
    Gfx::Material* mMaterial;
    Transform mTransform;
    AABB mAABB;
};

struct TerrainVertex
{
    float x, y, z;
    //float nx, ny, nz;
    //float u1, v1; // diffuse map
    //float u2, v2; // detail map

    TD_DECLARE_ALLOCATOR();
};

Gfx::Material* createTerrainMaterial(LandscapeAsset* landscapeAsset, Gfx::VertexBuffer* vertexBuffer)
{
    return nullptr;
}

void createTerrainPatch(TerrainRenderable& renderable, int x, int y, LandscapeScene* scene)
{
    float xStep, zStep; // one-over the width and height of the patch
    float uStart, vStart; // starting U and V coordinates for the patch
    float uStep, vStep; // one-over the width and height of the terrain
    Vector4 scale = Vector4::UNIT_SCALE; // world size of a patch

    LandscapeAsset* landscapeAsset = scene->getLandscapeAsset();
    TextureAsset* hf = landscapeAsset->getHeightField();
    int w = hf->getWidth();
    int h = hf->getHeight();
    int patchesX = (int)scene->getTiling().x;
    int patchesY = (int)scene->getTiling().y;
    size_t tileWidth = (size_t)(w / patchesX);
    size_t tileHeight = (size_t)(h / patchesY);
    size_t patchWidth = tileWidth + 1;
    size_t patchHeight = tileHeight + 1;

    Vector4& worldSize = scene->getSize();
    if (worldSize != Vector4::ZERO) {
        scale.x = worldSize.x / (float)w;
        scale.z = worldSize.z / (float)h;

        float altitudeSpan = landscapeAsset->getMaxAltitude() - landscapeAsset->getMinAltitude();
        scale.y = worldSize.y / altitudeSpan;
    }

    xStep = 1.0f / (float)tileWidth;
    zStep = 1.0f / (float)tileHeight;
    float* pSrc = (float*)hf->data();
    size_t imgWidth = hf->getWidth();
    size_t imgHeight = hf->getHeight();
    uStep = 1.f / (float)imgWidth;
    vStep = 1.f / (float)imgHeight;
    size_t xOff = x * tileWidth;
    size_t yOff = y * tileHeight;
    uStart = (float)xOff / (float)hf->getWidth();
    vStart = (float)yOff / (float)hf->getHeight();

    Gfx::Mesh* mesh = TD_NEW Gfx::Mesh;
    // this looks a little strange, but TerrainRenderable inherits from Renderable
    // so that we have somewhere to hang the mesh and material pointers; Renderable
    // has its data members private on purpose, so we populate the "external" Renderable
    // via its setter methods
    renderable.mMesh = mesh;
    renderable.setMesh(mesh);

    Gfx::Submesh* patchMesh = mesh->createSubmesh();

    // create the vertex data buffer and format
    Gfx::VertexBuffer* vb = patchMesh->createVertexBuffer();

    vb->beginAddVertexElements();
    Gfx::VertexElement& posElem = vb->addVertexElement();
    posElem.mIndex = 0;
    posElem.mUsage = Gfx::VEU_POSITION;
    posElem.mType = Gfx::VET_FLOAT;
    posElem.mCount = 3;

# if 0
    Gfx::VertexElement& normElem = vb->addVertexElement();
    normElem.mIndex = 0;
    normElem.mUsage = Gfx::VEU_NORMAL;
    normElem.mType = Gfx::VET_FLOAT;
    normElem.mCount = 3;
#endif

    vb->endAddVertexElements();

    // TODO: add UVs for different maps?

    size_t numVerts = patchWidth * patchHeight;
    vb->initialize((int)numVerts, Gfx::VertexBuffer::INIT_WRITEONLY);

    Gfx::Material* mtl = createTerrainMaterial(landscapeAsset, vb);
    renderable.mMaterial = mtl;
    renderable.addMaterial(mtl);

    float minY = 0, maxY = 0;

    TerrainVertex* vertexData = (TerrainVertex*)vb->map(Gfx::VertexBuffer::MAP_DISCARD);
    for (size_t py = 0; py < patchHeight; ++py) {
        for (size_t px = 0; px < patchWidth; ++px) {
            size_t i = py * patchWidth + px;
            TerrainVertex& terrainVertex = vertexData[i];

            size_t globalX = xOff + px;
            size_t globalY = yOff + py;

            // clamp the bottom and right edges to the last texels on those edges
            size_t effectiveX = globalX;
            if (effectiveX >= imgWidth) {
                effectiveX = imgWidth - 1;
            }

            size_t effectiveY = globalY;
            if (effectiveY >= imgHeight) {
                effectiveY = imgHeight - 1;
            }

            float h = pSrc[effectiveY * imgWidth + effectiveX];

            // calculate vert position and UV data; height is already in full (not
            // normalized) single-precision form
            terrainVertex.y = h;
            terrainVertex.x = (float)px;
            terrainVertex.z = (float)py;

            minY = MathUtil::min(terrainVertex.y, minY);
            maxY = MathUtil::max(terrainVertex.y, maxY);
        }
    }

    // TODO: normals are calculated in the vertex shader?
#if 0
    for (size_t ny = 0; ny < patchHeight; ++ny)	{
        for (size_t nx = 0; nx < patchWidth; ++nx) {
            //Vector4 n(Vector4::ZERO);
            //size_t i = ny + nx * patchWidth;

            if (ny < tileHeight) {
                // then we know there is one below this row
                //n += Vector4(xScale * m_pVertexBuffer[i].x, )
            }
        }
    }
#endif

    vb->unmap();

    renderable.mAABB.extents = Vector4(
        scale.x, (maxY-minY), scale.z, 0) / 2;

    Vector4 ulCorner(
        -worldSize.x / 2 + (float)x * scale.x,
        minY,
        -worldSize.z / 2 + (float)y * scale.z,
        0);

    renderable.mAABB.center = renderable.mAABB.extents + ulCorner;

    // TODO: normals

    // make triangle lists in the index buffer
    /**
        Terrain patch triangles look like this:

        0 - 1 - 2 - 3 ... 63
        |  /|  /|  /|
        | / | / | / |
        |/  |/  |/  |
       64--65--66--67 ... 127

       So the index marching order (CW winding) is

       ( y*w + x )    ( (y+1)*w + x )   ( y*w + x+1 )
       ( y*w + x+1 )  ( (y+1)*w + x )   ( (y+1)*w + x+1 )

       where w = patch width

   */

    size_t numIndices = tileHeight * tileWidth * 6;
    Gfx::IndexBuffer* indexBuffer = patchMesh->createIndexBuffer();
    indexBuffer->initialize((int)numIndices, Gfx::IndexBuffer::INIT_WRITEONLY);
    short* pI = (short*)indexBuffer->map(Gfx::IndexBuffer::MAP_WRITEONLY);

    for (size_t py = 0; py < (patchHeight-1); ++py) {
        for (size_t px = 0; px < (patchWidth-1); ++px) {
            *pI++ = short(py		* patchWidth + px		); // 0
            *pI++ = short((py + 1)	* patchWidth + px		); // 64
            *pI++ = short(py		* patchWidth + px + 1	); // 1
            *pI++ = short((py + 1)	* patchWidth + px + 1	); // 64
            *pI++ = short(py		* patchWidth + px + 1	); // 1
            *pI++ = short((py + 1)	* patchWidth + px		); // 65
        }
    }

    indexBuffer->unmap();

    Transform xform(
        ulCorner,
        Quaternion::IDENTITY,
        Vector4::UNIT_SCALE);

    renderable.setTransform(xform);

    // TODO: set up shader constants in the renderable...
}

} // namespace

TD_CLASS_IMPL(LandscapeScene);

namespace Teardrop {

LandscapeScene::LandscapeScene()
    : mSceneRenderer(nullptr)
    , mSceneRenderStep(nullptr)
    , mTerrainRenderables(nullptr)
    , mBoundingBoxes(nullptr)
    , mTerrainRenderableCount(0)
{
}

LandscapeScene::~LandscapeScene()
{
    // the Renderables do not own their contents, so we need to
    // delete any meshes/materials we may have created
    for (size_t i = 0; i < mTerrainRenderableCount; ++i) {
        Gfx::Renderable& renderable = mTerrainRenderables[i];
        Gfx::Mesh* mesh = renderable.mesh();

        for (size_t s = 0; s < (size_t)mesh->submeshCount(); ++s) {
            delete renderable.material(s);
        }

        delete mesh;
    }

    delete [] mTerrainRenderables;
    delete [] mBoundingBoxes;
    delete mSceneRenderer;
}

bool LandscapeScene::initialize()
{
	return true;
}

bool LandscapeScene::destroy()
{
	return true;
}

bool LandscapeScene::onPreLoad()
{
    Logic* logic = getLogic();
    if (logic) {
        // invoke the logic first
        logic->onPreLoad(this);
    }

    return true;
}

bool LandscapeScene::onPostLoad()
{
    // do things to finalize scene loading
    generateTerrainTiles();

    // then invoke the logic, if set
    Logic* logic = getLogic();
    if (logic) {
        logic->onPostLoad(this);
    }

    mSceneRenderer = new SceneRenderer;
    mSceneRenderStep = new SceneRenderStep;
    mSceneRenderer->addStep(mSceneRenderStep);

    return true;
}

void LandscapeScene::tick()
{
    Logic* logic = getLogic();
    if (logic) {
        // tick the logic first
        logic->update(this);
    }

    // update anything else that needs updated
}

void LandscapeScene::renderFrame(Gfx::Renderer* renderer, Gfx::RenderTarget* rt)
{
    CameraController* camControl = getCameraController();
    if (!camControl) {
        return;
    }

    Gfx::Camera* camera = camControl->camera();
    if (!camera) {
        return;
    }

    mSceneRenderStep->setRenderTarget(rt);

    ZoneObjects visibleObjects;
    camera->setAspect(rt->aspect());
    getVisibleObjects(camera->getFrustumPlanes(), visibleObjects);

    mSceneRenderer->render(visibleObjects, renderer, this, camera);

    rt->present();
}

bool LandscapeScene::onPreUnload()
{
    delete mSceneRenderer;
    mSceneRenderer = nullptr;

    Logic* logic = getLogic();
    if (logic) {
        // invoke the logic first
        logic->onPreUnload(this);
    }

    // then start unloading ourselves
    return true;
}

bool LandscapeScene::onPostUnload()
{
    // do things to finalize scene unloading

    // then invoke the logic, if set
    Logic* logic = getLogic();
    if (logic) {
        logic->onPostUnload(this);
    }

    return true;
}

void LandscapeScene::injectMouseMove(int absX, int absY, int relX, int relY)
{
    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseMove(absX, absY, relX, relY);
    }
}

void LandscapeScene::injectMouseWheel(int absZ, int relZ)
{
    CameraController* camControl = getCameraController();
    if (camControl) {
        camControl->injectMouseWheel(absZ, relZ);
    }
}

void LandscapeScene::generateTerrainTiles()
{
    LandscapeAsset* asset = getLandscapeAsset();
    if (!asset) {
        return;
    }

    TextureAsset* hf = asset->getHeightField();
    if (!hf) {
        return;
    }

    int nTilesX = (int)getTiling().x;
    int nTilesY = (int)getTiling().y;

    mTerrainRenderableCount = nTilesX * nTilesY;
    mTerrainRenderables = new TerrainRenderable[mTerrainRenderableCount];

    for (int y = 0; y < nTilesY; ++y) {
        for (int x = 0; x < nTilesX; ++x) {
            TerrainRenderable* terrainRenderables = (TerrainRenderable*)mTerrainRenderables;
            createTerrainPatch(terrainRenderables[y * nTilesX + x], x, y, this);
        }
    }
}

} // namespace Teardrop
