CONFIG(debug, debug|release) {
    targetDir = $$quote($$(OFFIS_Automation_Framework)/debug/plugins)
} else {
    targetDir = $$quote($$(OFFIS_Automation_Framework)/plugins)
}

include($${targetDir}/olvis/shared.pro)
include(PylonCamFiles.pri)
