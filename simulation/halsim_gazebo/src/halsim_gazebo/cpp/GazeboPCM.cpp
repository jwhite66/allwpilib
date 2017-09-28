/*----------------------------------------------------------------------------*/
/* Copyright (c) 2014-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "GazeboPCM.h"

#include <iostream>
#include <string>

#include "MockData/HAL_Value.h"
#include "MockData/NotifyListener.h"
#include "MockData/PCMData.h"
#include "simulation/gz_msgs/msgs.h"

static void pcm_callback(const char* name, void* param,
                         const struct HAL_Value* value) {
  GazeboPCM* pcm = static_cast<GazeboPCM*>(param);
  pcm->Publish(value->data.v_boolean);
}

GazeboPCM::GazeboPCM(int index, int channel, HALSimGazebo* halsim) {
  m_index = index;
  m_channel = channel;
  m_halsim = halsim;
  m_pub = NULL;
  HALSIM_RegisterPCMSolenoidOutputCallback(index, channel, pcm_callback, this,
                                           false);
}

void GazeboPCM::Publish(bool value) {
  if (!m_pub)
    m_pub = m_halsim->node.Advertise<gazebo::msgs::Bool>(
        "~/simulator/pneumatic/" + std::to_string(m_index + 1) + "/" +
        std::to_string(m_channel + 1));
  gazebo::msgs::Bool msg;
  msg.set_data(value);
  if (m_pub) m_pub->Publish(msg);
}

void GazeboPCM_SetPressureSwitch(int index, bool value) {
  HALSIM_SetPCMPressureSwitch(index, value);
}
