# OFFIS Automation Framework
# Copyright (C) 2013 OFFIS e.V.
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
# Project created by QtCreator 2011-02-14T15:21:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = olvisTest
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../../include
INCLUDEPATH += ../olvisCore/src

DESTDIR = ../../lib/debug

debug {
        LIBS += -L../../lib/debug -lolvisFilterd  -lolvisPortsd -lolvisCored
#        LIBS +=  -LC:/AMiRDevFiles/opencv/lib -lcxcore210d -lcv210d
} else {
        LIBS += -L../../lib -lolvisFilter  -lolvisPorts
#        LIBS +=  -LC:/AMiRDevFiles/opencv/lib -lcxcore210 -lcv210
}

TEMPLATE = app


SOURCES += main.cpp

