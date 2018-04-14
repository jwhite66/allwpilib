/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DSCommPacket.h"

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

#include <FRC_NetworkCommunication/FRCComm.h>
#include <MockData/DriverStationData.h>

/*----------------------------------------------------------------------------
**  The following classes help parse and hold information about the
**   driver station and it's joysticks.
**--------------------------------------------------------------------------*/
void DSCommPacket::SetIndex(uint8_t hi, uint8_t lo) {
  m_hi = hi;
  m_lo = lo;
}

void DSCommPacket::GetIndex(uint8_t& hi, uint8_t& lo) {
  hi = m_hi;
  lo = m_lo;
}

void DSCommPacket::SetControl(uint8_t control, uint8_t request) {
  std::memset(&m_control_word, 0, sizeof(m_control_word));
  m_control_word.enabled = (control & cEnabled) != 0;
  m_control_word.autonomous = (control & cAutonomous) != 0;
  m_control_word.test = (control & cTest) != 0;
  m_control_word.eStop = (control & cEmergencyStop) != 0;
  m_control_word.fmsAttached = (control & cFMS_Attached) != 0;
  m_control_word.dsAttached = (request & cRequestNormalMask) != 0;

  m_control_sent = control;
}

void DSCommPacket::GetControl(uint8_t& control) {
  control = m_control_sent;
}

void DSCommPacket::GetStatus(uint8_t& status) {
  status = cRobotHasCode;
}

void DSCommPacket::SetAlliance(uint8_t station_code) {
  m_alliance_station = static_cast<enum AllianceStationID_t>(station_code);
}

int DSCommPacket::AddDSCommJoystick(uint8_t* data, int len) {
  DSCommJoystick stick;
  if (len > 0) {
    int axis_count = *data++;
    len--;
    if (axis_count > len) return -1;
    len -= axis_count;
    for (; axis_count > 0; axis_count--) {
      stick.axes.push_back(*data++);
    }
  }

  if (len > 2) {
    stick.button_count = *data++;
    stick.buttons = (*data++) << 8;
    stick.buttons |= *data++;
    len -= 3;
  }

  if (len > 0) {
    int pov_count = *data++;
    len--;
    if (pov_count * 2 > len) return -1;
    len -= pov_count * 2;
    for (; pov_count > 0; pov_count--) {
      stick.povs.push_back((data[0] << 8) | data[1]);
      data += 2;
    }
  }

  m_joysticks.push_back(stick);

  return len;
}

void DSCommPacket::GetControlWord(struct ControlWord_t& control_word) {
  control_word = m_control_word;
}

void DSCommPacket::GetAllianceStation(enum AllianceStationID_t& alliance_station) {
  alliance_station = m_alliance_station;
}

/*----------------------------------------------------------------------------
**  Communications functions
**--------------------------------------------------------------------------*/
int DSCommPacket::DecodeTCP(uint8_t* packet, int len) {
  if (len < 2) return 0;
  if (packet[0] == 0 && packet[1] == 0) return 2;
  int packet_len = packet[1];
  if (packet_len + 2 > len) return 0;
  int packet_type = static_cast<int>(packet[2]);

  Lock();
  if (packet_type == cGameDataType) {
      m_game_data = std::string(reinterpret_cast<char *>(packet + 3), packet_len - 1);
  }
  // TODO(jwhite@codeweavers.com) - packet type 02 is joy stick descriptor
  else {
    std::cerr << "TCP packet type " << packet_type << " unimplemented" << std::endl;
    for (int i = 0; i < packet_len + 2; i++)
        fprintf(stderr, "%02x ", packet[i]);
    fprintf(stderr, "\n");
  }
  Unlock();
  return packet_len + 2;
}

void DSCommPacket::DecodeUDP(uint8_t* packet, int len) {
  if (len < 3) return;

  Lock();
  m_joysticks.clear();
  SetIndex(packet[0], packet[1]);
  if (packet[2] != 0) {
    if (len >= 6) {
      SetControl(packet[3], packet[4]);
      SetAlliance(packet[5]);
      packet += 6;
      len -= 6;
      while (len > 0) {
        int packet_len = *packet++;
        if (packet_len > len) break;
        if (*packet == cTagDsCommJoystick) {
          if (AddDSCommJoystick(packet + 1, packet_len - 1) < 0) break;
        }
        packet += packet_len;
        len -= packet_len;
      }
    }
  }
  m_udp_packets++;
  Unlock();
}

