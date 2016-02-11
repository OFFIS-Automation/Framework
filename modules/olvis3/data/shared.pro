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


CVINCLUDEDIR = $${PWD}/opencv/include
CVLIBDIR = $${PWD}/opencv/lib
include(opencv_version.pro)
INCLUDEPATH += $${PWD}/include
INCLUDEPATH += $$CVINCLUDEDIR
CONFIG(debug, debug|release) {
	LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION}d -lopencv_imgcodecs$${CVVERSION}d -lopencv_imgproc$${CVVERSION}d -lopencv_highgui$${CVVERSION}d -lopencv_video$${CVVERSION}d -lopencv_videoio$${CVVERSION}d -lopencv_photo$${CVVERSION}d
} else {
	LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgcodecs$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION} -lopencv_video$${CVVERSION} -lopencv_videoio$${CVVERSION} -lopencv_photo$${CVVERSION}
}
LIBS += -L$${PWD}/lib -lolvisFilter -lolvisPorts -lolvisPlugin
