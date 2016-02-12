CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_AUTOMATION_FRAMEWORK)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_AUTOMATION_FRAMEWORK)/plugins)
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
    
