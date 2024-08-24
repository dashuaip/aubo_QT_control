#ifndef WIDGET_H
#define WIDGET_H

#include "init.h"
#include "example_1.h"
#include "example_3.h"
#include "example_4.h"
#include "example_5.h"
#include "example_6.h"
#include "example_8.h"
#include "example_9.h"
#include "util.h"

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

#include <random>
#include <cmath>
#include <unistd.h>
#include <string>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
// robot_msg
    ServiceInterface robotService;

    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_connect_clicked();

    void on_origin_clicked();

    void on_stop_clicked();

    void on_reset_accvel_clicked();

    void on_shutdown_clicked();

    void on_random_Joint_clicked();

    void on_random_End_clicked();

    void on_J_jointMove_clicked();

    void on_J_lineMove_clicked();

    void on_E_jointMove_clicked();

    void on_E_lineMove_clicked();

private:
    Ui::Widget *ui;

    bool stop_signal = false;

    aubo_robot_namespace::RobotState robotstate;

};

#endif // WIDGET_H
