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

dlls.path  =  $${DESTDIR}
dllA.path   += $${DESTDIR}/platforms
dllB.path   += $${DESTDIR}/plugins/imageformats/

CONFIG(debug, debug|release) {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Cored.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Networkd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Guid.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgetsd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGld.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupportd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPortd.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgetsd.dll
} else {
    dlls.files += $$[QT_INSTALL_BINS]/icudt5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuin5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/icuuc5*.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Core.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Network.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Gui.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgets.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5OpenGl.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5PrintSupport.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5SerialPort.dll
    dlls.files += $$[QT_INSTALL_BINS]/Qt5Widgets.dll
}
dllA.files  += $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qico.dll
dllB.files  += $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp.dll
INSTALLS   += dlls dllA dllB
