/******************************************************************************
Copyright (c) 2015 Teardrop Games

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


#include "Light.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Renderer.h"
#include "Math/MathUtil.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;
//---------------------------------------------------------------------------
Light::Light()
{
	// defaults: white point light with range 1000 and no attentuation
	m_type = LT_POINT;
	m_range = 1000;
	m_constant = 1;
	m_linear = 0;
	m_quadratic = 0;
	m_innerAngle = MathUtil::HALF_PI / 2;
	m_outerAngle = MathUtil::HALF_PI;
	m_red = 1;
	m_green = 1;
	m_blue = 1;
	m_direction = Vector4(0, 0, 1, 0);

	m_pShadowTexture = 0;
	m_pShadowCam = 0;
}
//---------------------------------------------------------------------------
Light::~Light()
{
//	if (m_pShadowTexture)
//		Environment::get().pRenderer->destroyRenderTarget(m_pShadowTexture);
}
//---------------------------------------------------------------------------
const Vector4& Light::getPosition() const
{
	return Vector4::ZERO;
}
//---------------------------------------------------------------------------
const Vector4& Light::getDirection() const
{
	return m_direction;
}
//---------------------------------------------------------------------------
Light::Type Light::getType() const
{
	return m_type;
}
//---------------------------------------------------------------------------
float Light::getRange() const
{
	return m_range;
}
//---------------------------------------------------------------------------
void Light::getAttenuation(
	float& /*out*/constant, float& /*out*/linear, float& /*out*/quadratic) const
{
	constant = m_constant;
	linear = m_linear;
	quadratic = m_quadratic;
}
//---------------------------------------------------------------------------
void Light::getSpotParams(
	float& /*out*/innerAngle, float& /*out*/outerAngle) const
{
	innerAngle = m_innerAngle;
	outerAngle = m_outerAngle;
}
//---------------------------------------------------------------------------
void Light::getColor(
	float& /*out*/r, float& /*out*/g, float& /*out*/b) const
{
	r = m_red;
	g = m_green;
	b = m_blue;
}
//---------------------------------------------------------------------------
void Light::getColor(
	unsigned char& /*out*/r, unsigned char& /*out*/g, unsigned char& /*out*/b) const
{
	r = (unsigned char)(255.0f * m_red);
	g = (unsigned char)(255.0f * m_green);
	b = (unsigned char)(255.0f * m_blue);
}
//---------------------------------------------------------------------------
void Light::setDirection(const Vector4& direction)
{
	m_direction = direction;
}
//---------------------------------------------------------------------------
void Light::setType(Type type)
{
	m_type = type;
}
//---------------------------------------------------------------------------
void Light::setRange(float range)
{
	assert(range > 0);
	m_range = range;
}
//---------------------------------------------------------------------------
void Light::setAttenuation(float constant, float linear, float quadratic)
{
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}
//---------------------------------------------------------------------------
void Light::setSpotParams(float innerAngle, float outerAngle)
{
	m_innerAngle = innerAngle;
	m_outerAngle = outerAngle;
}
//---------------------------------------------------------------------------
void Light::setColor(float r, float g, float b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
}
//---------------------------------------------------------------------------
void Light::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	float oo255 = 1 / 255.0f;
	setColor((float)r * oo255, (float)g * oo255, (float)b * oo255);
}
//---------------------------------------------------------------------------
const Gfx::RenderTarget* Light::getShadowTexture() const
{
	return m_pShadowTexture;
}
//---------------------------------------------------------------------------
Gfx::RenderTarget* Light::getShadowTexture()
{
	if (!m_pShadowTexture)
	{
		// create one with purely arbitrary cfg values (the renderer will 
		// change the render target as needed)
//		GfxRenderConfig cfg;
//		cfg.width = 512;
//		cfg.height = 512;
//		m_pShadowTexture =
//			Environment::get().pRenderer->createRenderTarget(
//			cfg,
//			FMT_FLOAT32,
//			GfxRenderer::RT_TEXTURE);
	}

	return m_pShadowTexture;
}
//---------------------------------------------------------------------------
void Light::setShadowCamera(Gfx::Camera* pCam)
{
	m_pShadowCam = pCam;
}
//---------------------------------------------------------------------------
Gfx::Camera* Light::getShadowCamera()
{
	//if (!m_pShadowCam)
	//{
	//	// create the shadow texture render cam now
	//	m_pShadowCam = createShadowCamera();
	//}

	return m_pShadowCam;
}
//---------------------------------------------------------------------------
const Gfx::Camera* Light::getShadowCamera() const
{
	return m_pShadowCam;
}
//---------------------------------------------------------------------------
void Light::createShadowCamera()
{
	if (m_pShadowCam)
	{
		// delete existing one
		m_pShadowCam->destroy();
		delete m_pShadowCam;

        m_pShadowCam = TD_NEW Gfx::Camera;
		m_pShadowCam->initialize();
	}

	// figure out what type of camera to make for the light type;
	// point lights and spot lights will use standard perspective cameras,
	// while directional lights will use an orthographic camera
	if (m_type == LT_DIRECTIONAL)
	{
		
	}
	else
	{

	}
}
