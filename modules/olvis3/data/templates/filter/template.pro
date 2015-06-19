CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_Automation_Toolbox)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_Automation_Toolbox)/plugins)
}

include($${targetDir}/olvis/shared.pro)

TEMPLATE = lib
CONFIG += dll
TARGET = CustomPlugin
DESTDIR = $${targetDir}/olvisPlugins

HEADERS += \
    src/CustomPlugin.h \
	src/CustomFilter.h

SOURCES += \
    src/CustomPlugin.cpp \
	src/CustomFilter.cpp
    
