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


win32-msvc*{
    CVINCLUDEDIR = $$(AmirDevDir)/opencv3/include
    CVLIBDIR = $$(AmirDevDir)/opencv3/x86/vc12/lib
    CVBINDIR = $$(AmirDevDir)/opencv3/x86/vc12/bin
    INCLUDEPATH += $$CVINCLUDEDIR
    DEFINES += _CRT_SECURE_NO_WARNINGS # supress win32 stdlib warnings
    include($$(AmirDevDir)/opencv3/opencv_version.pro)
    CONFIG(debug, debug|release) {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION}d -lopencv_imgcodecs$${CVVERSION}d -lopencv_imgproc$${CVVERSION}d -lopencv_highgui$${CVVERSION}d -lopencv_video$${CVVERSION}d -lopencv_videoio$${CVVERSION}d -lopencv_photo$${CVVERSION}d
    } else {
        LIBS +=  -L$${CVLIBDIR} -lopencv_core$${CVVERSION} -lopencv_imgcodecs$${CVVERSION} -lopencv_imgproc$${CVVERSION} -lopencv_highgui$${CVVERSION} -lopencv_video$${CVVERSION} -lopencv_videoio$${CVVERSION} -lopencv_photo$${CVVERSION}
    }
}
