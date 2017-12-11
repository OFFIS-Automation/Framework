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

win32-msvc*:contains(QT_ARCH, i386):{
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x86/include/gstreamer-1.0
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x86/include/glib-2.0
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x86/lib/glib-2.0/include
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x86/lib/gstreamer-1.0/include

    LIBS += -L$$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x86/lib -lgstreamer-1.0 -lgstapp-1.0 -lglib-2.0 -lgobject-2.0
} else {
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x64/include/gstreamer-1.0
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x64/include/glib-2.0
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x64/lib/glib-2.0/include
    INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x64/lib/gstreamer-1.0/include

    LIBS += -L$$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/x64/lib -lgstreamer-1.0 -lgstapp-1.0 -lglib-2.0 -lgobject-2.0
}
