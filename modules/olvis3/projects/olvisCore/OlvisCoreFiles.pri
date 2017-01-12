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

TEMPLATE = lib

VPATH += $$PWD

DEFINES += OVCORE_LIBRARY
INCLUDEPATH += $${PWD}/../../include
INCLUDEPATH += $${PWD}/../../../frontend/projects/Notifications/include
LIBS += -L$${targetDir}/plugins -L$${targetDir} -lolvisFilter -lolvisPorts -lNotifications
LIBS += -L$${targetDir} -lNotifications


CONFIG+=dll

HEADERS += \
    src/OlvisCoreInterface.h \
    ../../include/core/ProcessorInfo.h \
    ../../include/core/PortInfo.h \
    ../../include/core/OlvisInterface.h \
    ../../include/core/FilterTypeInfo.h \
    ../../include/core/FilterInfo.h \
    ../../include/filter/UserFilter.h \
    src/CoreGlobal.h \
    ../../include/filter/FilterPluginInterface.h \
    src/Processor.h \
    src/ProcessorOutputPort.h \
    ../../include/core/PortId.h \
    ../../include/core/FilterConnection.h \
    ../../include/core/PortListener.h \
    src/ConfigWriter.h \
    src/ConfigReader.h \
    src/DataJoin.h \
    src/DataBuffer.h \
    src/ProcessorTriggerInterface.h \
    src/ProcessorInputPort.h \
    src/MakroFilter.h \
    src/FilterGroup.h

SOURCES += \
    src/OlvisCoreInterface.cpp \
    src/Processor.cpp \
    src/ConfigWriter.cpp \
    src/ConfigReader.cpp \
    src/DataJoin.cpp \
    src/DataBuffer.cpp \
    src/ProcessorOutputPort.cpp \
    src/ProcessorInputPort.cpp \
    src/MakroFilter.cpp \
    src/FilterGroup.cpp
