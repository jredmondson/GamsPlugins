@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
echo.
echo RUNNING GamsPlugins in GAME MODE
echo ----------------------
echo UE4_ROOT=%UE4_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------

set ADDRESS="239.255.0.1:4150"
set WAIT_TIME=180.0

"%MADARA_ROOT%\bin\karl" -m %ADDRESS% -y 1 -ky -w %WAIT_TIME%

