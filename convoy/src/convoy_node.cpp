#include <iostream>
#include <math.h>
#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"

using namespace std;
using namespace geometry_msgs;
using namespace ros;
using namespace turtlesim;

Pose LeaderPose;
Pose FollowerPose;

float constant = 0.3;
float distance_tolerance = 1.5; //0.9;

class Convoy {
public:
    Convoy();
    void trackLeader();
    float euclidean_distance();
    float linear_vel();
    float angle_vel();
    float steering_angle();
    
    NodeHandle ListenerLeaderNode;
    NodeHandle ListenerFollowerNode;
    Subscriber LeaderSub;
    Subscriber FollowerSub;
    NodeHandle CommanderNode;
    Publisher Command;
    Twist msg;
private:
};

Convoy::Convoy(){
}

void Convoy::trackLeader(){
    if(euclidean_distance() >= distance_tolerance){
        msg.linear.x = linear_vel();
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = angle_vel();
        Command.publish(msg);
    }
}

float Convoy::euclidean_distance(){
    return sqrt(pow(LeaderPose.x - FollowerPose.x, 2) + pow(LeaderPose.y - FollowerPose.y, 2));
}

float Convoy::linear_vel(){
    return constant * euclidean_distance();
}

float Convoy::angle_vel(){
    return constant * (steering_angle() - FollowerPose.theta);//*1.5;
}

float Convoy::steering_angle(){
    return atan2((LeaderPose.y - FollowerPose.y),(LeaderPose.x - FollowerPose.x));
}

void LeaderPoseUpdate(const turtlesim::Pose::ConstPtr& msg)
{
    LeaderPose.x = msg->x;
    LeaderPose.y = msg->y;
    LeaderPose.linear_velocity = msg->linear_velocity;
    LeaderPose.angular_velocity = msg->angular_velocity;
    LeaderPose.theta = msg->theta;
}

void FollowerPoseUpdate(const turtlesim::Pose::ConstPtr& msg)
{
    FollowerPose.x = msg->x;
    FollowerPose.y = msg->y;
    FollowerPose.linear_velocity = msg->linear_velocity;
    FollowerPose.angular_velocity = msg->angular_velocity;
    FollowerPose.theta = msg->theta;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "follower");
    Convoy turtle;
    
    turtle.LeaderSub = turtle.ListenerLeaderNode.subscribe("/leader/pose", 10, LeaderPoseUpdate);
    turtle.FollowerSub = turtle.ListenerFollowerNode.subscribe("/follower/pose", 10, FollowerPoseUpdate);
    turtle.Command = turtle.CommanderNode.advertise<geometry_msgs::Twist>("/follower/cmd_vel", 10);
    
    ros::Rate loop_rate(1);
    while (ros::ok())
    {
        turtle.trackLeader();
        ros::spinOnce();
        loop_rate.sleep();
    }
  return 0;
}
