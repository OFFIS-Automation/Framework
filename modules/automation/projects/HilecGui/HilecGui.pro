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

TEMPLATE = lib

QT += core gui widgets

include(../../../properties/pathes.pro)
DESTDIR = $${targetDir}/plugins
TARGET = HilecGui

INCLUDEPATH += ../../../frontend/include
INCLUDEPATH += ../../../frontend/projects/LogWidget
INCLUDEPATH += ../../include
INCLUDEPATH += $${PWD}
LIBS += -L$${targetDir} -lLogWidget

templates.path    = $${DESTDIR}/hilec
templates.files  += ../../data/template
templates.files  += ../../data/doc
templates.files  += ../../data/examples
templates.files  += ../../data/pylint_config.txt
templates.files  += ../../data/compile.bat
INSTALLS       += templates


HEADERS += \
    src/HilecGui.h \
    src/HilecSingleton.h \
    src/debugVars/DebugVariables.h \
    src/exception/ScriptExceptionDialog.h \
    src/rcUnits/RcUnitWidget.h \
    src/rcUnits/RcHelpCreator.h \
    src/rcUnits/HilecHelpWidget.h \
    src/runConfigurations/ConfigurationWidget.h \
    src/scriptInteraction/ScriptUi.h \
    src/scriptInteraction/ScriptProgress.h \
    src/scriptInteraction/ScriptOutput.h \
    src/scriptInteraction/ScriptInput.h \
    src/createRcUnit/CreateRcUnitDialog.h \
    src/telecontrol/TelecontrolWidget.h \
    src/runConfigurations/ConfigurationListWidget.h \
    src/scriptInteraction/ScriptInfoPanel.h \
    src/HilecGuiToolbar.h \
    src/rcUnits/RcUnitContainerWidget.h \
    src/debugVars/CallStackWidget.h \
    src/ScriptErrors/ScriptErrorWidget.h \
    src/scriptInteraction/ScriptStringInput.h \
    src/telecontrol/TelecontrolHapticWidget.h \
    src/createRcUnit/AddRcUnitWizard.h \
    src/createRcUnit/SelectRcUnitWizardPage.h \
    src/createRcUnit/DefineRcUnitWizardPage.h \
    src/telecontrol/TelecontrolGamepadWidget.h \
    src/telecontrol/TelecontrolAssignmentWidget.h \
    src/telecontrol/ShowAssignmentButton.h \
    src/telecontrol/GamepadAssignmentWidget.h \
    src/telecontrol/EditGamepadAssignment.h \
    src/telecontrol/EditGamepadArea.h \
    src/rcUnits/AddRemoteRcServerDialog.h \
    src/rcUnits/RcUnitFlagWidget.h \
    src/telecontrol/TelecontrolSelectionComboBox.h \
    src/telecontrol/ConnexionAssignmentWidget.h \
    src/rcUnits/RcFlagWidgetSettingsDialog.h \
    src/scratch/ScratchWidget.h \
    src/scratch/ScratchProgramTreeWidget.h

SOURCES += \
    src/HilecGui.cpp \
    src/HilecSingleton.cpp \
    src/debugVars/DebugVariables.cpp \
    src/exception/ScriptExceptionDialog.cpp \
    src/rcUnits/RcUnitWidget.cpp \
    src/rcUnits/RcHelpCreator.cpp \
    src/rcUnits/HilecHelpWidget.cpp \
    src/runConfigurations/ConfigurationWidget.cpp \
    src/scriptInteraction/ScriptUi.cpp \
    src/scriptInteraction/ScriptProgress.cpp \
    src/scriptInteraction/ScriptOutput.cpp \
    src/scriptInteraction/ScriptInput.cpp \
    src/createRcUnit/CreateRcUnitDialog.cpp \
    src/telecontrol/TelecontrolWidget.cpp \
    src/runConfigurations/ConfigurationListWidget.cpp \
    src/telecontrol/TelecontrolGamepadWidget.cpp \
    src/scriptInteraction/ScriptInfoPanel.cpp \
    src/HilecGuiToolbar.cpp \
    src/rcUnits/RcUnitContainerWidget.cpp \
    src/debugVars/CallStackWidget.cpp \
    src/ScriptErrors/ScriptErrorWidget.cpp \
    src/scriptInteraction/ScriptStringInput.cpp \
    src/telecontrol/TelecontrolHapticWidget.cpp \
    src/createRcUnit/AddRcUnitWizard.cpp \
    src/createRcUnit/SelectRcUnitWizardPage.cpp \
    src/createRcUnit/DefineRcUnitWizardPage.cpp \
    src/telecontrol/TelecontrolAssignmentWidget.cpp \
    src/telecontrol/ShowAssignmentButton.cpp \
    src/telecontrol/GamepadAssignmentWidget.cpp \
    src/telecontrol/EditGamepadAssignment.cpp \
    src/telecontrol/EditGamepadArea.cpp \
    src/rcUnits/AddRemoteRcServerDialog.cpp \
    src/rcUnits/RcUnitFlagWidget.cpp \
    src/telecontrol/TelecontrolSelectionComboBox.cpp \
    src/telecontrol/ConnexionAssignmentWidget.cpp \
    src/rcUnits/RcFlagWidgetSettingsDialog.cpp \
    src/scratch/ScratchWidget.cpp \
    src/scratch/ScratchProgramTreeWidget.cpp


FORMS += \
    src/debugVars/DebugVariables.ui \
    src/exception/ScriptExceptionDialog.ui \
    src/rcUnits/RcUnitWidget.ui \
    src/rcUnits/HilecHelpWidget.ui \
    src/runConfigurations/ConfigurationWidget.ui \
    src/scriptInteraction/ScriptUi.ui \
    src/scriptInteraction/ScriptProgress.ui \
    src/scriptInteraction/ScriptOutput.ui \
    src/scriptInteraction/ScriptInput.ui \
    src/createRcUnit/CreateRcUnitDialog.ui \
    src/telecontrol/TelecontrolWidget.ui \
    src/scriptInteraction/ScriptInfoPanel.ui \
    src/HilecGuiToolbar.ui \
    src/rcUnits/RcUnitContainerWidget.ui \
    src/debugVars/CallStackWidget.ui \
    src/ScriptErrors/ScriptErrorWidget.ui \
    src/scriptInteraction/ScriptStringInput.ui \
    src/telecontrol/TelecontrolHapticWidget.ui \
    src/createRcUnit/AddRcUnitWizard.ui \
    src/telecontrol/TelecontrolAssignmentWidget.ui \
    src/telecontrol/ShowAssignmentButton.ui \
    src/telecontrol/GamepadAssignmentWidget.ui \
    src/telecontrol/EditGamepadAssignment.ui \
    src/telecontrol/EditGamepadArea.ui \
    src/rcUnits/AddRemoteRcServerDialog.ui \
    src/rcUnits/RcUnitFlagWidget.ui \
    src/telecontrol/TelecontrolGamepadWidget.ui \
    src/telecontrol/TelecontrolSelectionComboBox.ui \
    src/telecontrol/ConnexionAssignmentWidget.ui \
    src/scratch/ScratchWidget.ui


RESOURCES += \
    images/images.qrc
















