# Overview

Importing these plugins into your project allows you to create
GAMS agents or use MADARA knowledge bases within Unreal projects.
More information on these AI middleware packages can be found on
the GAMS (http://gams.ai) and MADARA (http://madara.ai) open source
project sites.

---

# Modules

## GamsAgents

Provides access to agents and controllers that use GAMS and MADARA for
standard distributed algorithms within UE4. Allows for interfacing with
custom algorithms and platforms generated via the GAMS gpc.pl tool.

## GamsLibraryLibrary

Provides access to GAMS controllers, algorithms, and platforms for multi-agent
artificial intelligence. This module is necessary if you need to link to the
GAMS software library within Unreal.

## GamsLibrary

Provides access to simple header includes for GAMS that mask UE4 build issues

## MadaraLibraryLibrary

Provides access to MADARA knowledge bases, threads, the KaRL language, and other
MADARA primitives. This module is necessary if you need to link to the
MADARA software library within Unreal.

## MadaraLibrary

Provides access to simple header includes for MADARA that mask UE4 build issues

---

# Installation

---

## Windows

### For Users
  * Clone this repository to your computer (e.g., "git clone https://github.com/jredmondson/GamsPlugins.git")
  * Open the GamsPlugins.uproject file
  * Within UE4, migrate the Plugins directory contents to your new project
  * Modify your build.cs files to include the GamsLibrary, GamsLibraryLibrary, and MadaraLibraryLibrary modules

### For Repository Developers
  * Clone this repository to your computer (e.g., "git clone https://github.com/jredmondson/GamsPlugins.git")
  * Right click on GamsPlugins.uproject and select "Generate VS Studio project files"
  * Open GamsPlugins.sln and build project (Ctrl+Shift+B)

---

## Linux


### Install Unreal Engine 4.21
```
git clone -b 4.21 git@github.com:EpicGames/UnrealEngine.git
cd UnrealEngine
./Setup.sh
./GenerateProjectFiles.sh
make
export UE4_ROOT=`pwd`
```

Don't forget to set your UE4_ROOT to the UE 4.21 directory as above!

### Generate and Build Makefiles
```
git clone https://github.com/jredmondson/GamsPlugins.git
cd GamsPlugins
Scripts/Linux/generate.sh
Scripts/Linux/build.sh
```

### Run the project in Editor or Game mode
```
# to run in Editor mode do this:
Scripts/Linux/run_editor.sh

# or if you want to run in Game mode do this:
Scripts/Linux/run_game.sh
```

---

## Mac

Not currently supported.