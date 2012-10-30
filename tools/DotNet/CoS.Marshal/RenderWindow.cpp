/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgements in Readme.txt

This software is provided free-of-charge for non-commercial use, under the 
GNU Lesser General Public License as published by the Free Software Foundation; 
either version 2 of the License, or (at your option) any later version. This 
program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

For commercial applications, contact the Clash Of Steel Team at 
info@clashofsteel.net for commercial licensing terms and information.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include <msclr/marshal.h>
#include "Gfx/include/GfxCommon.h"
#include "Gfx/include/GfxRenderWindow.h"

using namespace CoS;
using namespace Gfx;
using namespace msclr::interop;
//---------------------------------------------------------------------------
RenderWindow::RenderWindow(CoS::GfxRenderWindow* pWindow)
: RenderTarget(pWindow)
{
	m_pWindow = pWindow;
}
//---------------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
}
//---------------------------------------------------------------------------
RenderWindow::!RenderWindow()
{
}
//---------------------------------------------------------------------------
System::Windows::Forms::Control^ RenderWindow::Parent::get()
{
	return m_parent;
}
//---------------------------------------------------------------------------
void RenderWindow::Parent::set(System::Windows::Forms::Control^ parent)
{
	m_parent = parent;
}
//---------------------------------------------------------------------------
void RenderWindow::Resize(int width, int height)
{
	m_pWindow->resizeWindow(width, height);
}
//---------------------------------------------------------------------------
Viewport^ RenderWindow::AddViewport(float x, float y, float w, float h)
{
	return gcnew Viewport(m_pWindow->addViewport(x, y, w, h));
}
