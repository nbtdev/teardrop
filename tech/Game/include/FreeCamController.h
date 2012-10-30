/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(FREECAMCONTROLLER_INCLUDED)
#define FREECAMCONTROLLER_INCLUDED

#include "Game/include/CameraController.h"
#include "Memory/include/Allocators.h"

namespace CoS
{
	/*
		Free-camera implementation class
	*/

	class FreeCamInputFilter;
	class InputFilter;

	class FreeCamController 
		: public CameraController
	{
		FreeCamInputFilter* m_pInputFilter;
		float m_move;
		float m_strafe;
		float m_zoom;
		float m_rotX;
		float m_rotY;

	public:
		COS_CLASS(FreeCamController, CameraController);

		FreeCamController();
		virtual ~FreeCamController();

		/*
			CameraController implementation
		*/
		bool initialize();
		bool destroy();
		bool update(float deltaT);
		InputFilter* getInputFilter() const;

		void reset();

		COS_DECLARE_ALLOCATOR();

	private:
		void evaluate(float deltaT);
	};
}

#endif // FREECAMCONTROLLER_INCLUDED
