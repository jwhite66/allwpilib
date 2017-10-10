/*----------------------------------------------------------------------------*/
/* Copyright (c) 2014-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "HALSimGazebo.h"
#include "simulation/gz_msgs/msgs.h"

class GazeboAnalogIn {
 public:
  GazeboAnalogIn(int index, HALSimGazebo* halsim);
  void SetInitialized(bool value) { m_initialized = value; }
  bool IsInitialized(void) { return m_initialized; }
  void Listen(void);

 private:
  HALSimGazebo* m_halsim;
  int m_index;
  bool m_initialized;
  void Callback(const gazebo::msgs::ConstFloat64Ptr& msg);
  gazebo::transport::SubscriberPtr m_sub;
};
