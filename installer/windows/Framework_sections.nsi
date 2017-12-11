Section ""
	SetOutPath "$INSTDIR"
	File "license.txt"
	File "$%OFFIS_DEVELOPMENT_ENVIRONMENT%\tools\installer\vcredist_x64_vs2017.exe"
	ExecWait "vcredist_x64_vs2017.exe /q"
	Delete "vcredist_x64_vs2017.exe"
	File "$%OFFIS_DEVELOPMENT_ENVIRONMENT%\tools\installer\vcredist_x86_vs2017.exe"
	ExecWait "vcredist_x86_vs2017.exe /q"
	Delete "vcredist_x86_vs2017.exe"
SectionEnd

Section "!OFFIS Automation Framework" SecToolbox
	SectionIn RO
	SetOutPath "$INSTDIR"
	File "..\..\bin\x86\Framework.exe"
	File "..\..\bin\x86\Framework.pdb"
	File "..\..\bin\x86\Notifications.dll"
	File "..\..\bin\x86\Qt5*.dll"
	File /nonfatal "..\..\bin\x86\icu*.dll"
	File "..\..\bin\x86\libEGL.dll"
	File "..\..\bin\x86\libGLESv2.dll"
	File "..\..\bin\x86\d3dcompiler_47.dll"
	File "..\..\bin\x86\opengl32sw.dll"
	File "..\..\bin\x86\WinSparkle.dll"
	File "..\..\bin\x86\gamepads.ini"
	File /x "opencv*d.dll" "..\..\bin\x86\opencv*.dll"
	
	SetOutPath "$INSTDIR\plugins"
	File "..\..\bin\x86\plugins\olvis*.dll"
	File /x "opencv*d.dll" "..\..\bin\x86\plugins\opencv*.dll"
	
	SetOutPath "$INSTDIR\plugins\olvisGuiPlugins"
	File "..\..\bin\x86\plugins\olvisGuiPlugins\*.dll"
	
	SetOutPath "$INSTDIR\plugins\olvisPlugins"
	File "..\..\bin\x86\plugins\olvisPlugins\*.dll"
	
	SetOutPath "$INSTDIR\plugins\olvis"
	File /r "..\..\bin\x86\plugins\olvis\*"
	
	SetOutPath "$INSTDIR\plugins\rcUnits"
	File "..\..\bin\x86\plugins\rcUnits\VisionControlUnit.dll"
	File "..\..\bin\x86\plugins\rcUnits\TutorialUnit.dll"
	File "..\..\bin\x86\plugins\rcUnits\SensorSystem.dll"
	
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\x86\plugins\hilec\python\rcunits\olvis.py"
	
	SetOutPath "$INSTDIR\plugins"
	File "..\..\bin\x86\plugins\Hilec*.dll"
	File /nonfatal "..\..\bin\x86\plugins\lib*.dll"
	File "..\..\bin\x86\plugins\RcUnit*.dll"
	File "..\..\bin\x86\plugins\ProjectEditor.dll"
	File "..\..\bin\x86\plugins\qscintilla2.dll"
	File "..\..\bin\x86\plugins\python35.dll"
	File "..\..\bin\x86\plugins\python.exe"
	File "..\..\bin\x86\plugins\TutorialUnitCore.dll"
	
	File "..\..\bin\x86\plugins\SensorSystem.dll"
	File "..\..\bin\x86\plugins\SensorSystemGui.dll"
	File "..\..\bin\x86\plugins\SensorTracer.dll"
	File /r "..\..\bin\x86\plugins\sensorSystem"
	
	SetOutPath "$INSTDIR\plugins\hilec"
	File /r /x "..\..\bin\x86\plugins\hilec\python\rcunits" "..\..\bin\x86\plugins\hilec\*"
	
	SetOutPath "$INSTDIR\plugins\hilec\python\rcunits"
	File "..\..\bin\x86\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR\platforms"
	File /r "..\..\bin\x86\platforms\*"
	
	SetOutPath "$INSTDIR\imageformats"
	File /r "..\..\bin\x86\imageformats\*"
	
	SetOutPath "$INSTDIR\iconengines"
	File /r "..\..\bin\x86\iconengines\*"
SectionEnd

