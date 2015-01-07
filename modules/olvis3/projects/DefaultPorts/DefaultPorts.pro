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

#-------------------------------------------------
#
# Project created by QtCreator 2010-12-17T14:40:48
#
#-------------------------------------------------


TEMPLATE = lib

QT += core gui widgets

include(../../../properties/pathes.pro)
include(../../../properties/opencv.pro)
DESTDIR = $${targetDir}/plugins/olvisGuiPlugins
TARGET = defaultPorts
LIBS += -L$${targetDir}/plugins -lolvisFilter  -lolvisPorts

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../olvis3/include

DEFINES += NO_SENSOR_SYSTEM


CONFIG+=dll

HEADERS += \
    ../../include/gui/PortEditInterface.h \
    ../../include/gui/OlvisGuiPluginInterface.h \
    src/DefaultPorts.h \
    src/BoolEdit.h \
    src/AbstractPortEditWidget.h \
    src/AbstractPortEdit.h \
    src/DirectoryEdit.h \
    src/FileEdit.h \
    src/StringEdit.h \
    src/NumberEdit.h \
    src/DoubleEdit.h \
    src/ChoiceEdit.h \
    src/RectEdit.h \
    src/PointEdit.h \
    src/DateTimeEdit.h \
    ../../include/gui/OverlayInterface.h \
    src/overlays/RectOverlay.h \
    src/overlays/StringOverlay.h \
    src/overlays/ImagePortOverlay.h \
    src/overlays/Overlay.h \
    src/overlays/RectPortOverlay.h \
    src/overlays/DataOverlay.h \
    src/overlays/SimpleShapeOverlay.h \
    src/overlays/MainOverlay.h \
    src/overlays/HistogramOverlay.h \
    src/overlays/GraphOverlay.h \
    src/overlays/SensorSystemOverlay.h \
    src/overlays/ScaleBarOverlay.h \
    src/overlays/GraphOverlayOptions.h \
    src/overlays/PointPortOverlay.h


SOURCES += \
    src/DefaultPorts.cpp \
    src/BoolEdit.cpp \
    src/AbstractPortEditWidget.cpp \
    src/AbstractPortEdit.cpp \
    src/DirectoryEdit.cpp \
    src/FileEdit.cpp \
    src/StringEdit.cpp \
    src/NumberEdit.cpp \
    src/DoubleEdit.cpp \
    src/ChoiceEdit.cpp \
    src/RectEdit.cpp \
    src/PointEdit.cpp \
    src/DateTimeEdit.cpp \
    src/overlays/StringOverlay.cpp \
    src/overlays/ImagePortOverlay.cpp \
    src/overlays/Overlay.cpp \
    src/overlays/RectPortOverlay.cpp \
    src/overlays/DataOverlay.cpp \
    src/overlays/RectOverlay.cpp \
    src/overlays/SimpleShapeOverlay.cpp \
    src/overlays/MainOverlay.cpp \
    src/overlays/HistogramOverlay.cpp \
    src/overlays/GraphOverlay.cpp \
    src/overlays/SensorSystemOverlay.cpp \
    src/overlays/ScaleBarOverlay.cpp \
    src/overlays/GraphOverlayOptions.cpp \
    src/overlays/PointPortOverlay.cpp


FORMS += \
    src/AbstractPortEditWidget.ui \
    src/overlays/GraphOverlayOptions.ui \

RESOURCES += \
    images/images.qrc
