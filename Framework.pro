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

TEMPLATE = subdirs
CONFIG += ordered

# Do all the other stuff
SUBDIRS +=  subtree/qt-remote-signals/qtRemoteSignals \
            modules/version \
            modules/frontend/projects/LogWidget \
            modules/sensorSystem \
            modules/frontend/projects/AmirFrontend \
            modules/automation \
            modules/olvis3 \
            modules/TutorialPlugins


unix:!macx{
    QMAKE_CXXFLAGS_WARN_OFF += -Wno-write-strings
    QMAKE_CXXFLAGS_WARN_OFF += -Wno-deprecated
}

# Print enviroment variables
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])

unix:!macx{
    message(UNIX-Build)
}
macx{
    message(Mac-Build)
}
win32-msvc*{
    message(Windows-Build)
}

