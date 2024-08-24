#include "init.h"

#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

std::string SERVER_HOST = "127.0.0.1";
int SERVER_PORT = 8899;
double J_ACC = 0;
double J_VEL = 0;
double E_ACC = 0;
double E_VEL = 0;

void set_AccVel(ServiceInterface &robotService)
{
    /** 设置运动的速度、加速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = J_ACC/180.0*M_PI;
    jointMaxAcc.jointPara[1] = J_ACC/180.0*M_PI;
    jointMaxAcc.jointPara[2] = J_ACC/180.0*M_PI;
    jointMaxAcc.jointPara[3] = J_ACC/180.0*M_PI;
    jointMaxAcc.jointPara[4] = J_ACC/180.0*M_PI;
    jointMaxAcc.jointPara[5] = J_ACC/180.0*M_PI;   //The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = J_VEL/180.0*M_PI;
    jointMaxVelc.jointPara[1] = J_VEL/180.0*M_PI;
    jointMaxVelc.jointPara[2] = J_VEL/180.0*M_PI;
    jointMaxVelc.jointPara[3] = J_VEL/180.0*M_PI;
    jointMaxVelc.jointPara[4] = J_VEL/180.0*M_PI;
    jointMaxVelc.jointPara[5] = J_VEL/180.0*M_PI;   //The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(E_ACC);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(E_ACC);

    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(E_VEL);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(E_VEL);
}

bool robot_init(ServiceInterface &robotService)
{

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST.c_str(), SERVER_PORT, "aubo", "123456");
    if(ret == aubo_robot_namespace::InterfaceCallSuccCode)
    {
        std::cout<<"login successful."<<std::endl;
    }
    else
    {
        std::cerr<<"login failed."<<std::endl;
        return false;
    }


    /** If the real robot arm is connected, the arm needs to be initialized.　**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    //Tool dynamics parameter
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));

    ret = robotService.rootServiceRobotStartup(toolDynamicsParam/**Tool dynamics parameter**/,
                                               6        /*Collision level*/,
                                               true     /*Whether to allow reading poses defaults to true*/,
                                               true,    /*Leave the default to true */
                                               1000,    /*Leave the default to 1000 */
                                               result); /*Robot arm initialization*/
    if(ret == aubo_robot_namespace::InterfaceCallSuccCode)
    {
        std::cout<<"Robot arm initialization succeeded."<<std::endl;
    }
    else
    {
        std::cerr<<"Robot arm initialization failed."<<std::endl;
        return false;
    }

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    set_AccVel(robotService);

    return true;
}