Section /o "Development version" SecDevFiles
	SetOutPath "$INSTDIR\debug"
	File "..\..\bin\x86\debug\Framework.exe"
	File "..\..\bin\x86\debug\Framework.pdb"
	File "..\..\bin\x86\debug\Notifications.dll"
	File "..\..\bin\x86\debug\Qt5*.dll"
	File /nonfatal "..\..\bin\x86\debug\icu*.dll"
	File "..\..\bin\x86\debug\libEGLd.dll"
	File "..\..\bin\x86\debug\libGLESv2d.dll"
	File "..\..\bin\x86\debug\d3dcompiler_47.dll"
	File "..\..\bin\x86\debug\opengl32sw.dll"
	File "..\..\bin\x86\debug\WinSparkle.dll"
	File "..\..\bin\x86\debug\gamepads.ini"
	File "..\..\bin\x86\debug\opencv*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\x86\debug\plugins\olvis*.dll"
	File "..\..\bin\x86\debug\plugins\opencv*d.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvisGuiPlugins"
	File "..\..\bin\x86\debug\plugins\olvisGuiPlugins\*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvisPlugins"
	File "..\..\bin\x86\debug\plugins\olvisPlugins\*.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\olvis"
	File /r "..\..\bin\x86\debug\plugins\olvis\*"
	
	SetOutPath "$INSTDIR\debug\plugins\rcUnits"
	File "..\..\bin\x86\debug\plugins\rcUnits\VisionControlUnit.dll"
	File "..\..\bin\x86\debug\plugins\rcUnits\TutorialUnit.dll"
	File "..\..\bin\x86\debug\plugins\rcUnits\SensorSystem.dll"
	
	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\x86\debug\plugins\hilec\python\rcunits\olvis.py"

	SetOutPath "$INSTDIR\debug\plugins"
	File "..\..\bin\x86\debug\plugins\Hilec*.dll"
	File /nonfatal "..\..\bin\x86\debug\plugins\lib*.dll"
	File "..\..\bin\x86\debug\plugins\RcUnit*.dll"
	File "..\..\bin\x86\debug\plugins\ProjectEditor.dll"
	File "..\..\bin\x86\debug\plugins\qscintilla2.dll"
	File "..\..\bin\x86\debug\plugins\python35_d.dll"
	File "..\..\bin\x86\debug\plugins\python_d.exe"
	File "..\..\bin\x86\debug\plugins\TutorialUnitCore.dll"
	File "..\..\bin\x86\debug\plugins\SensorSystem.dll"
	File "..\..\bin\x86\debug\plugins\SensorSystemGui.dll"
	File "..\..\bin\x86\debug\plugins\SensorTracer.dll"
	File /r "..\..\bin\x86\debug\plugins\sensorSystem"
	
	SetOutPath "$INSTDIR\debug\plugins\hilec"
	File /r /x "..\..\bin\x86\debug\plugins\hilec\python\rcunits" "..\..\bin\x86\debug\plugins\hilec\*" 
	
	SetOutPath "$INSTDIR\debug\plugins\hilec\python\rcunits"
	File "..\..\bin\x86\debug\plugins\hilec\python\rcunits\__init__.py"
	
	SetOutPath "$INSTDIR\debug\platforms"
	File /r "..\..\bin\x86\debug\platforms\*"
	
	SetOutPath "$INSTDIR\debug\imageformats"
	File /r "..\..\bin\x86\debug\imageformats\*"
	
	SetOutPath "$INSTDIR\debug\iconengines"
	File /r "..\..\bin\x86\debug\iconengines\*"
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
  Delete $INSTDIR\*.manifest
  Delete $INSTDIR\*.dll
  Delete $INSTDIR\*.dat
  Delete $INSTDIR\*.log
  Delete $INSTDIR\*.bin
  Delete $INSTDIR\*.ini
  Delete $INSTDIR\*.txt  
  RMDir /r "$INSTDIR\plugins"
  RMDir /r "$INSTDIR\debug"
  RMDir /r "$INSTDIR\driver"
  RMDir /r "$INSTDIR\platforms"
  RMDir /r "$INSTDIR\iconengines"
  RMDir /r "$INSTDIR\imageformats"
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
