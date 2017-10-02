/*----------------------------------------------------------------------------*/
/* Copyright (c) 2014-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "HALSimGazebo.h"
#include "simulation/gz_msgs/msgs.h"

class GazeboEncoder {
 public:
  GazeboEncoder(int index, HALSimGazebo* halsim);
  void Control(const char* command);
  void Listen(void);
  bool m_reverse;

 private:
  HALSimGazebo* m_halsim;
  int m_index;

  void Callback(const gazebo::msgs::ConstFloat64Ptr& msg);

  gazebo::transport::PublisherPtr m_pub;
  gazebo::transport::SubscriberPtr m_sub;
};
