#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
echo "Generating GamsPlugins project files"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "----------------------"
mkdir -p "$SCRIPTS_DIR/../../Binaries/Linux"
"$UE4_ROOT"/GenerateProjectFiles.sh -project="$SCRIPTS_DIR/../../GamsPlugins.uproject" -game -engine -Makefile -vscode
