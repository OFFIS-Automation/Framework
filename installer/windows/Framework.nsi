 ;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Icon
!define MUI_ICON "icon.ico"


;General
!define VERSION "nightly-build"
!define REGKEY "OFFIS_Automation_Toolbox"
!define NAME "OFFIS Automation Toolbox-${VERSION}"

  Name "${NAME}"
  OutFile "OFFIS-Automation-Toolbox-Setup-${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${Name}"
  
  InstallDirRegKey HKLM "Software\${REGKEY}" "Install_Dir"
  
  Var StartMenuFolder
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${REGKEY}" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${Name}"  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English"

 
;--------------------------------
;Installer Sections

Section ""
	SetOutPath "$INSTDIR"
	File "license.txt"
	File "$%AmirDevDir%\tools\installer\vcredist_x86.exe"
	ExecWait "vcredist_x86.exe /q"
	Delete "vcredist_x86.exe"
SectionEnd

Section "!OFFIS Automation Toolbox" SecToolbox
	SectionIn RO
	SetOutPath "$INSTDIR"
	File /x "opencv*d.dll" "..\..\bin\opencv*.dll"
	SetOutPath "$INSTDIR\plugins"
	File "..\..\bin\plugins\olvis*.dll"
	File /x "opencv*d.dll" "..\..\bin\plugins\opencv*.dll"
	
	SetOutPath "$INSTDIR\plugins\olvisGuiPlugins"
	File "..\..\bin\plugins\olvisGuiPlugins\*.dll"
	
	SetOutPath "$INSTDIR\plugins\olvisPlugins"
	File "..\..\bin\plugins\olvisPlugins\*.dll"
	SetOutPath "$INSTDIR\plugins\olvis"
	File /r "..\..\bin\plugins\olvis\*"
	SetOutPath "$INSTDIR\plugins\lolecs"
	File /r "..\..\bin\plugins\lolecs\OlvisLolec.dll"
	File /r "..\..\bin\plugins\lolecs\TutorialUnit.dll"
	File /r "..\..\bin\plugins\lolecs\SensorSystem.dll"
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\plugins\hilec\python\rcunits\olvis.py"
	SetOutPath "$INSTDIR\plugins"
	File "..\..\bin\plugins\Hilec*.dll"
	File "..\..\bin\plugins\RcUnit*.dll"
	File "..\..\bin\plugins\ProjectEditor.dll"
	File "..\..\bin\plugins\qscintilla2.dll"
	File "..\..\bin\plugins\python32.dll"
	File "..\..\bin\plugins\python.exe"
	
	File "..\..\bin\plugins\SensorSystem.dll"
	File "..\..\bin\plugins\SensorSystemGui.dll"
	File "..\..\bin\plugins\SensorTracer.dll"
	File /r "..\..\bin\plugins\sensorSystem"
	
	SetOutPath "$INSTDIR\plugins\hilec"
	File /r /x "..\..\bin\plugins\hilec\python\rcunits" "..\..\bin\plugins\hilec\*" 
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR"
	File "..\..\bin\*.exe"

	File "$%AmirDevDir%\qt\bin\QtCore4.dll"
	File "$%AmirDevDir%\qt\bin\QtGui4.dll"
	File "$%AmirDevDir%\qt\bin\QtNetwork4.dll"
	File "$%AmirDevDir%\qt\bin\QtOpenGl4.dll"
	
	
	
SectionEnd

Section /o "Development version" SecDevFiles
	SetOutPath "$INSTDIR\debug"
	File "..\..\bin\debug\opencv*.dll"
	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\debug\plugins\olvis*.dll"
	File "..\..\bin\debug\plugins\opencv*d.dll"
	SetOutPath "$INSTDIR\debug\plugins\olvisGuiPlugins"
	File "..\..\bin\debug\plugins\olvisGuiPlugins\*.dll"
	
	SetOutPath "$INSTDIR\\debug\plugins\olvisPlugins"
	File "..\..\bin\debug\plugins\olvisPlugins\*.dll"
	SetOutPath "$INSTDIR\debug\plugins\olvis"
	File /r "..\..\bin\debug\plugins\olvis\*"
	SetOutPath "$INSTDIR\debug\plugins\lolecs"
	File /r "..\..\bin\debug\plugins\lolecs\OlvisLolec.dll"
	File /r "..\..\bin\debug\plugins\lolecs\TutorialUnit.dll"
	File /r "..\..\bin\debug\plugins\lolecs\SensorSystem.dll"

	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\debug\plugins\hilec\python\rcunits\olvis.py"
	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\debug\plugins\Hilec*.dll"
	File "..\..\bin\debug\plugins\RcUnit*.dll"
	File "..\..\bin\debug\plugins\ProjectEditor.dll"
	File "..\..\bin\debug\plugins\qscintilla2.dll"
	File "..\..\bin\debug\plugins\python32.dll"
	File "..\..\bin\debug\plugins\python32_d.dll"
	File "..\..\bin\debug\plugins\python.exe"
	File "..\..\bin\debug\plugins\SensorSystem.dll"
	File "..\..\bin\debug\plugins\SensorTracer.dll"
	File "..\..\bin\debug\plugins\SensorSystemGui.dll"
	File /r "..\..\bin\debug\plugins\sensorSystem"
	
	SetOutPath "$INSTDIR\debug\plugins\hilec"
	File /r /x "..\..\bin\debug\plugins\hilec\python\rcunits" "..\..\bin\debug\plugins\hilec\*" 
	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\debug\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR\debug"
	File "..\..\bin\debug\*.exe"
	
	SetOutPath "$INSTDIR\debug\plugins"
	
	
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecToolbox ${LANG_ENGLISH} "Files for the OFFIS automation toolbox"
  LangString DESC_SecDevFiles ${LANG_ENGLISH} "Development build of the Toolbox. Needed to develop own vision or automation plugins."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecToolbox} $(DESC_SecToolbox)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecDevFiles} $(DESC_SecDevFiles)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------


Section ""

	WriteRegStr HKLM "SOFTWARE\${REGKEY}" "Install_Dir" "$INSTDIR"
	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
	SetShellVarContext all

		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\${Name}.lnk" "$INSTDIR\Toolbox.exe"
;		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\RemoteServer.lnk" "$INSTDIR\RemoteLolecServer.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall ${Name}.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
	
	WriteRegExpandStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "${REGKEY}" "$INSTDIR"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
	WriteUninstaller "uninstall.exe"
	!addplugindir "."
	AccessControl::GrantOnFile "$INSTDIR" "(S-1-5-32-545)" "FullAccess"
SectionEnd

Section "Uninstall"

  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  Delete $INSTDIR\*.dll
  Delete $INSTDIR\*.log
  Delete $INSTDIR\*.bin
  Delete $INSTDIR\*.txt
  RMDir /r "$INSTDIR\plugins"
  RMDir /r "$INSTDIR\debug"
  RMDir /r "$INSTDIR\driver"
  RMDir "$INSTDIR"
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  SetShellVarContext all
  Delete "$SMPROGRAMS\$StartMenuFolder\${NAME}.lnk" 
;  Delete "$SMPROGRAMS\$StartMenuFolder\RemoteServer.lnk" 
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall ${Name}.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

	; Remove registry keys  
  DeleteRegKey HKLM "SOFTWARE\${REGKEY}"
  DeleteRegValue HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "${REGKEY}"
  
SectionEnd
