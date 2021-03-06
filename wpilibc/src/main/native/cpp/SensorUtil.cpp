/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "SensorUtil.h"

#include <HAL/AnalogInput.h>
#include <HAL/AnalogOutput.h>
#include <HAL/DIO.h>
#include <HAL/HAL.h>
#include <HAL/PDP.h>
#include <HAL/PWM.h>
#include <HAL/Ports.h>
#include <HAL/Relay.h>
#include <HAL/Solenoid.h>

using namespace frc;

const int SensorUtil::kDigitalChannels = HAL_GetNumDigitalChannels();
const int SensorUtil::kAnalogInputs = HAL_GetNumAnalogInputs();
const int SensorUtil::kSolenoidChannels = HAL_GetNumSolenoidChannels();
const int SensorUtil::kSolenoidModules = HAL_GetNumPCMModules();
const int SensorUtil::kPwmChannels = HAL_GetNumPWMChannels();
const int SensorUtil::kRelayChannels = HAL_GetNumRelayHeaders();
const int SensorUtil::kPDPChannels = HAL_GetNumPDPChannels();

int SensorUtil::GetDefaultSolenoidModule() { return 0; }

bool SensorUtil::CheckSolenoidModule(int moduleNumber) {
  return HAL_CheckSolenoidModule(moduleNumber);
}

bool SensorUtil::CheckDigitalChannel(int channel) {
  return HAL_CheckDIOChannel(channel);
}

bool SensorUtil::CheckRelayChannel(int channel) {
  return HAL_CheckRelayChannel(channel);
}

bool SensorUtil::CheckPWMChannel(int channel) {
  return HAL_CheckPWMChannel(channel);
}

bool SensorUtil::CheckAnalogInputChannel(int channel) {
  return HAL_CheckAnalogInputChannel(channel);
}

bool SensorUtil::CheckAnalogOutputChannel(int channel) {
  return HAL_CheckAnalogOutputChannel(channel);
}

bool SensorUtil::CheckSolenoidChannel(int channel) {
  return HAL_CheckSolenoidChannel(channel);
}

bool SensorUtil::CheckPDPChannel(int channel) {
  return HAL_CheckPDPModule(channel);
}
