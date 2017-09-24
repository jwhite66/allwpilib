/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>

#include <GazeboNode.h>

/* Currently, robots never terminate, so we keep a single static object
   to access Gazebo with and it is never properly released or cleaned up. */
static GazeboNode node;

extern "C" {
int init(void) {
  std::cout << "Gazebo Simulator Initializing." << std::endl;

  if (!node.Connect()) {
    std::cerr << "Error: unable to connect to Gazebo.  Is it running?."
              << std::endl;
    return -1;
  }
  std::cout << "Gazebo Simulator Connected." << std::endl;
  return 0;
}
}
