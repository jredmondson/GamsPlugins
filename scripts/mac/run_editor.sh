#!/usr/bin/env bash
SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPTS_DIR")")"
PROJECT_FILE="$PROJECT_ROOT/GamsPlugins.uproject"

echo ""
echo "RUNNING GamsPlugins in EDITOR MODE"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "PROJECT_ROOT=$PROJECT_ROOT"
echo "PROJECT_FILE=$PROJECT_FILE"
echo "----------------------"

"$UE4_ROOT"/Engine/Binaries/Mac/UE4Editor "$PROJECT_FILE" $@

