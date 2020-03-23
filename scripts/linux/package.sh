SCRIPTS_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPTS_DIR")")"
PROJECT_FILE="$PROJECT_ROOT/GamsPlugins.uproject"
VERSION=`cat $PROJECT_ROOT/VERSION.txt`

echo ""
echo "Packaging GamsPlugins for optimized release"
echo "----------------------"
echo "UE4_ROOT=$UE4_ROOT"
echo "SCRIPTS_DIR=$SCRIPTS_DIR"
echo "PROJECT_ROOT=$PROJECT_ROOT"
echo "PROJECT_FILE=$PROJECT_FILE"
echo "----------------------"

mkdir -p "$PROJECT_ROOT/Binaries/Linux"

#"$UE4_ROOT/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun -project="$PROJECT_FILE" -noP4 -build -cook -stage -stagingdirectory="$PROJECT_ROOT/out" -pak

tar -czvf "$PROJECT_ROOT/out/GamsPlugins_Linux_v$VERSION.tar.gz" $PROJECT_ROOT/out/LinuxNoEditor/*

echo "Created release at $PROJECT_ROOT/out/GamsPlugins_Linux_v$VERSION.tar.gz"

echo Finished with script

