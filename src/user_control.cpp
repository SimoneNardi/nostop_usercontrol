/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2015, University of Pisa
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the ISR University of Coimbra nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Alessandro Settimi 2015
* Author: Mirko Ferrati 2015
* 
* 
* Modified by:  Niko Giovannini 
* 		Alessandro Faralli
*********************************************************************/

#include "user_control.h"

double MAX_LIN_VEL = 0.3;
double STEP_LIN_VEL = 0.1;
double MAX_ANG_VEL = 0.0;
double STEP_ANG_VEL = 10;

joy_handler::joy_handler(std::string namespace_)
{
    ROS_INFO("start Costruttore di usercontrol");
    joy_sub  = nodeh.subscribe<sensor_msgs::Joy>("/joy", 1, &joy_handler::joy_receive, this);
    std::string l_topic_name = namespace_  + "/cmd_vel";
    twist_pub = nodeh.advertise<geometry_msgs::Twist>(l_topic_name.c_str(), 1);
    
    twist.linear.x=0;
    twist.linear.y=0;
    twist.linear.z=0;
    twist.angular.x=0;
    twist.angular.y=0;
    twist.angular.z=0;
    ROS_INFO("end Costruttore di usercontrol");
}

void joy_handler::joy_receive(const sensor_msgs::Joy::ConstPtr& joy_msg)
try
{
  
  ROS_INFO("joipad ricevuto");
  
//     if(dual_mode)
//     {
// 	twist.linear.x = MAX_LIN_VEL*joy_msg->axes.at(1);
// 	twist.angular.z = MAX_ANG_VEL*joy_msg->axes.at(2);
//     }
//     else
    {
	twist.linear.x = MAX_LIN_VEL*joy_msg->axes.at(1);
	twist.angular.z = -MAX_ANG_VEL*joy_msg->axes.at(0);
    }
    
    if(joy_msg->buttons.at(0))
      dual_mode=!(dual_mode);
    
    if(joy_msg->buttons.at(4))
    {
    ROS_INFO("Bottone 5 -----> somma velocità lineare : %f", MAX_LIN_VEL);
    MAX_LIN_VEL+=STEP_LIN_VEL;
    }
    
    if(joy_msg->buttons.at(5))
    {
      ROS_INFO("Bottone 6 -----> somma velocità angolare : %f", MAX_ANG_VEL);
      MAX_ANG_VEL+=STEP_ANG_VEL;
    }
    if(joy_msg->buttons.at(6) && MAX_LIN_VEL > 0)
    {
      ROS_INFO("Bottone 7 -----> somma velocità lineare : %f", MAX_LIN_VEL);
      MAX_LIN_VEL-=STEP_LIN_VEL;
    }
    
    if(joy_msg->buttons.at(7) && MAX_ANG_VEL > 0)
    {
      ROS_INFO("Bottone 8 -----> somma velocità angolare : %f", MAX_ANG_VEL);
      MAX_ANG_VEL-=STEP_ANG_VEL;
    }
    twist_pub.publish(twist);
}
catch(...)
{
}

joy_handler::~joy_handler()
{

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "arduino_joy_node");
    ros::NodeHandle l_node("~");
    std::string robot_name;
    
    if (l_node.hasParam("robot_name"))
      l_node.getParam("robot_name",robot_name);
    else
      robot_name = "thief";
    
    joy_handler joy_h("/"+robot_name);

    ROS_INFO("Joy Handler Started");

    ros::spin();
}