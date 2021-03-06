/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "NidecBrushless.h"

#include <HAL/HAL.h>

#include "SmartDashboard/SendableBuilder.h"

using namespace frc;

NidecBrushless::NidecBrushless(int pwmChannel, int dioChannel)
    : m_safetyHelper(this), m_dio(dioChannel), m_pwm(pwmChannel) {
  AddChild(&m_dio);
  AddChild(&m_pwm);
  m_safetyHelper.SetExpiration(0.0);
  m_safetyHelper.SetSafetyEnabled(false);

  // the dio controls the output (in PWM mode)
  m_dio.SetPWMRate(15625);
  m_dio.EnablePWM(0.5);

  HAL_Report(HALUsageReporting::kResourceType_NidecBrushless, pwmChannel);
  SetName("Nidec Brushless", pwmChannel);
}

void NidecBrushless::Set(double speed) {
  if (!m_disabled) {
    m_speed = speed;
    m_dio.UpdateDutyCycle(0.5 + 0.5 * (m_isInverted ? -speed : speed));
    m_pwm.SetRaw(0xffff);
  }
  m_safetyHelper.Feed();
}

double NidecBrushless::Get() const { return m_speed; }

void NidecBrushless::SetInverted(bool isInverted) { m_isInverted = isInverted; }

bool NidecBrushless::GetInverted() const { return m_isInverted; }

void NidecBrushless::Disable() {
  m_disabled = true;
  m_dio.UpdateDutyCycle(0.5);
  m_pwm.SetDisabled();
}

void NidecBrushless::StopMotor() {
  m_dio.UpdateDutyCycle(0.5);
  m_pwm.SetDisabled();
}

void NidecBrushless::Enable() { m_disabled = false; }

void NidecBrushless::PIDWrite(double output) { Set(output); }

void NidecBrushless::SetExpiration(double timeout) {
  m_safetyHelper.SetExpiration(timeout);
}

double NidecBrushless::GetExpiration() const {
  return m_safetyHelper.GetExpiration();
}

bool NidecBrushless::IsAlive() const { return m_safetyHelper.IsAlive(); }

void NidecBrushless::SetSafetyEnabled(bool enabled) {
  m_safetyHelper.SetSafetyEnabled(enabled);
}

bool NidecBrushless::IsSafetyEnabled() const {
  return m_safetyHelper.IsSafetyEnabled();
}

void NidecBrushless::GetDescription(wpi::raw_ostream& desc) const {
  desc << "Nidec " << GetChannel();
}

int NidecBrushless::GetChannel() const { return m_pwm.GetChannel(); }

void NidecBrushless::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("Nidec Brushless");
  builder.SetSafeState([=]() { StopMotor(); });
  builder.AddDoubleProperty("Value", [=]() { return Get(); },
                            [=](double value) { Set(value); });
}
