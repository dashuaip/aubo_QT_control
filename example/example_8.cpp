#include "example_8.h"


#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "util.h"

#define SERVER_HOST "192.168.40.128"
#define SERVER_PORT 8899



void Example_8::demo()
{
    FK();
    IK();
}

void Example_8::FK()
{
    ServiceInterface robotService;

    aubo_robot_namespace::wayPoint_S wayPoint;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    double jointAngle[aubo_robot_namespace::ARM_DOF] = {0};

    Util::initJointAngleArray(jointAngle, Util::RAD, 0.0/180.0*M_PI,  0.0/180.0*M_PI,  90.0/180.0*M_PI, 0.0/180.0*M_PI, 90.0/180.0*M_PI,   0.0/180.0*M_PI);

    ret = robotService.robotServiceRobotFk(jointAngle, aubo_robot_namespace::ARM_DOF, wayPoint);

    if(ret == aubo_robot_namespace::InterfaceCallSuccCode)
    {
        Util::printWaypoint(wayPoint);
    }
    else
    {
        std::cerr<<"Calling the positive solution failed"<<std::endl;
    }

}

void Example_8::IK()
{
    ServiceInterface robotService;

    aubo_robot_namespace::wayPoint_S wayPoint;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    double startPointJointAngle[aubo_robot_namespace::ARM_DOF] = {0};

    Util::initJointAngleArray(startPointJointAngle, Util::RAD, 0.0/180.0*M_PI,  0.0/180.0*M_PI,  0.0/180.0*M_PI, 0.0/180.0*M_PI, 0.0/180.0*M_PI,0.0/180.0*M_PI);

    aubo_robot_namespace::Pos targetPosition;
    targetPosition.x =-0.400;
    targetPosition.y =-0.1215;
    targetPosition.z = 0.5476;

    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori targetOri;

    rpy.rx = 180.0/180.0*M_PI;
    rpy.ry = 0.0/180.0*M_PI;
    rpy.rz = -90.0/180.0*M_PI;

    robotService.RPYToQuaternion(rpy, targetOri);

    ret = robotService.robotServiceRobotIk(startPointJointAngle, targetPosition, targetOri, wayPoint);

    if(ret == aubo_robot_namespace::InterfaceCallSuccCode)
    {
        Util::printWaypoint(wayPoint);
    }
    else
    {
        std::cerr<<"Calling the inverse solution failed"<<std::endl;
    }
}
