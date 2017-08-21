//Auto Generated
#ifndef _FRC_MSGS_H_
#define _FRC_MSGS_H_

#ifdef _WIN32
  //include this before anything else includes windows.h
  //putting this here saves putting it in more files
  #include <Winsock2.h>
#endif


 #include "simulation/gz_msgs/bool.pb.h"
 #include "simulation/gz_msgs/driver-station.pb.h"
 #include "simulation/gz_msgs/float64.pb.h"
 #include "simulation/gz_msgs/frc_joystick.pb.h"


#include <gazebo/msgs/msgs.hh>
#include <boost/shared_ptr.hpp>

namespace gazebo {
  namespace msgs {

    typedef GzString String;

    typedef boost::shared_ptr< gazebo::msgs::String > StringPtr;
    typedef const boost::shared_ptr< const gazebo::msgs::String > ConstStringPtr;

    typedef boost::shared_ptr< msgs::Float64 > Float64Ptr;
    typedef const boost::shared_ptr< const msgs::Float64 > ConstFloat64Ptr;

    typedef boost::shared_ptr< msgs::Bool > BoolPtr;
    typedef const boost::shared_ptr< const msgs::Bool > ConstBoolPtr;

    typedef boost::shared_ptr< msgs::FRCJoystick > FRCJoystickPtr;
    typedef const boost::shared_ptr< const msgs::FRCJoystick > ConstFRCJoystickPtr;

    typedef boost::shared_ptr< msgs::DriverStation > DriverStationPtr;
    typedef const boost::shared_ptr< const msgs::DriverStation > ConstDriverStationPtr;
  }
}

#endif /* _FRC_MSGS_H_ */
