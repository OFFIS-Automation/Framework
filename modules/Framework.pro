TEMPLATE = subdirs
CONFIG += ordered

# Do all the other stuff
SUBDIRS +=  version \
            frontend/projects/LogWidget \
            sensorSystem \
            frontend/projects/AmirFrontend \
            automation \
            olvis3 \
            TutorialPlugins \

unix:!macx{
    QMAKE_CXXFLAGS_WARN_OFF += -Wno-write-strings
    QMAKE_CXXFLAGS_WARN_OFF += -Wno-deprecated
}

# Print enviroment variables
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])

unix:!macx{
    message(UNIX-Build)
}
macx{
    message(Mac-Build)
}
win32-msvc*{
    message(Windows-Build)
}

