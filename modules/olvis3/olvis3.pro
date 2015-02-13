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


SUBDIRS += projects/olvisFilter
SUBDIRS += projects/olvisPorts
SUBDIRS += projects/olvisCore
SUBDIRS += projects/OlvisCorePlugin
SUBDIRS += projects/OlvisRcUnit
SUBDIRS += projects/olvisUserExtension
SUBDIRS += projects/OlvisGuiPlugin
SUBDIRS += projects/OlvisExecGuiPlugin
SUBDIRS += projects/DefaultPorts
SUBDIRS += projects/simpleNodes
SUBDIRS += projects/ContourNodes
SUBDIRS += projects/DepthMapNodes
SUBDIRS += projects/measureNodes
#SUBDIRS += projects/sandboxedTest

include(../properties/pathes.pro)

TRANSLATIONS = $${translationDir}/de/olvis_de.ts \
    $${translationDir}/stub/olvis_stub.ts
