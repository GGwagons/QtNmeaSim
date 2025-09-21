QT += core
CONFIG += console c++17
TARGET = nmea_sim
TEMPLATE = app


SOURCES += \
    main.cpp \
    simulator.cpp \
    parser.cpp \
    client.cpp

HEADERS += \
    client.h \
    parser.h \
    simulator.h \


DISTFILES += \
    nmea.txt
