Section ""
	SetOutPath "$INSTDIR"
	File "license.txt"
	File "$%OFFIS_DEVELOPMENT_ENVIRONMENT%\tools\installer\vcredist_x86.exe"
	ExecWait "vcredist_x86.exe /q"
	Delete "vcredist_x86.exe"
SectionEnd

Section "!OFFIS Automation Framework" SecToolbox
	SectionIn RO
	SetOutPath "$INSTDIR"
	File "..\..\bin\*.exe"
	File "..\..\bin\Notifications.dll"
	File "..\..\bin\Qt5*.dll"
	File "..\..\bin\icu*.dll"
	File "..\..\bin\gamepads.ini"
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
	
	SetOutPath "$INSTDIR\plugins\rcUnits"
	File "..\..\bin\plugins\rcUnits\VisionControlUnit.dll"
	File "..\..\bin\plugins\rcUnits\TutorialUnit.dll"
	File "..\..\bin\plugins\rcUnits\SensorSystem.dll"
	
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\plugins\hilec\python\rcunits\olvis.py"
	
	SetOutPath "$INSTDIR\plugins"
	File "..\..\bin\plugins\Hilec*.dll"
	File "..\..\bin\plugins\RcUnit*.dll"
	File "..\..\bin\plugins\Remote*.dll"
	File "..\..\bin\plugins\ProjectEditor.dll"
	File "..\..\bin\plugins\qscintilla2.dll"
	File "..\..\bin\plugins\python35.dll"
	File "..\..\bin\plugins\python.exe"
	File "..\..\bin\plugins\TutorialUnitCore.dll"
	File "..\..\bin\plugins\SensorSystem.dll"
	File "..\..\bin\plugins\SensorSystemGui.dll"
	File "..\..\bin\plugins\SensorTracer.dll"
	File /r "..\..\bin\plugins\sensorSystem\*"
	
	SetOutPath "$INSTDIR\plugins\hilec"
	File /r /x "..\..\bin\plugins\hilec\python\rcunits" "..\..\bin\plugins\hilec\*"
	
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR\platforms"
	File /r "..\..\bin\platforms\*"
SectionEnd

Section /o "Development version" SecDevFiles
	SetOutPath "$INSTDIR\debug"
	File "..\..\bin\debug\*.exe"
	File "..\..\bin\debug\Notifications.dll"
	File "..\..\bin\debug\Qt5*.dll"
	File "..\..\bin\debug\icu*.dll"
	File "..\..\bin\debug\gamepads.ini"
	File "..\..\bin\debug\opencv*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\debug\plugins\olvis*.dll"
	File "..\..\bin\debug\plugins\opencv*d.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvisGuiPlugins"
	File "..\..\bin\debug\plugins\olvisGuiPlugins\*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvisPlugins"
	File "..\..\bin\debug\plugins\olvisPlugins\*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvis"
	File /r "..\..\bin\debug\plugins\olvis\*"
	
	SetOutPath "$INSTDIR\plugins\rcUnits"
	File "..\..\bin\debug\plugins\rcUnits\VisionControlUnit.dll"
	File "..\..\bin\debug\plugins\rcUnits\TutorialUnit.dll"
	File "..\..\bin\debug\plugins\rcUnits\SensorSystem.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\debug\plugins\hilec\python\rcunits\olvis.py"

	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\debug\plugins\Hilec*.dll"
	File "..\..\bin\debug\plugins\RcUnit*.dll"
	File "..\..\bin\debug\plugins\Remote*.dll"
	File "..\..\bin\debug\plugins\ProjectEditor.dll"
	File "..\..\bin\debug\plugins\qscintilla2.dll"
	File "..\..\bin\debug\plugins\python35_d.dll"
	File "..\..\bin\debug\plugins\python.exe"
	File "..\..\bin\debug\plugins\TutorialUnitCore.dll"
	File "..\..\bin\debug\plugins\SensorSystem.dll"
	File "..\..\bin\debug\plugins\SensorSystemGui.dll"
	File "..\..\bin\debug\plugins\SensorTracer.dll"
	File /r "..\..\bin\debug\plugins\sensorSystem\*"
	
	SetOutPath "$INSTDIR\debug\plugins\hilec"
	File /r /x "..\..\bin\debug\plugins\hilec\python\rcunits" "..\..\bin\debug\plugins\hilec\*" 
	
	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\debug\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR\platforms"
	File /r "..\..\bin\debug\platforms\*"
SectionEnd

;--------------------------------
;Descriptions

;Language strings
LangString DESC_SecToolbox ${LANG_ENGLISH} "Files for the OFFIS Automation Framework"
LangString DESC_SecDevFiles ${LANG_ENGLISH} "Development build of the OFFIS Automation Framework. This is required to develop own vision or automation plugins."

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
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\${NAME}.lnk" "$INSTDIR\Framework.exe"
	CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall ${NAME}.lnk" "$INSTDIR\Uninstall.exe"
    
	!insertmacro MUI_STARTMENU_WRITE_END
	
	WriteRegExpandStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "${REGKEY}" "$INSTDIR"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
	WriteUninstaller "uninstall.exe"
	!addplugindir "."
	AccessControl::GrantOnFile "$INSTDIR" "(BU)" "FullAccess"
	AccessControl::EnableFileInheritance "$INSTDIR"
SectionEnd

Section "Uninstall"

  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  Delete $INSTDIR\*.dll
  Delete $INSTDIR\*.log
  Delete $INSTDIR\*.bin
  Delete $INSTDIR\*.txt
  Delete $INSTDIR\*.ini
  Delete $INSTDIR\*.dat
  RMDir /r "$INSTDIR\plugins"
  RMDir /r "$INSTDIR\debug"
  RMDir /r "$INSTDIR\driver"
  RMDir /r "$INSTDIR\platforms"
  RMDir "$INSTDIR"
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  SetShellVarContext all
  Delete "$SMPROGRAMS\$StartMenuFolder\${NAME}.lnk" 
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall ${NAME}.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  ; Remove registry keys  
  DeleteRegKey HKLM "SOFTWARE\${REGKEY}"
  DeleteRegValue HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "${REGKEY}"
  
SectionEnd
