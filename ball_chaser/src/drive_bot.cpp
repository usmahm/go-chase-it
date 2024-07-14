#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

class DBServicenPublisher
{
  public:
    DBServicenPublisher() {
      _motor_cmd_publisher = _n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

      _command_robot_service = _n.advertiseService("/ball_chaser/command_robot", &DBServicenPublisher::handler_drive_request, this);
    }

    bool handler_drive_request(ball_chaser::DriveToTarget::Request &req, ball_chaser::DriveToTarget::Response &res) {
      geometry_msgs::Twist motor_cmd;
      
      std::string log = "Moving with velocities linear_x - " + std::to_string(req.linear_x) + " angular_z - " + std::to_string(req.angular_z);

      ROS_INFO_STREAM(log);

      motor_cmd.linear.x = req.linear_x;
      motor_cmd.angular.z = req.angular_z;
      
      _motor_cmd_publisher.publish(motor_cmd);

      res.msg_feedback = log;
      ROS_INFO_STREAM("Done " + res.msg_feedback);
      return true;
    }

  private:
    ros::NodeHandle _n;
    ros::Publisher _motor_cmd_publisher;
    ros::ServiceServer _command_robot_service;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "drive_bot");

  DBServicenPublisher SAPObject;

  ros::spin();
  
  return 0;
}