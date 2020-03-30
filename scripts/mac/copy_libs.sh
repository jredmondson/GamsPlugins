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

pushd `pwd`

cd "$PROJECT_ROOT"

mkdir $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac

mkdir $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Mac

echo "Copying GAMS lib"
cp $GAMS_ROOT/install/lib/libgams.$GAMS_SOVERSION.dylib $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac
cd $PROJECT_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac
mv libgams.$GAMS_SOVERSION.dylib libgams.dylib

echo "Copying MADARA lib"
cp $MADARA_ROOT/install/lib/libmadara.$MADARA_SOVERSION.dylib $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Mac
cd $PROJECT_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Mac
mv libmadara.$MADARA_SOVERSION.dylib libmadara.dylib

popd
