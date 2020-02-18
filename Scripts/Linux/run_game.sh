#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo "Running GamsPlugins in Editor Mode"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "----------------------"
"$UE4_ROOT"/Engine/Binaries/Linux/UE4Editor "$SCRIPTS_DIR/../../GamsPlugins.uproject" -windowed -game -ResX=1024 -ResY=576 $@
