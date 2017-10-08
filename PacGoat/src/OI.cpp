#include "OI.h"

#include "Commands/Collect.h"
#include "Commands/DriveForward.h"
#include "Commands/LowGoal.h"
#include "Commands/SetCollectionSpeed.h"
#include "Commands/SetPivotSetpoint.h"
#include "Commands/Shoot.h"
#include "Subsystems/Collector.h"
#include "Subsystems/Pivot.h"

OI::OI() {
	LowGoalButton.WhenPressed(new LowGoal());
	CollectButton.WhenPressed(new Collect());

	PivotShootButton.WhenPressed(new SetPivotSetpoint(Pivot::kShoot));
	PivotShootNearButton.WhenPressed(new SetPivotSetpoint(Pivot::kShootNear));

        ForwardButton.WhenPressed(new DriveForward(1.0));
        BackwardButton.WhenPressed(new DriveForward(-1.0));

	sticks.WhenActive(new Shoot());

        /* TODO - this appears to be a bug in the sample code.  The OI
                  initialization preceeds the call to SmartDashboard::init
                  that occurs during the RobotBase initialization */
	frc::SmartDashboard::init(); 

	// SmartDashboard Buttons
	frc::SmartDashboard::PutData("Drive Forward", new DriveForward(2.25));
	frc::SmartDashboard::PutData("Drive Backward", new DriveForward(-2.25));
	frc::SmartDashboard::PutData("Start Rollers", new SetCollectionSpeed(Collector::kForward));
	frc::SmartDashboard::PutData("Stop Rollers", new SetCollectionSpeed(Collector::kStop));
	frc::SmartDashboard::PutData("Reverse Rollers", new SetCollectionSpeed(Collector::kReverse));
}

frc::Joystick* OI::GetJoystick() {
	return &joystick;
}