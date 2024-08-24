#include "widget.h"
#include <QApplication>

#include <iostream>

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();

    return a.exec();
}
