# OFFIS Automation Framework
# Copyright (C) 2013-2016 OFFIS e.V.
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

VPATH += $$PWD

INCLUDEPATH += $$PWD/../../include
LIBS += -L$${targetDir}/plugins -lolvisFilter
TEMPLATE = lib

DEFINES += DEFAULTPORTS_LIBRARY


SOURCES += \
    src/IntegerPort.cpp \
    src/ImagePort.cpp \
    src/FilePort.cpp \
    src/GrayImage.cpp \
    src/DirectoryPort.cpp \
    src/StringPort.cpp \
    src/RealPort.cpp \
    src/RectPort.cpp \
    src/BooleanPort.cpp \
    src/Pose2d.cpp \
    src/PosePort.cpp \
    src/PointPort.cpp \
    src/PolygonPort.cpp \
    src/LinePort.cpp \
    src/DepthMap.cpp \
    src/DepthMapPort.cpp \
    src/Histogram.cpp \
    src/HistogramPort.cpp \
    src/RgbImage.cpp \
    src/RgbaImage.cpp \
    src/DateTimePort.cpp \
    $$PWD/src/special/PhysicalSizePort.cpp \
    $$PWD/src/ColorPort.cpp

HEADERS += \
    ../../include/ports/IntegerPort.h \
    ../../include/ports/ImagePort.h \
    ../../include/ports/FilePort.h \
    ../../include/ports/DefaultPortsGlobal.h \
    ../../include/ports/GrayImage.h \
    ../../include/ports/DirectoryPort.h \
    ../../include/ports/StringPort.h \
    ../../include/ports/RealPort.h \
    ../../include/ports/RectPort.h \
    ../../include/ports/PointPort.h \
    ../../include/ports/BooleanPort.h \
    ../../include/ports/PosePort.h \
    ../../include/ports/Pose2d.h \
    ../../include/ports/PolygonPort.h \
    ../../include/ports/LinePort.h \
    ../../include/ports/StringPort.h \
    ../../include/ports/RgbImage.h \
    ../../include/ports/RectPort.h \
    ../../include/ports/RealPort.h \
    ../../include/ports/PosePort.h \
    ../../include/ports/Pose2d.h \
    ../../include/ports/PolygonPort.h \
    ../../include/ports/PointPort.h \
    ../../include/ports/LinePort.h \
    ../../include/ports/IntegerPort.h \
    ../../include/ports/ImagePort.h \
    ../../include/ports/GrayImage.h \
    ../../include/ports/FilePort.h \
    ../../include/ports/DirectoryPort.h \
    ../../include/ports/DepthMapPort.h \
    ../../include/ports/DepthMap.h \
    ../../include/ports/DefaultPortsGlobal.h \
    ../../include/ports/BooleanPort.h \
    ../../include/ports/HistogramPort.h \
    ../../include/ports/Histogram.h \
    ../../include/ports/DateTimePort.h \
    $$PWD/../../include/ports/special/PhysicalSizePort.h \
    $$PWD/../../include/ports/ColorPort.h

CONFIG+=dll

RESOURCES += \
    resource/resources.qrc
