#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPTS_DIR")")"
PROJECT_FILE="$PROJECT_ROOT/GamsPlugins.uproject"

echo ""
echo "GENERATING GamsPlugins project files"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "----------------------"
mkdir -p "$PROJECT_ROOT/Binaries/Mac"

pushd `pwd`

"$UE4_ROOT/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project="$PROJECT_FILE" -game -engine -makefiles

cd $PROJECT_DIR
sed -i .original -e "s/Linux/Mac/g" Makefile

popd
