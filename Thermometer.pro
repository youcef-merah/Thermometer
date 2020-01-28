QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += THERMOMETER_LIBRARY

TARGET = thermometer
DESTDIR = ../../lib

TEMPLATE = lib

SOURCES += \
    thermometer.cpp

HEADERS += \
    thermometer.h \
    thermometer_global.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
