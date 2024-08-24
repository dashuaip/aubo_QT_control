#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    this->ui->setupUi(this);
    this->setFixedSize(1111,621);
    this->setWindowTitle("AUBO机器人控制软件 作者：J-hope");

    this->ui->reset_accvel->setEnabled(false);
    this->ui->origin->setEnabled(false);
    this->ui->stop->setEnabled(false);
    this->ui->shutdown->setEnabled(false);
    this->ui->random_Joint->setEnabled(false);
    this->ui->random_End->setEnabled(false);
    this->ui->J_jointMove->setEnabled(false);
    this->ui->E_jointMove->setEnabled(false);
    this->ui->J_lineMove->setEnabled(false);
    this->ui->E_lineMove->setEnabled(false);

    this->ui->JOINT1->setEnabled(false);
    this->ui->JOINT2->setEnabled(false);
    this->ui->JOINT3->setEnabled(false);
    this->ui->JOINT4->setEnabled(false);
    this->ui->JOINT5->setEnabled(false);
    this->ui->JOINT6->setEnabled(false);
    this->ui->X->setEnabled(false);
    this->ui->Y->setEnabled(false);
    this->ui->Z->setEnabled(false);
    this->ui->RX->setEnabled(false);
    this->ui->RY->setEnabled(false);
    this->ui->RZ->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

QString addTime(const char *msg)
{
    // 获取当前时间并格式化为字符串
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    // 构建包含时间戳的消息字符串
    QString message = QString("----------%1----------\n%2").arg(timeString).arg(msg);
    return message;
}

