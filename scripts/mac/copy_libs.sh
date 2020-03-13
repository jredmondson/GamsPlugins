#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo ""
echo "COPYING LIBS to GamsPlugins"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "MADARA_ROOT=$MADARA_ROOT"
echo "GAMS_ROOT=$GAMS_ROOT"
echo "CAPNP_ROOT=$CAPNP_ROOT"
echo "----------------------"
cd "$SCRIPTS_DIR/../.."
export GAMSPLUGINS_ROOT=$SCRIPTS_DIR/../..

pushd `pwd`

mkdir $GAMSPLUGINS_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac

mkdir $GAMSPLUGINS_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Mac

echo "Copying GAMS lib"
cp $GAMS_ROOT/lib/libGAMS.so $GAMSPLUGINS_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac

echo "Copying MADARA lib"
cp $MADARA_ROOT/lib/libMADARA.so $GAMSPLUGINS_ROOT/Plugins/MadaraLibrary/ThirdParty/madara/Mac

echo "Copying oscpack lib"
cp $OSC_ROOT/liboscpack.so.1.1.0 $GAMSPLUGINS_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac
cd $GAMSPLUGINS_ROOT/Plugins/GamsLibrary/ThirdParty/gams/Mac
mv liboscpack.so.1.1.0 liboscpack.so

popd
