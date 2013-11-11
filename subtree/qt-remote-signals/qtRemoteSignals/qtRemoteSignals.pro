QT       += core


TARGET = qtRemoteSignals
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../bin



SOURCES += src/main.cpp \
    src/main.cpp

OTHER_FILES += \
    testData.slots

HEADERS +=
