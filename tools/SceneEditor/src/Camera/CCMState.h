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

#if !defined(CCMSTATE_INCLUDED)
#define CCMSTATE_INCLUDED

/*
	FSM state for use in the camera controller manager class. Each
	CCMState implementation is responsible for maintaining a particular
	type of camera controller and updating it (including processing 
	input for it, something usually performed by the controller itself).
*/

class CCMState
{
public:
	CCMState(CCMState* pNextState=0);
	virtual ~CCMState();

	virtual size_t getType() const = 0;
	virtual void rotate(int dx, int dy) = 0;
	virtual void dolly(int val) = 0;
	virtual void strafe(int val) = 0;
	virtual void lift(int val) = 0;

	virtual bool handleInput(const CoS::InputEvent& event) = 0;

	/*
		Activate the state (usually happens after the state is made current
		but before the first update)
	*/
	virtual void activate() = 0;

	/*
		Deactivate the state (usually happens before the state is destroyed)
	*/
	virtual void deactivate() = 0;

	/*
		Process an update tick. Returns the next state to activate if a state
		change is ready; 0 otherwise.
	*/
	virtual CCMState* update(float deltaT) = 0;

protected:
	CCMState* m_pNextState;
};

#endif // CCMSTATE_INCLUDED
