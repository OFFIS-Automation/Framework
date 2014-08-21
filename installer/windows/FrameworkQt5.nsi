;--------------------------------
;Include Modern UI

!include "MUI2.nsh"

;--------------------------------
;Icon
!define MUI_ICON "icon.ico"

;General
!define VERSION "nightly-build"
!define REGKEY "OFFIS_Automation_Framework"
!define NAME "OFFIS Automation Framework"

Name "${NAME}"
OutFile "OFFIS-Automation-Framework-Setup-${VERSION}.exe"

;Default installation folder
InstallDir "$PROGRAMFILES\${Name}"
InstallDirRegKey HKLM "Software\${REGKEY}" "Install_Dir"

Var StartMenuFolder

;--------------------------------
;Interface Settings
!define MUI_ABORTWARNING

;--------------------------------
;Pages
!insertmacro MUI_PAGE_WELCOME

!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${REGKEY}" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${Name}"  

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Start OFFIS Automation Framework"
!define MUI_FINISHPAGE_RUN_FUNCTION "$PROGRAMFILES\Framework.exe"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English"

 
;--------------------------------
;Installer Sections
!include "Framework_sectionsQt5.nsi"
