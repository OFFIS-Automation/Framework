# OFFIS Automation Framework
# Copyright (C) 2013-2017 OFFIS e.V.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.

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
