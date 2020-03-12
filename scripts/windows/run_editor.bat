@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
echo.
echo Running GamsPlugins in EDITOR MODE
echo -------Settings-------
echo UE4_ROOT=%UE4_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------


setlocal enableextensions
"%UE4_ROOT%\Engine\Binaries\Win64\UE4Editor" "%PROJECT_FILE%"
endlocal


