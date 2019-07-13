// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4103)
#pragma warning(disable:4191)
#pragma warning(disable:4457)
#pragma warning(disable:4458)
#pragma warning(disable:4459)
#pragma warning(disable:4530)
#pragma warning(disable:4577)
#pragma warning(disable:4583)
#pragma warning(disable:4582)
#pragma warning(disable:4668)
#include "gams/controllers/Multicontroller.h"
#pragma warning(pop)

class FGamsAgentsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};

extern GAMSAGENTS_API madara::utility::Refcounter<gams::controllers::Multicontroller> gams_controller;
extern GAMSAGENTS_API madara::utility::Refcounter<madara::knowledge::KnowledgeBase> gams_kb;