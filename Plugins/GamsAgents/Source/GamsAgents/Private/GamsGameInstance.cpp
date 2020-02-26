// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsGameInstance.h"
#include "GamsAgentsLogs.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"
#include "madara/utility/Utility.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Engine/World.h"
#include <stdlib.h>

void UGamsGameInstance::Init()
{
  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: entering"));

  // add the dynamic unreal agent platform factory
  std::vector <std::string> aliases;
  aliases.resize(3);
  aliases[0] = "default_agent";
  aliases[1] = "unreal_agent";
  aliases[2] = "dynamic_agent";

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: adding aliases for agent factory"));

  gams::platforms::global_platform_factory()->add(aliases, &agent_factory_);

  // seed the current world
  gams_current_world = GetWorld();
  size_t num_controllers = 100;

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: resizing controller to %d agents"),
    num_controllers);

  // create 100 agents
  controller_.resize(num_controllers);

  FString filename = FPaths::Combine(FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("line.mf"));
  FString filecontents;
  FFileHelper::LoadFileToString(filecontents, *filename);

  //std::string filename_with_env =
  //  "$(GAMS_ROOT)/scripts/simulation/unreal/move/line.mf";
  //std::string filename = madara::utility::expand_envs(filename_with_env);
  //FString filename_ue (filename.c_str());

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: Init: reading karl init from file %s"),
    *filename);

  //std::string contents = madara::utility::file_to_string(filename.s);

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: Init: evaluating %d byte karl logic on each platform"),
      (int32)filecontents.Len());

  std::string contents = TCHAR_TO_UTF8(*filecontents);
  controller_.evaluate(contents);

  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGamsGameInstance::OnPostLoadMap);

  UnrealAgentPlatform::load_platform_classes();

#if UE_EDITOR
  OnPostLoadMap(gams_current_world);
#endif

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: leaving"));
}

void UGamsGameInstance::OnPostLoadMap(UWorld* new_world)
{
  gams_current_world = new_world;

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: post_level_load: "
      "initializing unreal_agent platforms"));

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: post_level_load: creating args knowledge map"));

  // assign dynamic unreal platforms to the agents
  madara::knowledge::KnowledgeMap args;
  args["blueprint"] = "random";
  //args["blueprints.size"] = madara::knowledge::KnowledgeRecord::Integer(3);
  //args["blueprints.0"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_A.BP_Quadcopter_A";
  //args["blueprints.1"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_B.BP_Quadcopter_B";
  //args["blueprints.2"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_C.BP_Quadcopter_C";
  args["location"] = "random";
  args["orientation"] = "random";

  controller_.init_platform("unreal_agent", args);

  GetTimerManager().SetTimer(run_timer_handler_, this,
    &UGamsGameInstance::ControllerRun, gams_delta_time, true, 5.0f);
}

void UGamsGameInstance::Shutdown ()
{
  UnrealAgentPlatform::unload_platform_classes();
  TimerManager->ClearTimer(run_timer_handler_);
}

void UGamsGameInstance::ControllerRun()
{
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: controller_run: calling"));

  UWorld * temp_world = GetWorld();

  gams_delta_time = temp_world->DeltaTimeSeconds;

  if (gams_current_world != temp_world)
  {
    UE_LOG(LogGamsGameInstance, Log,
      TEXT("UGamsGameInstance: world has been changed"));

    gams_current_world = temp_world;
  }

  controller_.run_once();
}

// global static variables
UWorld * gams_current_world(0);
float gams_delta_time(0.1);
