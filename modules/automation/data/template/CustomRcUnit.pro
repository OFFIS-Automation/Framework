CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_AUTOMATION_FRAMEWORK)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_AUTOMATION_FRAMEWORK)/plugins)
}
TEMPLATE = lib
CONFIG += dll

TARGET = CustomRcUnit
DESTDIR = $${targetDir}/rcUnits

INCLUDEPATH += $${targetDir}/hilec/include

HEADERS += \
    $${targetDir}/hilec/include/types/HwRcUnit.h \
    src/CustomRcUnitInterface.h \
    src/CustomRcUnit.h

SOURCES += \
    src/CustomRcUnitInterface.cpp \
    src/CustomRcUnit.cpp
