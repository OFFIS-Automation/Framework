# OFFIS Automation Framework
# Copyright (C) 2013-2014 OFFIS e.V.
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

SUBDIRS += projects/RcUnits
SUBDIRS += projects/remoteClient
SUBDIRS += projects/hilec
SUBDIRS += projects/HilecGui
SUBDIRS += projects/QScintilla
SUBDIRS += projects/ProjectEditor
SUBDIRS += projects/RemoteRcUnitServer

include(../properties/pathes.pro)

TRANSLATIONS = $${translationDir}/de/automation_de.ts \
    $${translationDir}/stub/automation_stub.ts
