/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Viewport.h"
#include "RenderTarget.h"
//#include "GfxMeshInstance.h"

using namespace Teardrop;
using namespace Gfx;

Viewport::Viewport()
	: mPos(Vector2::ZERO)
	, mSize(Vector2::ZERO)
	, mPosNorm(Vector2::ZERO)
	, mSizeNorm(Vector2::ZERO)
	, mRT(0)
	, mEnabled(false)
	, mClearEachRender(true)
	//, m_pVisibleMeshes(0)
	//, m_numVisibleMeshes(0)
{
}

Viewport::~Viewport()
{
}

bool Viewport::initialize(RenderTarget* rt)
{
	mRT = rt;
	return true;
}

bool Viewport::destroy()
{
	return true;
}

const Vector2& Viewport::getPosition(bool normalized) const
{
	if (normalized)
		return mPosNorm;
	else
		return mPos;
}

const Vector2& Viewport::getSize(bool normalized) const
{
	if (normalized)
		return mSizeNorm;
	else
		return mSize;
}

void Viewport::setPosition(const Vector2& pos, bool normalized)
{
	mPos = pos;
	if (normalized)
	{
		mPosNorm = pos;
		mPos.x *= (float)mRT->width();
		mPos.y *= (float)mRT->height();
	}
	else
	{
		mPosNorm.x = mPos.x / (float)mRT->width();
		mPosNorm.y = mPos.y / (float)mRT->height();
	}
}

void Viewport::translate(const Vector2& pos, bool normalized)
{
	Vector2 p(pos);

	if (normalized)
	{
		p.x *= (float)mRT->width();
		p.y *= (float)mRT->height();
	}

	setPosition(mPos + p, false);
}

void Viewport::setSize(const Vector2& size, bool normalized)
{
	mSize = size;
	if (normalized)
	{
		mSizeNorm = size;
		mSize.x *= (float)mRT->width();
		mSize.y *= (float)mRT->height();
	}
	else
	{
		mSizeNorm.x = mSize.x / (float)mRT->width();
		mSizeNorm.y = mSize.y / (float)mRT->height();
	}
}

const Matrix44& Viewport::getViewMatrix() const
{
	return mView;
}

const Matrix44& Viewport::getProjectionMatrix() const
{
	return mProj;
}

void Viewport::setViewMatrix(const Matrix44& view)
{
	mView = view;
}

void Viewport::setProjectionMatrix(const Matrix44& proj)
{
	mProj = proj;
}
//
//void Viewport::setVisibleMeshes(const GfxMeshInstance* pMeshes, size_t ct)
//{
//	// if somehow we still have a pointer value, discard the existing block
//	if (m_pVisibleMeshes)
//	{
//		GFX_FREE(m_pVisibleMeshes);
//	}
//
//	m_numVisibleMeshes = ct;
//	m_pVisibleMeshes = 0;
//
//	if (pMeshes && ct)
//	{
//		// copy the mesh instance block
//		m_pVisibleMeshes = (GfxMeshInstance*)GFX_ALLOCATE(sizeof(GfxMeshInstance) * ct);
//		memcpy(m_pVisibleMeshes, pMeshes, sizeof(GfxMeshInstance) * ct);
//	}
//}
//
//const GfxMeshInstance* Viewport::getVisibleMeshes() const
//{
//	return m_pVisibleMeshes;
//}
//
//size_t Viewport::getNumVisibleMeshes() const
//{
//	return m_numVisibleMeshes;
//}

const Gfx::RenderTarget* Viewport::getRenderTarget() const
{
	return mRT;
}

bool Viewport::isEnabled() const
{
	return mEnabled;
}

void Viewport::enable(bool enabled)
{
	mEnabled = enabled;
}

bool Viewport::getClearEachFrame() const
{
	return mClearEachRender;
}

void Viewport::setClearEachFrame(bool clear)
{
	mClearEachRender = clear;
}

void Viewport::updateDimensions()
{
	mPos = Vector2(
		mPosNorm.x * (float)mRT->width(),
		mPosNorm.y * (float)mRT->height());

	mSize = Vector2(
		mSizeNorm.x * (float)mRT->width(),
		mSizeNorm.y * (float)mRT->height());
}