void Widget::on_connect_clicked()
{
    // 读取初始化数据
    SERVER_HOST = this->ui->IP->text().toStdString();
    SERVER_PORT = this->ui->PORT->text().toInt();
    J_ACC = this->ui->J_ACC->text().toDouble();
    J_VEL = this->ui->J_VEL->text().toDouble();
    E_ACC = this->ui->E_ACC->text().toDouble();
    E_VEL = this->ui->E_VEL->text().toDouble();
    if(robot_init(this->robotService)){
        // 禁用连接按钮，使其无法再被点击
        this->ui->connect->setEnabled(false);
        this->ui->IP->setEnabled(false);
        this->ui->PORT->setEnabled(false);

        // 开启其他按钮
        this->ui->reset_accvel->setEnabled(true);
        this->ui->origin->setEnabled(true);

        this->ui->stop->setEnabled(true);
        this->ui->shutdown->setEnabled(true);

        this->ui->random_Joint->setEnabled(true);
        this->ui->random_End->setEnabled(true);
        this->ui->J_jointMove->setEnabled(true);
        this->ui->E_jointMove->setEnabled(true);
        this->ui->J_lineMove->setEnabled(true);
        this->ui->E_lineMove->setEnabled(true);

        this->ui->JOINT1->setEnabled(true);
        this->ui->JOINT2->setEnabled(true);
        this->ui->JOINT3->setEnabled(true);
        this->ui->JOINT4->setEnabled(true);
        this->ui->JOINT5->setEnabled(true);
        this->ui->JOINT6->setEnabled(true);

        this->ui->X->setEnabled(true);
        this->ui->Y->setEnabled(true);
        this->ui->Z->setEnabled(true);
        this->ui->RX->setEnabled(true);
        this->ui->RY->setEnabled(true);
        this->ui->RZ->setEnabled(true);

        // 向QListWidget添加一些信息
        this->ui->show_status->addItem(addTime("连接机器人成功"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }
    else {
        this->ui->show_status->addItem(addTime("连接机器人失败！！！"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }
}

void Widget::on_reset_accvel_clicked()
{
    J_ACC = this->ui->J_ACC->text().toDouble();
    J_VEL = this->ui->J_VEL->text().toDouble();
    E_ACC = this->ui->E_ACC->text().toDouble();
    E_VEL = this->ui->E_VEL->text().toDouble();
    set_AccVel(this->robotService);

    this->ui->show_status->addItem(addTime("已重新设置机器人速度"));
    this->ui->show_status->addItem("\n");
    this->ui->show_status->scrollToBottom();
}

void Widget::on_origin_clicked()
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    double jointAngle[aubo_robot_namespace::ARM_DOF] = {0};
    Util::initJointAngleArray(jointAngle, Util::DEG, 0.0,  0.0,  0.0, 0.0, 0.0, 0.0);

    QStringList JOINT; // 与容器类似
    for (int i=0;i<aubo_robot_namespace::ARM_DOF;i++) {
        JOINT.push_back(QString::number(0.0));
    }
    this->ui->JOINT1->setText(JOINT[0]);
    this->ui->JOINT2->setText(JOINT[1]);
    this->ui->JOINT3->setText(JOINT[2]);
    this->ui->JOINT4->setText(JOINT[3]);
    this->ui->JOINT5->setText(JOINT[4]);
    this->ui->JOINT6->setText(JOINT[5]);
    this->ui->X->setText(QString::number(0));
    this->ui->Y->setText(QString::number(-0.2155));
    this->ui->Z->setText(QString::number(0.985));
    this->ui->RX->setText(QString::number(90));
    this->ui->RY->setText(QString::number(0));
    this->ui->RZ->setText(QString::number(0));

    robotService.robotServiceJointMove(jointAngle, false);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {

        this->ui->show_status->addItem(addTime("机器人回到原点失败！！！"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }
    else {
        this->ui->show_status->addItem(addTime("机器人开始回到原点"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }
}

void Widget::on_stop_clicked()
{
    stop_signal = true;
    robotService.robotMoveFastStop(); // 快速停止
    this->ui->show_status->addItem(addTime("机器人停止运动"));
    this->ui->show_status->addItem("\n");
    this->ui->show_status->scrollToBottom();
}

void Widget::on_shutdown_clicked()
{
    robotService.rootServiceRobotMoveControl(aubo_robot_namespace::RobotMoveStop); // 运动停止，停得慢一点、缓一点

    /** Robotic arm shutdown**/
    robotService.robotServiceRobotShutdown();

    /** Interface call: logout　**/
    robotService.robotServiceLogout();

    close();
}

/****************************************************************************************************************/

QString Qrandom_data(double min, double max){
    // 生成一个在min到max之间的随机数
    // 创建随机数生成器
    std::random_device rd;  // 用于获取随机数种子
    std::mt19937 gen(rd()); // 使用种子初始化Mersenne Twister生成器

    // 定义浮点数分布范围
    std::uniform_real_distribution<double> dis(min, max);

    // 生成随机浮点数
    return QString::number(dis(gen));
}

aubo_robot_namespace::wayPoint_S aubo_FK(ServiceInterface &robotService, const double *wp)
{
    aubo_robot_namespace::wayPoint_S waypointFK;
    robotService.robotServiceRobotFk(wp,aubo_robot_namespace::ARM_DOF,waypointFK);
    return waypointFK;
}

aubo_robot_namespace::wayPoint_S aubo_IK(ServiceInterface &robotService, aubo_robot_namespace::wayPoint_S &wp)
{
    //参考点关节角
    double startpoint[6] = {0};
    aubo_robot_namespace::wayPoint_S waypointIK;
    robotService.robotServiceRobotIk(startpoint,wp.cartPos.position,wp.orientation,waypointIK);
    return waypointIK;
}

void Widget::on_random_Joint_clicked()
{
    double jointAngle[aubo_robot_namespace::ARM_DOF] = {Qrandom_data(-170.0,170.0).toDouble()*M_PI/180,
                                                        Qrandom_data(-90.0,90.0).toDouble()*M_PI/180,
                                                        Qrandom_data(-170.0,170.0).toDouble()*M_PI/180,
                                                        Qrandom_data(-170.0,170.0).toDouble()*M_PI/180,
                                                        Qrandom_data(-170.0,170.0).toDouble()*M_PI/180,
                                                        Qrandom_data(-170.0,170.0).toDouble()*M_PI/180
                                                       };
    // 可达性检查
    aubo_robot_namespace::wayPoint_S wp;
    wp = aubo_FK(robotService,jointAngle);
    if(wp.cartPos.position.z > 0.3)
    {
        this->ui->JOINT1->setText(QString::number(jointAngle[0]/M_PI*180, 'f', 4));
        this->ui->JOINT2->setText(QString::number(jointAngle[1]/M_PI*180, 'f', 4));
        this->ui->JOINT3->setText(QString::number(jointAngle[2]/M_PI*180, 'f', 4));
        this->ui->JOINT4->setText(QString::number(jointAngle[3]/M_PI*180, 'f', 4));
        this->ui->JOINT5->setText(QString::number(jointAngle[4]/M_PI*180, 'f', 4));
        this->ui->JOINT6->setText(QString::number(jointAngle[5]/M_PI*180, 'f', 4));
    }
    else {
        Widget::on_random_Joint_clicked();
    }
}


void Widget::on_random_End_clicked()
{
    aubo_robot_namespace::wayPoint_S wp;
    wp.cartPos.position.x = Qrandom_data(-0.8773875,0.8773875).toDouble();
    wp.cartPos.position.y = Qrandom_data(-0.8773875,0.8773875).toDouble();
    wp.cartPos.position.z = Qrandom_data(0.0,0.8773875).toDouble();
    aubo_robot_namespace::Rpy rpy_tmp;
    rpy_tmp.rx = Qrandom_data(-170.0,170).toDouble()*M_PI/180;
    rpy_tmp.ry = Qrandom_data(-170.0,170).toDouble()*M_PI/180;
    rpy_tmp.rz = Qrandom_data(-170.0,170).toDouble()*M_PI/180;
    robotService.RPYToQuaternion(rpy_tmp,wp.orientation);
    wp = aubo_IK(robotService, wp);

    if(wp.jointpos[0] > 0.0000001 && wp.jointpos[1] > 0.0000001 && fabs(wp.jointpos[1]) < 1.57 &&
       wp.jointpos[2] > 0.0000001 && wp.jointpos[3] > 0.0000001 && wp.jointpos[4] > 0.0000001 && wp.jointpos[5] > 0.0000001 &&
       wp.cartPos.position.z > 0.3 && wp.cartPos.position.x < 0.8773875 && wp.cartPos.position.y < 0.8773875 && wp.cartPos.position.z < 0.8773875){
        this->ui->X->setText(QString::number(wp.cartPos.position.x, 'f', 4));
        this->ui->Y->setText(QString::number(wp.cartPos.position.y, 'f', 4));
        this->ui->Z->setText(QString::number(wp.cartPos.position.z, 'f', 4));
        aubo_robot_namespace::Rpy tempRpy;
        robotService.quaternionToRPY(wp.orientation,tempRpy);
        this->ui->RX->setText(QString::number(tempRpy.rx/M_PI*180, 'f', 4));
        this->ui->RY->setText(QString::number(tempRpy.ry/M_PI*180, 'f', 4));
        this->ui->RZ->setText(QString::number(tempRpy.rz/M_PI*180, 'f', 4));
    }
    else {
        Widget::on_random_End_clicked();
    }
}

void Widget::on_J_jointMove_clicked()
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    double jointAngle[aubo_robot_namespace::ARM_DOF] = {0};
    Util::initJointAngleArray(jointAngle, Util::DEG,
                              this->ui->JOINT1->text().toDouble(),
                              this->ui->JOINT2->text().toDouble(),
                              this->ui->JOINT3->text().toDouble(),
                              this->ui->JOINT4->text().toDouble(),
                              this->ui->JOINT5->text().toDouble(),
                              this->ui->JOINT6->text().toDouble());
    robotService.robotServiceJointMove(jointAngle, false);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {

        this->ui->show_status->addItem(addTime("关节运动失败！！！(关节空间)"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }
    usleep(0.1*1000000);  // 1000000微秒等于1秒，延时100ms防止机器人未启动
/******************************************************************************************/
    /*获取机械臂当前运行状态*/
    robotService.robotServiceGetRobotCurrentState(this->robotstate);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (robotstate == 0)
        {
            /*获取机械臂当前关节角信息*/
            aubo_robot_namespace::JointParam jointangle;
            robotService.robotServiceGetJointAngleInfo(jointangle);
            for(int i=0;i<aubo_robot_namespace::ARM_DOF;i++)
            {
                if(fabs(jointangle.jointPos[i]-jointAngle[i])*180.0/M_PI > 0.01)
                {
                    timer->stop(); // 停止定时器
                    timer->destroyed();

                    if(!stop_signal)
                    {
                        this->ui->show_status->addItem(addTime("关节运动失败！！！(关节空间)"));
                        this->ui->show_status->addItem("\n");
                        this->ui->show_status->scrollToBottom();
                        stop_signal = false;
                    }
                    return;
                }
            }
            timer->stop(); // 停止定时器
            timer->destroyed();
            this->ui->show_status->addItem(addTime("机器人关节运动成功(关节空间)"));
            this->ui->show_status->addItem("\n");
            this->ui->show_status->scrollToBottom();
            return;
        }
        else
        {
            robotService.robotServiceGetRobotCurrentState(this->robotstate);
        }
    });
    timer->start(10); // 定时器每10毫秒触发一次
/******************************************************************************************/
}

void Widget::on_J_lineMove_clicked()
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    double jointAngle[aubo_robot_namespace::ARM_DOF] = {0};
    Util::initJointAngleArray(jointAngle, Util::DEG,
                              this->ui->JOINT1->text().toDouble(),
                              this->ui->JOINT2->text().toDouble(),
                              this->ui->JOINT3->text().toDouble(),
                              this->ui->JOINT4->text().toDouble(),
                              this->ui->JOINT5->text().toDouble(),
                              this->ui->JOINT6->text().toDouble());

    robotService.robotServiceLineMove(jointAngle, false);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        this->ui->show_status->addItem(addTime("直线运动失败！！！(关节空间)"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }

    usleep(0.1*1000000);  // 1000000微秒等于1秒，延时100ms防止机器人未启动
/******************************************************************************************/
    /*获取机械臂当前运行状态*/
    robotService.robotServiceGetRobotCurrentState(this->robotstate);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (robotstate == 0)
        {
            /*获取机械臂当前关节角信息*/
            aubo_robot_namespace::JointParam jointangle;
            robotService.robotServiceGetJointAngleInfo(jointangle);
            for(int i=0;i<aubo_robot_namespace::ARM_DOF;i++)
            {
                if(fabs(jointangle.jointPos[i]-jointAngle[i])*180.0/M_PI > 0.01)
                {
                    timer->stop(); // 停止定时器
                    timer->destroyed();

                    if(!stop_signal)
                    {
                        this->ui->show_status->addItem(addTime("直线运动失败！！！(关节空间)"));
                        this->ui->show_status->addItem("\n");
                        this->ui->show_status->scrollToBottom();
                        stop_signal = false;
                    }
                    return;
                }
            }
            timer->stop(); // 停止定时器
            timer->destroyed();
            this->ui->show_status->addItem(addTime("机器人直线运动成功(关节空间)"));
            this->ui->show_status->addItem("\n");
            this->ui->show_status->scrollToBottom();
            return;
        }
        else
        {
            robotService.robotServiceGetRobotCurrentState(this->robotstate);
        }
    });
    timer->start(10); // 定时器每10毫秒触发一次
/******************************************************************************************/
}

bool checkXYZRPY(ServiceInterface &robotService, const aubo_robot_namespace::wayPoint_S &wp, const aubo_robot_namespace::wayPoint_S &wpFK)
{
    aubo_robot_namespace::Rpy wp_rpy, wpFK_rpy;
    robotService.quaternionToRPY(wp.orientation,wp_rpy);
    robotService.quaternionToRPY(wpFK.orientation,wpFK_rpy);
    if(fabs(wp.cartPos.position.x-wpFK.cartPos.position.x)<0.01 &&
       fabs(wp.cartPos.position.y-wpFK.cartPos.position.y)<0.01 &&
       fabs(wp.cartPos.position.z-wpFK.cartPos.position.z)<0.01 &&
       fabs(wp_rpy.rx-wpFK_rpy.rx)<0.01 &&
       fabs(wp_rpy.ry-wpFK_rpy.ry)<0.01 &&
       fabs(wp_rpy.rz-wpFK_rpy.rz)<0.01)
        return true;
    else
        return false;
}

void Widget::on_E_jointMove_clicked()
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    aubo_robot_namespace::wayPoint_S wp;
    wp.cartPos.position.x = this->ui->X->text().toDouble();
    wp.cartPos.position.y = this->ui->Y->text().toDouble();
    wp.cartPos.position.z = this->ui->Z->text().toDouble();
    aubo_robot_namespace::Rpy rpy_tmp;
    rpy_tmp.rx = this->ui->RX->text().toDouble()*M_PI/180;
    rpy_tmp.ry = this->ui->RY->text().toDouble()*M_PI/180;
    rpy_tmp.rz = this->ui->RZ->text().toDouble()*M_PI/180;
    robotService.RPYToQuaternion(rpy_tmp,wp.orientation);

    wp = aubo_IK(robotService, wp);

    robotService.robotServiceJointMove(wp, false);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        this->ui->show_status->addItem(addTime("关节运动失败！！！(笛卡尔空间)"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }

    usleep(0.1*1000000);  // 1000000微秒等于1秒，延时100ms防止机器人未启动
/******************************************************************************************/
    /*获取机械臂当前运行状态*/
    robotService.robotServiceGetRobotCurrentState(this->robotstate);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (robotstate == 0)
        {
            /*获取机械臂当前关节角信息*/
            aubo_robot_namespace::JointParam jointangle;
            robotService.robotServiceGetJointAngleInfo(jointangle);
            /*将关节角信息转换成末端信息*/
            aubo_robot_namespace::wayPoint_S waypointFK;
            waypointFK = aubo_FK(robotService, jointangle.jointPos);
            if(checkXYZRPY(robotService,wp,waypointFK) == false)
            {
                timer->stop(); // 停止定时器
                timer->destroyed();

                if(!stop_signal)
                {
                    this->ui->show_status->addItem(addTime("关节运动失败！！！(笛卡尔空间)"));
                    this->ui->show_status->addItem("\n");
                    this->ui->show_status->scrollToBottom();
                    stop_signal = false;
                }
                return;
            }
            else
            {
                timer->stop(); // 停止定时器
                timer->destroyed();
                this->ui->show_status->addItem(addTime("机器人关节运动成功(笛卡尔空间)"));
                this->ui->show_status->addItem("\n");
                this->ui->show_status->scrollToBottom();
                return;
            }
        }
        else
        {
            robotService.robotServiceGetRobotCurrentState(this->robotstate);
        }
    });
    timer->start(10); // 定时器每10毫秒触发一次
/******************************************************************************************/
}

void Widget::on_E_lineMove_clicked()
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    aubo_robot_namespace::wayPoint_S wp;
    wp.cartPos.position.x = this->ui->X->text().toDouble();
    wp.cartPos.position.y = this->ui->Y->text().toDouble();
    wp.cartPos.position.z = this->ui->Z->text().toDouble();
    aubo_robot_namespace::Rpy rpy_tmp;
    rpy_tmp.rx = this->ui->RX->text().toDouble()*M_PI/180;
    rpy_tmp.ry = this->ui->RY->text().toDouble()*M_PI/180;
    rpy_tmp.rz = this->ui->RZ->text().toDouble()*M_PI/180;
    robotService.RPYToQuaternion(rpy_tmp,wp.orientation);

    wp = aubo_IK(robotService, wp);

    robotService.robotServiceLineMove(wp, false);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        this->ui->show_status->addItem(addTime("直线运动失败！！！(笛卡尔空间)"));
        this->ui->show_status->addItem("\n");
        this->ui->show_status->scrollToBottom();
    }

    usleep(0.1*1000000);  // 1000000微秒等于1秒，延时100ms防止机器人未启动
/******************************************************************************************/
    /*获取机械臂当前运行状态*/
    robotService.robotServiceGetRobotCurrentState(this->robotstate);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (robotstate == 0)
        {
            /*获取机械臂当前关节角信息*/
            aubo_robot_namespace::JointParam jointangle;
            robotService.robotServiceGetJointAngleInfo(jointangle);
            /*将关节角信息转换成末端信息*/
            aubo_robot_namespace::wayPoint_S waypointFK;
            waypointFK = aubo_FK(robotService, jointangle.jointPos);
            for(int i=0;i<aubo_robot_namespace::ARM_DOF;i++)
            {
                if(checkXYZRPY(robotService,wp,waypointFK) == false)
                {
                    timer->stop(); // 停止定时器
                    timer->destroyed();

                    if(!stop_signal)
                    {
                        this->ui->show_status->addItem(addTime("直线运动失败！！！(笛卡尔空间)"));
                        this->ui->show_status->addItem("\n");
                        this->ui->show_status->scrollToBottom();
                        stop_signal = false;
                    }
                    return;
                }
            }
            timer->stop(); // 停止定时器
            timer->destroyed();
            this->ui->show_status->addItem(addTime("机器人直线运动成功(笛卡尔空间)"));
            this->ui->show_status->addItem("\n");
            this->ui->show_status->scrollToBottom();
            return;
        }
        else
        {
            robotService.robotServiceGetRobotCurrentState(this->robotstate);
        }
    });
    timer->start(10); // 定时器每10毫秒触发一次
/******************************************************************************************/
}
