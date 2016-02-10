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

#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T15:35:22
#
#-------------------------------------------------

TEMPLATE = lib

QT += core gui widgets

include(../../../properties/pathes.pro)
include(../../../properties/opencv.pro)

DESTDIR = $${targetDir}/plugins
TARGET = olvisGui

INCLUDEPATH += ../../include
INCLUDEPATH += ../../../olvis3/include
INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += $${PWD}


configs.path    = $${DESTDIR}/olvis
configs.files  += $$(OFFIS_DEVELOPMENT_ENVIRONMENT)/opencv3/opencv_version.pro
configs.files  += ../../data/*
INSTALLS       += configs

ovlibs.path     = $${DESTDIR}/olvis/lib
ovlibs.files   += $${DESTDIR}/olvisFilter.lib
ovlibs.files   += $${DESTDIR}/olvisPorts.lib
ovlibs.files   += $${DESTDIR}/olvisPlugin.lib
INSTALLS       += ovlibs

includes.path   = $${DESTDIR}/olvis/include
includes.files += ../../include/filter
includes.files += ../../include/ports
INSTALLS       += includes

opencv.path     = $${DESTDIR}/olvis/opencv
opencv.files   += $$CVINCLUDEDIR
opencv.files   += $$CVLIBDIR
INSTALLS       += opencv


SOURCES += \
    src/PluginContainer.cpp \
    src/OlvisGuiPlugin.cpp \
    src/FilterConnect/FilterWidget.cpp \
    src/FilterConnect/FilterSortingArea.cpp \
    src/FilterConnect/FilterConnectWidget.cpp \
    src/FilterInfo/FilterInfoWidget.cpp \
    src/FilterInfo/FilterInfoPortEditWidget.cpp \
    src/FilterSearch/FilterSearchWidget.cpp \
    src/FilterSearch/FilterSearchTreeItem.cpp \
    src/FilterSearch/FilterSearchTree.cpp \
    src/FilterConnect/FilterPortWidget.cpp \
    src/FilterConnect/FilterOutputPortWidget.cpp \
    src/ProcessorConnect/ProcessorsWidget.cpp \
    src/ProcessorConnect/DiagramScene.cpp \
    src/ProcessorConnect/Arrow.cpp \
    src/FilterConnect/FilterInputPortWidget.cpp \
    src/OlvisSingleton.cpp \
    src/FilterConnect/ProcessorOutputsWidget.cpp \
    src/FilterSearch/FilterSearchFlowLayout.cpp \
    src/FilterConnect/ProcessorOutputPortWidget.cpp \
    src/FilterConnect/ProcessorInputsWidget.cpp \
    src/FilterConnect/ProcessorInputPortWidget.cpp \
    src/OlvisToolbar.cpp \
    src/CreatePlugin/CreatePluginDialog.cpp \
    src/FilterConnect/FilterHeaderWidget.cpp \
    src/FilterSearch/CreateMakroFilterDialog.cpp \
    src/ProcessorConnect/ChangeProcessorDialog.cpp \
    src/ProcessorConnect/ProcessorItem.cpp

HEADERS += \
       ../../include/gui/PortEditInterface.h \
       ../../include/gui/OlvisGuiPluginInterface.h \
    src/PluginContainer.h \
    src/OlvisGuiPlugin.h \
    src/FilterConnect/FilterWidget.h \
    src/FilterConnect/FilterSortingArea.h \
    src/FilterConnect/FilterConnectWidget.h \
    src/FilterInfo/FilterInfoWidget.h \
    src/FilterInfo/FilterInfoPortEditWidget.h \
    src/FilterSearch/FilterSearchWidget.h \
    src/FilterSearch/FilterSearchTreeItem.h \
    src/FilterSearch/FilterSearchTree.h \
    src/FilterConnect/FilterPortWidget.h \
    src/FilterConnect/FilterOutputPortWidget.h \
    src/ProcessorConnect/ProcessorsWidget.h \
    src/ProcessorConnect/DiagramScene.h \
    src/ProcessorConnect/ProcessorItem.h \
    src/ProcessorConnect/Arrow.h \
    src/FilterConnect/FilterInputPortWidget.h \
    src/OlvisSingleton.h \
    src/FilterConnect/ProcessorOutputsWidget.h \
    src/FilterSearch/FilterSearchFlowLayout.h \
    src/FilterConnect/ProcessorOutputPortWidget.h \
    src/FilterConnect/ProcessorInputsWidget.h \
    src/FilterConnect/ProcessorInputPortWidget.h \
    src/OlvisToolbar.h \
    src/CreatePlugin/CreatePluginDialog.h \
    src/FilterConnect/FilterHeaderWidget.h \
    src/FilterSearch/CreateMakroFilterDialog.h \
    src/ProcessorConnect/ChangeProcessorDialog.h


FORMS += \
    src/FilterConnect/FilterWidget.ui \
    src/FilterConnect/FilterConnectWidget.ui \
    src/FilterInfo/FilterInfoWidget.ui \
    src/FilterInfo/FilterInfoPortEditWidget.ui \
    src/ProcessorConnect/ProcessorsWidget.ui \
    src/FilterSearch/FilterSearchWidget.ui \
    src/FilterConnect/FilterSortingArea.ui \
    src/FilterConnect/FilterPortWidget.ui \
    src/OlvisToolbar.ui \
    src/CreatePlugin/CreatePluginDialog.ui \
    src/FilterSearch/CreateMakroFilterDialog.ui \
    src/ProcessorConnect/ChangeProcessorDialog.ui

RESOURCES += \
    images/images.qrc

OTHER_FILES += \
    ../../include/ports/RgbImage.h
