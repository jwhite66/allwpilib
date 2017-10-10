#include "Robot.h"

#include <iostream>

#include <SmartDashboard/SmartDashboard.h>

std::shared_ptr<DriveTrain> Robot::drivetrain;
std::shared_ptr<Pivot> Robot::pivot;
std::shared_ptr<Collector> Robot::collector;
std::shared_ptr<Shooter> Robot::shooter;
std::shared_ptr<Pneumatics> Robot::pneumatics;
std::unique_ptr<OI> Robot::oi;

void Robot::RobotInit() {
        drivetrain = std::make_shared<DriveTrain>();
        pivot = std::make_shared<Pivot>();
        collector = std::make_shared<Collector>();
        shooter = std::make_shared<Shooter>();
        pneumatics = std::make_shared<Pneumatics>();
        oi = std::make_unique<OI>();

        driveAndShootAuto = new DriveAndShootAutonomous();
        driveForwardAuto = new DriveForward();

	// Show what command your subsystem is running on the SmartDashboard
	frc::SmartDashboard::init();
	frc::SmartDashboard::PutData(drivetrain.get());
	frc::SmartDashboard::PutData(pivot.get());
	frc::SmartDashboard::PutData(collector.get());
	frc::SmartDashboard::PutData(shooter.get());
	frc::SmartDashboard::PutData(pneumatics.get());

	// instantiate the command used for the autonomous period
	autoChooser.AddDefault("Drive and Shoot", driveAndShootAuto);
	autoChooser.AddObject("Drive Forward", driveForwardAuto);
	frc::SmartDashboard::PutData("Auto Mode", &autoChooser);

	std::cout << __func__ << std::endl;
	pneumatics->Start();  // Pressurize the pneumatics.
}

void Robot::AutonomousInit() {
	std::cout << __func__ << std::endl;
	autonomousCommand = autoChooser.GetSelected();
	autonomousCommand->Start();
}

void Robot::AutonomousPeriodic() {
	frc::Scheduler::GetInstance()->Run();
	Log();
}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// this line or comment it out.
	if (autonomousCommand != nullptr) {
		autonomousCommand->Cancel();
	}
	std::cout << "Starting Teleop" << std::endl;
}

void Robot::TeleopPeriodic() {
	frc::Scheduler::GetInstance()->Run();
	Log();
}

void Robot::TestPeriodic() {
	frc::LiveWindow::GetInstance()->Run();
}

void Robot::DisabledInit() {
	shooter->Unlatch();
}

void Robot::DisabledPeriodic() {
	Log();
}

/**
 * Log interesting values to the SmartDashboard.
 */
void Robot::Log() {
	Robot::pneumatics->WritePressure();
	frc::SmartDashboard::PutNumber("Pivot Pot Value", pivot->GetAngle());
	frc::SmartDashboard::PutNumber("Left Distance",
			drivetrain->GetLeftEncoder()->GetDistance());
	frc::SmartDashboard::PutNumber("Right Distance",
			drivetrain->GetRightEncoder()->GetDistance());
}

START_ROBOT_CLASS(Robot)
