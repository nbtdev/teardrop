/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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

	virtual bool handleInput(const Teardrop::InputEvent& event) = 0;

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
