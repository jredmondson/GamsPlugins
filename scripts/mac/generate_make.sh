#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo ""
echo "GENERATING GamsPlugins project files"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "----------------------"
mkdir -p "$SCRIPTS_DIR/../../Binaries/Mac"
export PROJECT_DIR=$SCRIPTS_DIR/../..

pushd `pwd`

"$UE4_ROOT/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project="$PROJECT_DIR/GamsPlugins.uproject" -game -engine -makefiles

cd $PROJECT_DIR
sed -i .original -e "s/Linux/Mac/g" Makefile

popd
