#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

enum MOVE_STATE {
  STOP,
  LEFT,
  RIGHT,
  FORWARD,
};

class PIClientnSubcriber {
  public:
    PIClientnSubcriber() {
      _sub1 = _n.subscribe("/camera/rgb/image_raw", 10, &PIClientnSubcriber::process_image_callback, this);

      _client = _n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    }

    void process_image_callback(const sensor_msgs::Image& img) {
      int pixel_value = 255;

      bool found_pixel_val = false;
      int column_found = -1;
      for (int i=0; i < (img.height * img.step); i++) {
        if (img.data[i] == pixel_value && img.data[i+1] == pixel_value && img.data[i+2] == pixel_value) { // Checks R, G, B val arranged consecutively
          found_pixel_val = true;
          column_found = i%img.step;
          break;
        }
      }

      if (column_found != -1) {
        MOVE_STATE move_state;
        
        if (column_found <= 700) { // Right
          move_state = RIGHT;
        } else if (column_found <= 1700) { //Center
          move_state = FORWARD;
        } else { // Left
          move_state = LEFT;
        }

        if (move_state != prev_move) {
          drive_robot(move_state);
        }
      } else if (moving) {
        drive_robot(STOP);
      }
    }

    void drive_robot(MOVE_STATE move_state) {
      float lin_x = 0.0;
      float ang_z = 0.0;

      std::string dir = "STOPPED";
      switch (move_state)
      {
        case FORWARD:
          lin_x = 0.5;
          dir = "FORWARD";
          break;
        case LEFT:
          ang_z = -0.7;
          dir = "LEFT";
          break;
        case RIGHT:
          ang_z = 0.7;
          dir = "RIGHT";
          break;
        default:
          break;
      }

      ROS_INFO_STREAM("Moving - " + dir);
      prev_move = move_state;
      

      ball_chaser::DriveToTarget srv;

      srv.request.linear_x = lin_x;
      srv.request.angular_z = ang_z;

      if (!_client.call(srv)) {
        ROS_ERROR("Failed to call command_robot service");
      }

      moving = lin_x != 0.0 || ang_z != 0.0;
    }

  private:
    ros::NodeHandle _n;
    ros::ServiceClient _client;
    ros::Subscriber _sub1;
    bool moving = false;
    MOVE_STATE prev_move = STOP;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "process_image");

  PIClientnSubcriber CASObject;

  ROS_INFO_STREAM("RUNNING PROCESS_IMAGE_NODE");

  ros::spin();

  return 0;
}