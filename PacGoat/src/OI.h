#ifndef OI_H
#define OI_H

#include <Buttons/JoystickButton.h>
#include <Joystick.h>

#include "Triggers/DoubleButton.h"

class OI {
public:
	OI();
	frc::Joystick* GetJoystick();

private:
	frc::Joystick joystick { 0 };

	frc::JoystickButton LowGoalButton { &joystick, 1 };
	frc::JoystickButton CollectButton { &joystick, 2 };
	frc::JoystickButton PivotShootButton { &joystick, 3 };
	frc::JoystickButton PivotShootNearButton { &joystick, 4 };

	frc::JoystickButton ForwardButton { &joystick, 5 };
	frc::JoystickButton BackwardButton { &joystick, 7 };

	DoubleButton sticks { &joystick, 11, 12 };
};

#endif
