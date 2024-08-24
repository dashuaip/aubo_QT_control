#ifndef INIT_H
#define INIT_H

#include <iostream>
#include "AuboRobotMetaType.h"
#include "serviceinterface.h"

extern std::string SERVER_HOST;
extern int SERVER_PORT;
extern double J_ACC;
extern double J_VEL;
extern double E_ACC;
extern double E_VEL;

void set_AccVel(ServiceInterface &robotService);
bool robot_init(ServiceInterface &robotService);

#endif // INIT_H
