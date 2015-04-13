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
