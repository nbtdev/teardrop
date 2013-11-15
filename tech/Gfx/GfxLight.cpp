/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


#include "GfxLight.h"
#include "GfxCamera.h"
#include "GfxRenderConfig.h"
#include "GfxRenderTarget.h"
#include "GfxRenderer.h"
#include "Math/MathUtil.h"
#include "Util/Environment.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
GfxLight::GfxLight()
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
GfxLight::~GfxLight()
{
	if (m_pShadowTexture)
		Environment::get().pRenderer->destroyRenderTarget(m_pShadowTexture);
}
//---------------------------------------------------------------------------
const Vector4& GfxLight::getPosition() const
{
	return Vector4::ZERO;
}
//---------------------------------------------------------------------------
const Vector4& GfxLight::getDirection() const
{
	return m_direction;
}
//---------------------------------------------------------------------------
GfxLight::Type GfxLight::getType() const
{
	return m_type;
}
//---------------------------------------------------------------------------
float GfxLight::getRange() const
{
	return m_range;
}
//---------------------------------------------------------------------------
void GfxLight::getAttenuation(
	float& /*out*/constant, float& /*out*/linear, float& /*out*/quadratic) const
{
	constant = m_constant;
	linear = m_linear;
	quadratic = m_quadratic;
}
//---------------------------------------------------------------------------
void GfxLight::getSpotParams(
	float& /*out*/innerAngle, float& /*out*/outerAngle) const
{
	innerAngle = m_innerAngle;
	outerAngle = m_outerAngle;
}
//---------------------------------------------------------------------------
void GfxLight::getColor(
	float& /*out*/r, float& /*out*/g, float& /*out*/b) const
{
	r = m_red;
	g = m_green;
	b = m_blue;
}
//---------------------------------------------------------------------------
void GfxLight::getColor(
	unsigned char& /*out*/r, unsigned char& /*out*/g, unsigned char& /*out*/b) const
{
	r = (unsigned char)(255.0f * m_red);
	g = (unsigned char)(255.0f * m_green);
	b = (unsigned char)(255.0f * m_blue);
}
//---------------------------------------------------------------------------
void GfxLight::setDirection(const Vector4& direction)
{
	m_direction = direction;
}
//---------------------------------------------------------------------------
void GfxLight::setType(Type type)
{
	m_type = type;
}
//---------------------------------------------------------------------------
void GfxLight::setRange(float range)
{
	assert(range > 0);
	m_range = range;
}
//---------------------------------------------------------------------------
void GfxLight::setAttenuation(float constant, float linear, float quadratic)
{
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}
//---------------------------------------------------------------------------
void GfxLight::setSpotParams(float innerAngle, float outerAngle)
{
	m_innerAngle = innerAngle;
	m_outerAngle = outerAngle;
}
//---------------------------------------------------------------------------
void GfxLight::setColor(float r, float g, float b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
}
//---------------------------------------------------------------------------
void GfxLight::setColor(unsigned char r, unsigned char g, unsigned char b)
{
	float oo255 = 1 / 255.0f;
	setColor((float)r * oo255, (float)g * oo255, (float)b * oo255);
}
//---------------------------------------------------------------------------
const GfxRenderTarget* GfxLight::getShadowTexture() const
{
	return m_pShadowTexture;
}
//---------------------------------------------------------------------------
GfxRenderTarget* GfxLight::getShadowTexture()
{
	if (!m_pShadowTexture)
	{
		// create one with purely arbitrary cfg values (the renderer will 
		// change the render target as needed)
		GfxRenderConfig cfg;
		cfg.width = 512;
		cfg.height = 512;
		m_pShadowTexture = 
			Environment::get().pRenderer->createRenderTarget(
			cfg, 
			FMT_FLOAT32,
			GfxRenderer::RT_TEXTURE);
	}

	return m_pShadowTexture;
}
//---------------------------------------------------------------------------
void GfxLight::setShadowCamera(GfxCamera* pCam)
{
	m_pShadowCam = pCam;
}
//---------------------------------------------------------------------------
GfxCamera* GfxLight::getShadowCamera()
{
	//if (!m_pShadowCam)
	//{
	//	// create the shadow texture render cam now
	//	m_pShadowCam = createShadowCamera();
	//}

	return m_pShadowCam;
}
//---------------------------------------------------------------------------
const GfxCamera* GfxLight::getShadowCamera() const
{
	return m_pShadowCam;
}
//---------------------------------------------------------------------------
void GfxLight::createShadowCamera()
{
	if (m_pShadowCam)
	{
		// delete existing one
		m_pShadowCam->destroy();
		delete m_pShadowCam;

		m_pShadowCam = TD_NEW GfxCamera;
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