void DSCommPacket::SendJoysticks(void) {
  unsigned int i;

  for (i = 0; i < cMaxJoysticks; i++) {
    struct HAL_JoystickAxes axes;
    struct HAL_JoystickPOVs povs;
    struct HAL_JoystickButtons buttons;
    struct HAL_JoystickDescriptor descriptor;
    int j;

    std::memset(&axes, 0, sizeof(axes));
    std::memset(&povs, 0, sizeof(povs));
    std::memset(&buttons, 0, sizeof(buttons));
    std::memset(&descriptor, 0, sizeof(descriptor));

    if (i < m_joysticks.size()) {
      axes.count = std::min(static_cast<int>(m_joysticks[i].axes.size()),
          HAL_kMaxJoystickAxes);
      for (j = 0; j < axes.count; j++) {
        int8_t value = m_joysticks[i].axes[j];
        if (value < 0) {
          axes.axes[j] = value / 128.0;
        } else {
          axes.axes[j] = value / 127.0;
        }
      }

      povs.count = std::min(static_cast<int>(m_joysticks[i].povs.size()),
          HAL_kMaxJoystickPOVs);
      for (j = 0; j < povs.count; j++)
        povs.povs[j] = m_joysticks[i].povs[j];

      buttons.count = m_joysticks[i].button_count;
      buttons.buttons = m_joysticks[i].buttons;

      descriptor.axisCount = axes.count;
      descriptor.povCount = povs.count;
      descriptor.buttonCount = buttons.count;
    }
    descriptor.type = -1;
    /* TODO(jwhite@codeweavers.com):  See about supporting Joystick descriptors */

    HALSIM_SetJoystickAxes(i, &axes);
    HALSIM_SetJoystickPOVs(i, &povs);
    HALSIM_SetJoystickButtons(i, &buttons);
    HALSIM_SetJoystickDescriptor(i, &descriptor);

    /* TODO(jwhite@codeweavers.com):  If we want to support rumble, etc,
            implement SetJoyStickOutputs, although that would be a callback  */
  }
}

void DSCommPacket::SendTCPToHALSim(void) {
  struct HAL_MatchInfo info;
  Lock();
  info.eventName = "Simulation";
  info.matchType = HAL_MatchType::HAL_kMatchType_none;
  info.matchNumber = 1;
  info.replayNumber = 0;
  info.gameSpecificMessage = reinterpret_cast<char *>(std::calloc(m_game_data.size() + 1, 1));
  m_game_data.copy(info.gameSpecificMessage, m_game_data.size(), 0);
  HALSIM_SetMatchInfo(&info);
  free(info.gameSpecificMessage);
  Unlock();
}


void DSCommPacket::SendUDPToHALSim(void) {
  struct ControlWord_t control_word;
  AllianceStationID_t alliance_station;

  Lock();
  GetControlWord(control_word);
  GetAllianceStation(alliance_station);
  auto now = std::chrono::high_resolution_clock::now();
  if (m_udp_packets == 1) {
      m_match_time = 0.0;
  }
  else if (control_word.enabled) {
      std::chrono::duration<double> delta = (now - m_packet_time);
      m_match_time += delta.count();
  }
  m_packet_time = now;
  SendJoysticks();
  Unlock();

  HALSIM_SetDriverStationMatchTime(m_match_time);
  HALSIM_SetDriverStationEnabled(control_word.enabled);
  HALSIM_SetDriverStationAutonomous(control_word.autonomous);
  HALSIM_SetDriverStationTest(control_word.test);
  HALSIM_SetDriverStationEStop(control_word.eStop);
  HALSIM_SetDriverStationFmsAttached(control_word.fmsAttached);
  HALSIM_SetDriverStationDsAttached(control_word.dsAttached);
  HALSIM_SetDriverStationAllianceStationId(
      static_cast<HAL_AllianceStationID>(alliance_station));

  HALSIM_NotifyDriverStationNewData();

}
