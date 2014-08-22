CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_Automation_Framework)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_Automation_Framework)/plugins)
}
TEMPLATE = lib
CONFIG += dll

TARGET = CustomLolec
DESTDIR = $${targetDir}/lolecs

INCLUDEPATH += $${targetDir}/hilec/include

HEADERS += \
    src/CustomLolecInterface.h \
    src/CustomLolec.h \
    src/CustomLolecGui.h

SOURCES += \
    src/CustomLolecInterface.cpp \
    src/CustomLolec.cpp \
    src/CustomLolecGui.cpp

FORMS += \
    src/CustomLolecGui.ui
