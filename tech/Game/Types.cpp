/******************************************************************************
Copyright (c) 2019 Teardrop Games

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

#include "Game/Types.h"

#include "MACRO.h"
#include "Prop.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Manifest.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "BeamWeapon.h"
#include "VFX/ParticleSystem.h"
#include "TerrainZone.h"
#include "LandscapeScene.h"

namespace Teardrop {

void initializeTypes()
{
    // hacky for static-init registration of these types
    //Teardrop::MACRO _macro;
    Teardrop::Prop _prop;
    Teardrop::SpawnPoint _spawnPoint;
    Teardrop::PointLight _pointLight;
    Teardrop::SpotLight _spotLight;
    Teardrop::DirectionalLight _dirLight;
    Teardrop::TerrainZone _tzone;
    Teardrop::LandscapeScene _lscene;
    //Teardrop::ParticleSystem _pSys;
    Teardrop::BeamWeapon _pBeamWeapon;
}

} // namespace Teardrop
