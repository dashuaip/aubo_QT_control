INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

#*** 功能示例　***#
HEADERS += \
    $$PWD/example_1.h \
    $$PWD/example_3.h \
    $$PWD/example_4.h \
    $$PWD/example_5.h \
    $$PWD/example_6.h \
    $$PWD/example_8.h \
    $$PWD/example_9.h \
    $$PWD/init.h \
    $$PWD/util.h \
    $$PWD/example_toolio.h \

SOURCES += \
    $$PWD/example_1.cpp \
    $$PWD/example_3.cpp \
    $$PWD/example_4.cpp \
    $$PWD/example_5.cpp \
    $$PWD/example_6.cpp \
    $$PWD/example_8.cpp \
    $$PWD/example_9.cpp \
    $$PWD/init.cpp \
    $$PWD/util.cpp \
    $$PWD/example_toolio.cpp \

LIBS+=-lpthread
