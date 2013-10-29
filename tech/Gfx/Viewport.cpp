/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Viewport.h"
#include "RenderTarget.h"
#include <assert.h>

using namespace Teardrop;
using namespace Gfx;

Viewport::Viewport(RenderTarget* rt)
	: mPos(Vector2::ZERO)
	, mSize(Vector2::ZERO)
	, mPosNorm(Vector2::ZERO)
	, mSizeNorm(Vector2::ZERO)
	, mRT(rt)
	, mEnabled(false)
	, mClearEachRender(true)
{
	assert(rt);
}

Viewport::~Viewport()
{
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

	assert(mRT);
	if (mRT) {
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
}

void Viewport::translate(const Vector2& pos, bool normalized)
{
	assert(mRT);
	if (mRT) {
		Vector2 p(pos);

		if (normalized)
		{
			p.x *= (float)mRT->width();
			p.y *= (float)mRT->height();
		}

		setPosition(mPos + p, false);
	}
}

void Viewport::setSize(const Vector2& size, bool normalized)
{
	mSize = size;

	assert(mRT);
	if (mRT) {
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
}

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
	assert(mRT);
	if (mRT) {
		mPos = Vector2(
			mPosNorm.x * (float)mRT->width(),
			mPosNorm.y * (float)mRT->height());

		mSize = Vector2(
			mSizeNorm.x * (float)mRT->width(),
			mSizeNorm.y * (float)mRT->height());
	}
}
