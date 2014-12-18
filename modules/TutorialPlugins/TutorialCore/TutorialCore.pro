#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T13:26:27
#
#-------------------------------------------------

include(../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = TutorialUnitCore

TEMPLATE = lib
CONFIG += dll

QT += core gui widgets

INCLUDEPATH += ../../frontend/include
INCLUDEPATH += ../../automation/include
INCLUDEPATH += ../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

DEFINES += TUTORIAL_LIBRARY

HEADERS += \
    src/Circle.h \
    src/GraphicsView.h \
    src/Robot.h \
    src/TutorialCore.h

SOURCES += \
    src/Cirlce.cpp \
    src/GraphicsView.cpp \
    src/Robot.cpp \
    src/TutorialCore.cpp
