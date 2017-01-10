#!/bin/bash

MASTER_IP=192.168.1.100
ssh $MASTER_IP
roscore
source /opt/ros/indigo/setup.bash
rosrun rviz rviz -d src/a_robot_platform/launch/slam.rviz