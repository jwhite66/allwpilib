/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

#include <DSCommJoystick.h>
#include <FRC_NetworkCommunication/FRCComm.h>
#include <MockData/DriverStationData.h>

class DSCommPacket {
 public:
  void Lock() { m_mutex.lock(); };
  void Unlock() { m_mutex.unlock(); };
  void SetIndex(uint8_t hi, uint8_t lo);
  void GetIndex(uint8_t& hi, uint8_t& lo);
  void SetControl(uint8_t control, uint8_t request);
  void GetControl(uint8_t& control);
  void GetStatus(uint8_t& status);
  void SetAlliance(uint8_t station_code);
  int  AddDSCommJoystick(uint8_t* data, int len);
  void GetControlWord(struct ControlWord_t& control_word);
  void GetAllianceStation(enum AllianceStationID_t& allianceStation);
  int  DecodeTCP(uint8_t* packet, int len);
  void DecodeUDP(uint8_t* packet, int len);
  void SendTCPToHALSim(void);
  void SendUDPToHALSim(void);
  void SendJoysticks(void);

  /* TCP (FMS) types */
  static const uint8_t cGameDataType = 0x0e;

  /* Control word bits */
  static const uint8_t cTest = 0x01;
  static const uint8_t cEnabled = 0x04;
  static const uint8_t cAutonomous = 0x02;
  static const uint8_t cFMS_Attached = 0x08;
  static const uint8_t cEmergencyStop = 0x80;

  /* Control request bitmask */
  static const uint8_t cRequestNormalMask = 0xF0;

  /* Status bits */
  static const uint8_t cRobotHasCode = 0x20;

  /* Joystick tag bits */
  static const uint8_t cTagDsCommJoystick = 0x0c;

  /* Joystick max count */
  /* TODO(jwhite@codeweavers.com) This is a magic number in the HAL; fix it there */
  static const uint8_t cMaxJoysticks = 6;

 private:
  std::string m_game_data;
  uint8_t m_hi;
  uint8_t m_lo;
  uint8_t m_control_sent;
  struct ControlWord_t m_control_word;
  enum AllianceStationID_t m_alliance_station;
  std::vector<DSCommJoystick> m_joysticks;
  std::mutex m_mutex;
  int m_udp_packets;
  std::chrono::high_resolution_clock::time_point m_packet_time;
  double m_match_time;
};
