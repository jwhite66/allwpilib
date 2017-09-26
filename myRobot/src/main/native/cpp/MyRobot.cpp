/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>

#include "WPILib.h"

class MyRobot : public IterativeRobot {
  Joystick* stick;
  /**
   * This function is run when the robot is first started up and should be
   * used for any initialization code.
   */
  void RobotInit() {
    stick = new Joystick(0);
    std::cout << "JPW " << __func__ << std::endl;
  }

  /**
   * This function is run once each time the robot enters autonomous mode
   */
  void AutonomousInit() { std::cout << "JPW " << __func__ << std::endl; }

  /**
   * This function is called periodically during autonomous
   */
  void AutonomousPeriodic() override {}

  /**
   * This function is called once each time the robot enters tele-operated mode
   */
  void TeleopInit() { std::cout << "JPW " << __func__ << std::endl; }

  /**
   * This function is called periodically during operator control
   */
  void TeleopPeriodic() {
    std::cout << "Joystick Axes: [ ";
    for (int i = 0; i < stick->GetAxisCount(); i++)
      std::cout << stick->GetRawAxis(i) << " ";
    std::cout << "] POV [ ";
    for (int i = 0; i < stick->GetPOVCount(); i++)
      std::cout << stick->GetPOV(i) << " ";
    std::cout << "] Buttons [ ";
    for (int i = 0; i < stick->GetButtonCount(); i++)
      if (stick->GetRawButton(i + 1))
        std::cout << "+"
                  << " ";
      else
        std::cout << "-"
                  << " ";
    std::cout << "]" << std::endl;
  }

  /**
   * This function is called periodically during test mode
   */
  void TestPeriodic() override {}

  /**
   * This function is called periodically during all modes
   */
  void RobotPeriodic() override {}
};

START_ROBOT_CLASS(MyRobot)
