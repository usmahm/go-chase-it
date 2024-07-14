#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

class PIClientnSubcriber {
  public:
    PIClientnSubcriber() {
      _sub1 = _n.subscribe("/camera/rgb/image_raw", 10, &PIClientnSubcriber::process_image_callback, this);

      _client = _n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

      // ROS_INFO_STREAM("SLEEPING");
      // ROS_INFO_STREAM("DONE SLEEPING");
      // drive_robot(0.0, 0.2);
    }

    void process_image_callback(const sensor_msgs::Image& img) {
      int pixel_value = 255;


      std::string log = "Height - " + std::to_string(img.height) + " - Step - " + std::to_string(img.step);
      ROS_INFO_STREAM(log);

      bool found_pixel_val = false;
      int pos = -1;
      for (int i=0; i < (img.height * img.step); i++) {
        if (img.data[i] == pixel_value) {
          found_pixel_val = true;
          pos = i;
          break;
        }
      }

      std::string log1 = "Found - " + std::to_string(found_pixel_val) + " - POS - " + std::to_string(pos);
      ROS_INFO_STREAM(log1);
    }

    void drive_robot(float lin_x, float ang_z) {
      ROS_INFO_STREAM("MOVING");
      ball_chaser::DriveToTarget srv;

      srv.request.linear_x = lin_x;
      srv.request.angular_z = ang_z;

      if (!_client.call(srv)) {
        ROS_ERROR("Failed to call command_robot service");
      }
    }

  private:
    ros::NodeHandle _n;
    ros::ServiceClient _client;
    ros::Subscriber _sub1;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "process_image");

  PIClientnSubcriber CASObject;
  ros::Duration(2).sleep();

  CASObject.drive_robot(0.0, 0.2);

  ROS_INFO_STREAM("RUNNING PROCESS_IMAGE_NODE");

  ros::spin();

  return 0;
}