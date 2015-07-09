#include <string>
#include <sstream>
#include <ros/ros.h>
#include "geometry_msgs/Twist.h"

ros::Subscriber g_sub;
ros::Publisher g_pub;

/////////////////////////////////////////
void forwardCallback(const geometry_msgs::TwistConstPtr& msg)
{
	g_pub.publish(msg);
	ROS_INFO("called");
}

/////////////////////////////////////////
int main ( int argc, char** argv )
{
	//TODO ros param robot name
	std::string robot_name="0";
    
	std::stringstream l_agentname;
	l_agentname << "AgentMotorControl_";
	l_agentname << robot_name;
	
	std::string l_nodename("Forward_");
	l_nodename += l_agentname.str();
	
	ros::init(argc,argv, l_nodename );
  
	ros::NodeHandle l_node;
	g_sub = l_node.subscribe("/base_controller/command", 1, forwardCallback);
	g_pub = l_node.advertise<geometry_msgs::Twist>(l_agentname.str().c_str(), 1);
	
	ros::spin();
	
	return 0;
}
