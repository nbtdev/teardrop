/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_PLAYERINPUT_INCLUDED)
#define TEARDROP_PLAYERINPUT_INCLUDED

namespace Teardrop
{
	class PlayerInput
	{
	public:
		virtual ~PlayerInput();

		struct Action
		{
			typedef enum
			{
				NONE = 0x00000001,
				FORWARD,
				BACKWARD,
				LEFT,
				RIGHT,
				ROTATELEFT,
				ROTATERIGHT,
				ZOOM,
				RESET,
				LOOK,
				WEAPON1,
				WEAPON2,
				WEAPON3,
				WEAPON4,

				// aliases for stick events
				THROTTLE = BACKWARD,
				TURN = RIGHT,
				TWIST = ROTATERIGHT,
				PITCH = LOOK,
			} Type;

			Type type;
			float extra[3];
			bool bEnd;
		};

		bool getNextAction(Action& /*inout*/ action);
		Action::Type getCurrentActionType();
	};
}

#endif // TEARDROP_PLAYERINPUT_INCLUDED
