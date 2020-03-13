#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo ""
echo "RUNNING GamsPlugins in GAME MODE"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "----------------------"
"$UE4_ROOT"/Engine/Binaries/Mac/UE4Editor "$SCRIPTS_DIR/../../GamsPlugins.uproject" -windowed -game -ResX=1024 -ResY=576 $@
