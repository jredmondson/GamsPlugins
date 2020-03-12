@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%
SET PROJECT_ROOT=%SCRIPTS_DIR:~0,-16%
SET PROJECT_FILE=%PROJECT_ROOT%\GamsPlugins.uproject
echo.
echo COPYING LIBS to GamsPlugins
echo -------Settings-------
echo GAMS_ROOT=%GAMS_ROOT%
echo MADARA_ROOT=%MADARA_ROOT%
echo OSC_ROOT=%OSC_ROOT%
echo SCRIPTS_DIR=%SCRIPTS_DIR%
echo PROJECT_ROOT=%PROJECT_ROOT%
echo PROJECT_FILE=%PROJECT_FILE%
echo ----------------------

echo "Copying GAMS dll and lib"
copy "%GAMS_ROOT%\lib\GAMS.lib" "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"
copy "%GAMS_ROOT%\lib\GAMS.dll" "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"

echo "Copying oscpack dll"
copy %OSC_ROOT%\Release\oscpack.lib "%PROJECT_ROOT%\Plugins\GamsLibrary\ThirdParty\gams\Win64"

echo "Copying MADARA lib and lib"
copy "%MADARA_ROOT%\lib\MADARA.lib" "%PROJECT_ROOT%\Plugins\MadaraLibrary\ThirdParty\madara\Win64"
copy "%MADARA_ROOT%\lib\MADARA.dll" "%PROJECT_ROOT%\Plugins\MadaraLibrary\ThirdParty\madara\Win64"
