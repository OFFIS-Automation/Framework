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

CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_Automation_Toolbox)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_Automation_Toolbox)/plugins)
}
TEMPLATE = lib
CONFIG += dll

TARGET = CustomLolec
DESTDIR = $${targetDir}/lolecs

INCLUDEPATH += $${targetDir}/hilec/include

HEADERS += \
    src/CustomLolecInterface.h \
    src/CustomLolec.h \
    src/CustomLolecGui.h

SOURCES += \
    src/CustomLolecInterface.cpp \
    src/CustomLolec.cpp \
    src/CustomLolecGui.cpp

FORMS += \
    src/CustomLolecGui.ui
