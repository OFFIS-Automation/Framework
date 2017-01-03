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


win32-msvc*{
	INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/include/gstreamer-1.0
	INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/include/glib-2.0
	INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/glib-2.0/include
	INCLUDEPATH += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/gstreamer-1.0/include

	LIBS += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/gstreamer-1.0.lib
	LIBS += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/gstapp-1.0.lib
	LIBS += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/glib-2.0.lib
	LIBS += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/gstreamer/1.0/x86/lib/gobject-2.0.lib
}
