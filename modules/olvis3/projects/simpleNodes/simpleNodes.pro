# OFFIS Automation Framework
# Copyright (C) 2013-2015 OFFIS e.V.
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

include(../../../properties/pathes.pro)
include(../../../properties/opencv.pro)

INCLUDEPATH += ../../../sensorSystem/include
LIBS += -L$${targetDir}/plugins/ -lSensorSystem

DESTDIR = $${targetDir}/plugins/olvisPlugins
TARGET = SimpleNodes
LIBS += -L$${PWD}/lib -L$${targetDir}/plugins -lolvisFilter -lolvisPorts  -lolvisPlugin

include(simpleNodesFiles.pri)
