@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
echo Generating GamsPlugins project files
echo ----------------------
echo UE4_ROOT=%UE4_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------


setlocal enableextensions
md "%SCRIPTS_DIR%\..\..\Binaries\Win64"
"%UE4_ROOT%\Engine\Build\BatchFiles\Build.bat" -Target="GamsPlugins Win64 DebugGame -Project="%PROJECT_FILE%"" -WaitMutex -FromMsBuild
endlocal
