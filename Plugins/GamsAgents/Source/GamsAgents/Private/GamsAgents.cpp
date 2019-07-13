// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GamsAgents.h"

#define LOCTEXT_NAMESPACE "FGamsAgentsModule"

namespace controllers = gams::controllers;
namespace knowledge = madara::knowledge;
namespace utility = madara::utility;

void FGamsAgentsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGamsAgentsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
utility::Refcounter<knowledge::KnowledgeBase> gams_kb (
  new knowledge::KnowledgeBase ());

utility::Refcounter<controllers::Multicontroller> gams_controller (
  new controllers::Multicontroller ());

IMPLEMENT_MODULE(FGamsAgentsModule, GamsAgents)