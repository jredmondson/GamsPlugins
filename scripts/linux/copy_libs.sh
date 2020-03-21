#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPTS_DIR")")"

echo ""
echo "COPYING LIBS to GamsPlugins"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "PROJECT_ROOT=$PROJECT_ROOT"
echo "MADARA_ROOT=$MADARA_ROOT"
echo "GAMS_ROOT=$GAMS_ROOT"
echo "OSC_ROOT=$OSC_ROOT"
echo "----------------------"

echo "Copying GAMS lib"
cp $GAMS_ROOT/lib/libGAMS.so $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Linux

echo "Copying oscpack lib"
cp $OSC_ROOT/liboscpack.so.1.1.0 $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Linux/liboscpack.so

echo "Copying MADARA lib"
cp $MADARA_ROOT/lib/libMADARA.so $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Linux
