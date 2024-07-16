A Go Chase It project done in the [Udacity Robotics](https://www.udacity.com/enrollment/nd209) Course I took.

The goal of the project was to practice new ROS concepts learned. The robot is to be actuated to move in the direction of a white ball once it detects it using the camera sensor attached to it.
The process_image node analysis the image data gotten from the camera sensor via the `/camera/rgb/image_raw` topic it subscribes to. If it detects a white ball in view, it calls the `/ball_chaser/command_robot` service exposed 
by the `drive_bot` node which in turn publishes to the `/cmd_vel` topic exposed by the wheels to move the robot around. 

To run the project, 
- Follow [this instruction](https://wiki.ros.org/catkin/Tutorials/create_a_workspace) to setup a catkin workspace on your machine
- Copy the folders in this repo to the src folder in the catkin workspace and run catkin_make in the root folder of your workspace.
- Run `source devel\setup.bash && roslaunch my_robot world.launch` to launch the robot and it's environment
- In another terminal tab run `source devel\setup.bash && roslaunch ball_chaser ball_chaser.launch` to launch the `process_image` and `drive_bot` nodes
- Interact with the environment in gazebo by moving the white ball around the robot

- Below is a demo of the project running

https://github.com/user-attachments/assets/416b20c2-5ced-4513-990c-7acd8632897b

