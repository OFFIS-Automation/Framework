# OFFIS Automation Framework
# Copyright (C) 2013-2014 OFFIS e.V.
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
include(../../properties/opencv.pro)

DESTDIR = $${targetDir}/plugins/olvisPlugins
INCLUDEPATH += ../../olvis3/include
INCLUDEPATH += ../../automation/include

LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts -lolvisPlugin
LIBS += -L$${targetDir}/plugins/ -lTutorialUnitCore

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += dll
TARGET = TutorialCamera
DESTDIR = $${targetDir}/plugins/olvisPlugins

HEADERS += \
    src/CustomFilter.h \
    src/Project.h

SOURCES += \
    src/CustomFilter.cpp \
    src/main.cpp
