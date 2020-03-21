@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
echo.
echo Packaging GamsPlugins for optimized release
echo -------Settings-------
echo UE4_ROOT=%UE4_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------

set /p VERSION=<"%PROJECT_ROOT%\version.txt"

setlocal enableextensions
md "%SCRIPTS_DIR%\..\..\Binaries\Win64"

call "%UE4_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%PROJECT_FILE%" -noP4 -build -cook -stage -stagingdirectory="%PROJECT_ROOT%\out" -pak

powershell -NoLogo -NoProfile -ExecutionPolicy Bypass -Command ^ Compress-Archive -Force -Path "%PROJECT_ROOT%\out\WindowsNoEditor\*" -DestinationPath "%PROJECT_ROOT%\out\GamsPlugins_Win64_v%VERSION%.zip"

echo Finished with script

endlocal
