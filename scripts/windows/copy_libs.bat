@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
set /p GAMS_VERSION=<"%GAMS_ROOT%\Version.txt"
set /p MADARA_VERSION=<"%MADARA_ROOT%\Version.txt"

echo.
echo COPYING LIBS to GamsPlugins
echo -------Settings-------
echo GAMS_ROOT=%GAMS_ROOT%
echo GAMS_VERSION=%GAMS_VERSION%
echo MADARA_ROOT=%MADARA_ROOT%
echo MADARA_VERSION=%MADARA_VERSION%
echo OSC_ROOT=%OSC_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------

mkdir "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"
mkdir "%PROJECT_ROOT%\Plugins\MadaraLibrary\ThirdParty\madara\Win64"

echo "Copying GAMS dll and lib"
copy "%GAMS_ROOT%\install\lib\gams.lib" "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"
copy "%GAMS_ROOT%\install\lib\gams.dll" "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"

echo "Copying oscpack dll"
copy %OSC_ROOT%\Release\oscpack.lib "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"

echo "Copying MADARA lib and lib"
copy "%MADARA_ROOT%\install\lib\madara.lib" "%PROJECT_ROOT%\Plugins\MadaraLibrary\ThirdParty\madara\Win64"
copy "%MADARA_ROOT%\install\lib\madara.dll" "%PROJECT_ROOT%\Plugins\MadaraLibrary\ThirdParty\madara\Win64"
