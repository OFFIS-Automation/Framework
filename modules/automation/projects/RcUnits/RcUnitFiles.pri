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

VPATH += $${PWD}

INCLUDEPATH += $${PWD}/../../../frontend/include
INCLUDEPATH += $${PWD}/../../include

DEFINES += RCUNITS_LIBRARY

TARGET = RcUnits
TEMPLATE = lib
CONFIG += lib

INCLUDEPATH += $${PWD}/../../../frontend/projects/Notifications/include
LIBS += -L$${targetDir}/plugins/ -L$${targetDir} -lNotifications
LIBS += -L$${targetDir} -lNotifications

SOURCES += src/RcUnit.cpp \
    src/TcInvoker.cpp \
    src/telecontrol/TelecontrolFactory.cpp\
    src/RcMethodInvoker.cpp \
    src/RcUnitBase.cpp \
    src/RcUnitsBase.cpp\
    src/telecontrol/Gamepad.cpp \
    src/MasterTcInvoker.cpp \
    src/telecontrol/WindowsTelecontrolFactory.cpp


HEADERS += src/RcUnit.h \
    src/TcInvoker.h \
    src/telecontrol/TelecontrolFactory.h\
    src/RcMethodInvoker.h \
    src/RcUnitBase.h \
    src/RcUnitsBase.h \
    src/RcUnitsGlobal.h \
    src/telecontrol/Gamepad.h \
    ../../include/telecontrol/TcConfig.h \
    src/MasterTcInvoker.h \
    src/GamepadEndpoint.h \
    src/HapticBaseEndpoint.h \
    ../../include/rc/types/HwRcUnit.h \
    ../../include/rc/types/RcUnitTypes.h \
    ../../include/rc/types/RobotRcUnit.h \
    ../../include/rc/types/UserRcUnit.h \
    ../../include/telecontrol/HapticInterface.h \
    src/telecontrol/WindowsTelecontrolFactory.h

win32*{
    HEADERS += src/telecontrol/WindowsGamepad.h \
        ../../include/telecontrol/GamepadInterface.h

    LIBS += -L$$(AmirDevDir)/directx_sdk/lib/x86 dinput8.lib dxguid.lib

    SOURCES += src/telecontrol/WindowsGamepad.cpp \
}

unix:!macx{
    HEADERS += src/telecontrol/LinuxGamepad.h \
        src/telecontrol/LinuxTelecontrolFactory.h \
        src/telecontrol/libjoystick/joystick.h \

    SOURCES += src/telecontrol/LinuxGamepad.cpp \
        src/telecontrol/LinuxTelecontrolFactory.cpp \
        src/telecontrol/libjoystick/joystick.cpp \
}
