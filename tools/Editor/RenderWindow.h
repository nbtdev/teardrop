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

#include <QObject>
#include <QWidget>
#include <memory>

class QTimer;

namespace Teardrop {

class Executable;

namespace Gfx {
class Renderer;
class RenderTarget;
} // namespace Gfx

namespace Tools {

class RenderWindow : public QWidget
{
    Q_OBJECT

public:
    RenderWindow(Gfx::Renderer* renderer, QWidget* parent=nullptr);
    ~RenderWindow();

    void setExecutable(Executable* executable);

protected: // QWidget event overrides
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

protected slots:
    void onIdle();

private:
    QTimer* mTimer;
    Gfx::Renderer* mRenderer;
    std::shared_ptr<Gfx::RenderTarget> mRT;
    Executable* mExecutable;
    int mLastMouseX;
    int mLastMouseY;
};

} // namespace Tools
} // namespace Teardrop
