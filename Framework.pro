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

TEMPLATE = subdirs
CONFIG += ordered

# Do all the other stuff
SUBDIRS +=  subtree/qt-remote-signals/qtRemoteSignals \
            modules/frontend/projects/LogWidget \
            modules/frontend/projects/Notifications \
            modules/sensorSystem \
            modules/frontend/projects/AmirFrontend \
            modules/automation \
            modules/olvis3 \
            modules/TutorialPlugins

# Print enviroment variables
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Header files: $$[QT_INSTALL_HEADERS])
message(Library files: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])

win32-msvc*:contains(QMAKE_TARGET.arch, x86_64):{
    message("Building for 64 bit Windows")
} else {
    message("Building for 32 bit Windows")
}

