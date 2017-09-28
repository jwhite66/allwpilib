/*----------------------------------------------------------------------------*/
/* Copyright (c) 2014-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "HALSimGazebo.h"

class GazeboPCM {
 public:
  GazeboPCM(int index, int channel, HALSimGazebo* halsim);
  void Publish(bool value);

 private:
  HALSimGazebo* m_halsim;

  int m_index;
  int m_channel;

  gazebo::transport::PublisherPtr m_pub;
};

void GazeboPCM_SetPressureSwitch(int index, bool value);
