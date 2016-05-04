echo off
cd /d %0\..
set WORKDIR=%CD%
set PYTHONPATH=%WORKDIR%\python
cd /d "%2"
"%WORKDIR%\..\python.exe" "%PYTHONPATH%\pylint\lint.py" --rcfile="%WORKDIR%\pylint_config.txt" %1
cd %WORKDIR%