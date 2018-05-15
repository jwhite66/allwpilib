/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Commands/DriveWithJoystick.h"

#include "Robot.h"

DriveWithJoystick::DriveWithJoystick() { Requires(&Robot::drivetrain); }

// Called repeatedly when this Command is scheduled to run
void DriveWithJoystick::Execute() {
  auto& joystick = Robot::oi.GetJoystick();
  // jwhite@codeweavers.com - shifting to use Logitech joystick
  // Robot::drivetrain.TankDrive(joystick.GetY(), joystick.GetRawAxis(4));
  Robot::drivetrain.TankDrive(joystick.GetRawAxis(1), joystick.GetRawAxis(3));
}

// Make this return true when this Command no longer needs to run execute()
bool DriveWithJoystick::IsFinished() { return false; }

// Called once after isFinished returns true
void DriveWithJoystick::End() { Robot::drivetrain.Stop(); }
