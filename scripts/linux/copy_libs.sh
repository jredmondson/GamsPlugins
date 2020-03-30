#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPTS_DIR")")"
MADARA_VERSION=`cat $MADARA_ROOT/VERSION.txt`
GAMS_VERSION=`cat $GAMS_ROOT/VERSION.txt`
MADARA_SOVERSION=${MADARA_VERSION%.*}
GAMS_SOVERSION=${GAMS_VERSION%.*}

echo ""
echo "COPYING LIBS to GamsPlugins"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "PROJECT_ROOT=$PROJECT_ROOT"
echo "MADARA_ROOT=$MADARA_ROOT"
echo "MADARA_VERSION=$MADARA_VERSION"
echo "MADARA_SOVERSION=$MADARA_SOVERSION"
echo "GAMS_ROOT=$GAMS_ROOT"
echo "GAMS_VERSION=$GAMS_VERSION"
echo "GAMS_SOVERSION=$GAMS_SOVERSION"
echo "OSC_ROOT=$OSC_ROOT"
echo "----------------------"

mkdir $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Linux
mkdir $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Linux/

echo "Copying GAMS lib"
cp $GAMS_ROOT/install/lib/libgams.so.$GAMS_SOVERSION $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Linux/libgams.so

echo "Copying oscpack lib"
cp $OSC_ROOT/liboscpack.so.1.1.0 $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Linux/liboscpack.so

echo "Copying MADARA lib"
cp $MADARA_ROOT/install/lib/libmadara.so.$MADARA_SOVERSION $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Linux/libmadara.so
