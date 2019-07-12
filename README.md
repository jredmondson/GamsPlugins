# GamsPlugins
UE4 Plugins for GAMS middleware usage

# Installation

## Linux

```bash
$GAMS_ROOT/scripts/linux/base_build.sh madara gams prereqs
Then open the uproject/compile the plugin
```

## Windows

```bash
%GAMS_ROOT%\scripts\windows\base_build.sh forcecapnp forceboost forceosc madara gams
Open MADARA.sln and gams.sln in %MADARA_ROOT% and %GAMS_ROOT% and build solutions in that order
Right click on GamsPlugins.uproj and regenerate VS studio files
Open GamsPlugins.sln and compile
```
