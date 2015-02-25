/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderWindowOpenGL.h"
#include "Gfx/Common.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

RenderWindow::RenderWindow(Display* aDisplay)
    : RenderWindow(aDisplay, 0, 0)
{
}

RenderWindow::RenderWindow(Display* aDisplay, int aFlags)
    : RenderWindow(aDisplay, 0, aFlags)
{
}

RenderWindow::RenderWindow(Display* aDisplay, Window aParent)
    : RenderWindow(aDisplay, aParent, 0)
{
}

namespace {
    Bool
    WaitForNotify(Display* aDisplay, XEvent* aEvent, XPointer aArg)
    {
        return (aEvent->type == MapNotify) && (aEvent->xmap.window == (Window)aArg);
    }
} // namespace

RenderWindow::RenderWindow(Display* aDisplay, Window aParent, int aFlags)
    : mDisplay(aDisplay)
    , mParent(aParent)
    , mInitFlags(aFlags)
{
    // use GLX to create a context into the provided window
    int attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
    };

    int nItems;
    GLXFBConfig* config = glXChooseFBConfig(mDisplay, DefaultScreen(mDisplay), attribs, &nItems);
    if (config) {
        XVisualInfo* vis = glXGetVisualFromFBConfig(mDisplay, config[0]);

        XSetWindowAttributes swa;
        swa.colormap = XCreateColormap(mDisplay, RootWindow(mDisplay, vis->screen), vis->visual, AllocNone);
        swa.border_pixel = 0;
        swa.event_mask = StructureNotifyMask;

        if (mParent) {
            // embed the context window into the supplied parent
            XWindowAttributes attr;
            Status s = XGetWindowAttributes(mDisplay, mParent, &attr);

            if (s) {
                mWidth = attr.width;
                mHeight = attr.height;
            }
        }

        // create a new window for the context, possibly at the screen root if no parent was supplied
        mWindow = XCreateWindow(
                    mDisplay,
                    mParent == 0 ? RootWindow(mDisplay, vis->screen) : mParent,
                    -1, -1,
                    mParent == 0 ? -1 : mWidth,
                    mParent == 0 ? -1 : mHeight,
                    0,
                    vis->depth,
                    InputOutput,
                    vis->visual,
                    CWBorderPixel|CWColormap|CWEventMask,
                    &swa
                );

        mCtx = glXCreateNewContext(mDisplay, config[0], GLX_RGBA_TYPE, NULL, True);
        mGLXWindow = glXCreateWindow(mDisplay, config[0], mWindow, NULL);
        XMapWindow(mDisplay, mWindow);

        XEvent event;
        XIfEvent(mDisplay, &event, WaitForNotify, (XPointer)mWindow);

        glXMakeContextCurrent(mDisplay, mGLXWindow, mGLXWindow, mCtx);
    }
}

RenderWindow::~RenderWindow()
{
    glXDestroyContext(mDisplay, mCtx);
    glXDestroyWindow(mDisplay, mGLXWindow);
}

void RenderWindow::resize(int w, int h)
{
#if 0
    assert(mHwnd);

    if (mSurface) {
        mSurface->Release();
        mSurface = 0;
    }

    if (mDepthStencil) {
        mDepthStencil->Release();
        mDepthStencil = 0;
    }

    if (mSwapChain) {
        mSwapChain->Release();
        mSwapChain = 0;
    }

    mWidth = w;
    mHeight = h;

    ZeroMemory(&mPParams, sizeof(mPParams));
    mPParams.Windowed = TRUE;
    mPParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    mPParams.BackBufferFormat = D3DFMT_UNKNOWN;
    mPParams.hDeviceWindow = mHwnd;
    mPParams.BackBufferWidth = mWidth;
    mPParams.BackBufferHeight = mHeight;
    mPParams.BackBufferCount = 1;
    mPParams.EnableAutoDepthStencil = TRUE;

    D3DFORMAT dsFmt = D3DFMT_UNKNOWN;

    if (mInitFlags & (INIT_ENABLE_DEPTH_BUFFER | INIT_ENABLE_STENCIL_BUFFER))
            dsFmt = D3DFMT_D24S8;

    mPParams.AutoDepthStencilFormat = dsFmt;

    mPParams.PresentationInterval =
        (mInitFlags & INIT_ENABLE_VSYNC) ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr = mDevice->CreateAdditionalSwapChain(&mPParams, &mSwapChain);

    if (SUCCEEDED(hr)) {
        // get render target surface ref
        mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);

        // create depth buffer for additional swap chain
        hr = mDevice->CreateDepthStencilSurface(
            mPParams.BackBufferWidth,
            mPParams.BackBufferHeight,
            mPParams.AutoDepthStencilFormat,
            D3DMULTISAMPLE_NONE, // todo: support AA at some point
            0,
            FALSE,
            &mDepthStencil,
            NULL);
    }

    assert(mSurface);
    assert(mDepthStencil);
    assert(mSwapChain);
#endif
}

void RenderWindow::present()
{
    glXSwapBuffers(mDisplay, mWindow);
}

void
RenderWindow::setCurrent()
{
    glXMakeContextCurrent(mDisplay, mGLXWindow, mGLXWindow, mCtx);
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
