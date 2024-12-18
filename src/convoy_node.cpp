#include <iostream>
#include <math.h>
#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"

using namespace std;
using namespace geometry_msgs;
using namespace ros;
using namespace turtlesim;


float Kp_lv = 0.3;
float Kp_av = 1.35;
float distance_tolerance = 1.5;

class Convoy {
public:
    Convoy();

private:
    Pose Robot_Leader_Pose;
    Pose Robot_Follower_Pose;
    Subscriber Robot_Leader_Sub;
    Subscriber Robot_Follower_Sub;
    Publisher Robot_Follower_Command;
    NodeHandle Listener;
    NodeHandle CommanderNode;

    void Robot_Leader_PoseUpdate(const turtlesim::Pose::ConstPtr& msg);
    void Robot_Follower_PoseUpdate(const turtlesim::Pose::ConstPtr& msg);

    void trackLeader();
    float euclidean_distance();
    float linear_vel();
    float angle_vel();
    float steering_angle();
   
};

Convoy::Convoy(){

    string leader_pose, follower_pose, follower_speeds;
    ros::param::get("~leader_pose", leader_pose);
    ros::param::get("~follower_pose", follower_pose);
    ros::param::get("~follower_speeds", follower_speeds);

    Robot_Leader_Sub = Listener.subscribe(leader_pose, 10, &Convoy::Robot_Leader_PoseUpdate, this);
    Robot_Follower_Sub = Listener.subscribe(follower_pose, 10, &Convoy::Robot_Follower_PoseUpdate, this);
    Robot_Follower_Command = CommanderNode.advertise<geometry_msgs::Twist>(follower_speeds, 10);
}

void Convoy::Robot_Leader_PoseUpdate(const turtlesim::Pose::ConstPtr& msg)
{
    Robot_Leader_Pose.x = msg->x;
    Robot_Leader_Pose.y = msg->y;
    Robot_Leader_Pose.linear_velocity = msg->linear_velocity;
    Robot_Leader_Pose.angular_velocity = msg->angular_velocity;
    Robot_Leader_Pose.theta = msg->theta;
}

void Convoy::Robot_Follower_PoseUpdate(const turtlesim::Pose::ConstPtr& msg)
{
    //Actualizar la pose del robot follower con los valores contenidos en el mensaje msg recibido en el topic
    
    Robot_Follower_Pose.x = 
    Robot_Follower_Pose.y = 
    Robot_Follower_Pose.linear_velocity = 
    Robot_Follower_Pose.angular_velocity = 
    Robot_Follower_Pose.theta = 

    trackLeader();
}

void Convoy::trackLeader(){

    Twist msg;

    if (euclidean_distance() >= distance_tolerance){        
        msg.angular.z = angle_vel();

        if (abs(angle_vel()) > 1) msg.linear.x = 0;
        else msg.linear.x = linear_vel();
    }

    Robot_Follower_Command.publish(msg);    
}

float Convoy::euclidean_distance(){
    return sqrt(pow(Robot_Leader_Pose.x - Robot_Follower_Pose.x, 2) + pow(Robot_Leader_Pose.y - Robot_Follower_Pose.y, 2));
}

float Convoy::linear_vel(){
    return XXXX;
}

float Convoy::steering_angle(){
    return XXXX;
}

float Convoy::angle_vel(){
    return XXXX;
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "convoy");
    Convoy turtle;

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
       ros::spinOnce();
       loop_rate.sleep();
    }

    return 0;
}
