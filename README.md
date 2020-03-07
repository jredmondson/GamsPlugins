# Overview

Importing these plugins into your project allows you to create
GAMS agents or use MADARA knowledge bases within Unreal projects.
More information on these AI middleware packages can be found on
the GAMS (http://gams.ai) and MADARA (http://madara.ai) open source
project sites.

* [Wiki](https://github.com/jredmondson/GamsPlugins/wiki) | [Installation](https://github.com/jredmondson/GamsPlugins/wiki/Installation) | [Running](https://github.com/jredmondson/GamsPlugins/wiki/RunningSimulations)

---

# Table of Contents

  * [Modules](#modules)
    * [GamsAgents](#gamsagents)
    * [GamsLibraryLibrary](#gamslibrarylibrary)
    * [GamsLibrary](#gamslibrary)
    * [MadaraLibraryLibrary](#madaralibrarylibrary)
    * [MadaraLibrary](#madaralibrary)
  * [Installation](#installation)
    * [Windows](#windows)
      * [Install Unreal Engine 4.24 from Launcher](#install-unreal-engine-424-from-launcher)
      * [Build Unreal Engine 4.24](#build-unreal-engine-424)
      * [For Users](#for-users)
      * [For Repository Developers](#for-repository-developers)
    * [Linux Users](#linux-users)
      * [Install Unreal Engine 4.24](#install-unreal-engine-424)
      * [Generate and Build Makefiles](#generate-and-build-makefiles)
      * [Run the project in Editor or Game mode](#run-the-project-in-editor-or-game-mode)
    * [Linux Developers](#linux-developers)
      * [Updating MADARA and GAMS](#updating-madara-and-gams)
    * [Mac](#mac)
   
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

### Install Unreal Engine 4.24 from Launcher
  * From your Epic Games launcher, download the Unreal Engine 4.24
  
### Build Unreal Engine 4.24

UE requires VS 2017 at the moment. It does not support VS 2019+.

```
git clone -b 4.24 git@github.com:EpicGames/UnrealEngine.git
cd UnrealEngine
.\Setup.bat
.\GenerateProjectFiles.bat
UE.sln (this should open in VS 2017)

```

### For Users
  * Install Unreal Engine 4.24 from the Launcher
  * Download Visual Studio 2017 or Visual Studio 2019 and Add .NET and C++ support. The UE build system requires this.
  * Compile and install the GamsPlugin via the following on your command line (prefer VS 2017 or VS 2019 command line through the Start Menu)
```
git clone https://github.com/jredmondson/GamsPlugins.git
cd GamsPlugins
Scripts/generate.bat
Scripts/build.bat
GamsPlugins.uproject
```
  * Once the editor loads, allow the compile shaders to finish and then press play

### For Repository Developers
  * Download git for Windows (https://git-scm.com/download/win or https://gitforwindows.org/)
  * Download cmake for Windows (https://cmake.org/download/)
  * Download perl for Windows (http://strawberryperl.com/). Enable options that allow using `perl` from command line (i.e., add to PATH).
  * Download and install GAMS and MADARA via the following
```
mkdir \code
cd \code
git clone -b windows_fixes https://github.com/jredmondson/gams
git clone -b windows_fixes https://github.com/jredmondson/madara
gams\scripts\windows\base_build.bat vs2017 forceboost forceosc forcecapnp madara gams
:: note that Windows scripting is a bit rough. Parts of the base_build.bat may fail.
:: check the boost, osc, madara, and gams directories for libraries (should be .lib or .dll)
:: if these libs have been built, you can remove the keywords to run fewer build tasks.
:: for instance, if you have built boost, OSC, and capnp and just want to regenerate the
:: solution files for MADARA and GAMS, you can run:
:: gams\scripts\windows\base_build.bat vs2017 madara gams
```
  * Set your environment variables to the contents of the file %USERPROFILE%\\.gams\env.bat
```
type %USERPROFILE%\.gams\env.bat
:: note that the above commands only displays the contents of the file.
:: If you want to apply the local variables, you can either run the above
:: script, which will only set them in your current terminal window, or you
:: can set each one of these through your Start Menu by searching for
:: Set Environment Variables and creating each one individually. The latter
:: is more recommended so you will have everything linking long term
```
  * Clone this repository to your computer (e.g., "git clone https://github.com/jredmondson/GamsPlugins.git")
  * Right click on GamsPlugins.uproject and select "Generate VS Studio project files"
  * Open GamsPlugins.sln and build project (Ctrl+Shift+B)

---

## Linux Users


### Install Unreal Engine 4.24
```
git clone -b 4.24 git@github.com:EpicGames/UnrealEngine.git
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

## Linux Developers

Following the instructions above in Linux Users will get you everything you need to use the GamsPlugins. However, if you're wanting to help with updating the Linux libraries for the GamsPlugins, please use the following process.

### Updating MADARA and GAMS

As of 4.24, we have to compile the MADARA and GAMS libraries with clang-8. Consequently, the following should build the libraries you need and copy them to your GamsPlugins directories.

```
mkdir projects
cd projects
git clone -b windows_fixes https://github.com/jredmondson/gams.git
git clone -b windows_fixes https://github.com/jredmondson/madara.git
git clone https://github.com/jredmondson/GamsPlugins.git
gams/scripts/linux/base_build.sh prereqs gams madara nocapnp clang-8
source ~/.gams/env.sh
GamsPlugins/Scripts/Linux/copy_libs.sh
```

---

## Mac

Not currently supported.
