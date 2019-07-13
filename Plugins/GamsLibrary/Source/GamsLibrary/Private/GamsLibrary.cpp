// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GamsLibrary.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FGamsLibraryModule"

void FGamsLibraryModule::StartupModule()
{
}

void FGamsLibraryModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGamsLibraryModule, GamsLibrary)
