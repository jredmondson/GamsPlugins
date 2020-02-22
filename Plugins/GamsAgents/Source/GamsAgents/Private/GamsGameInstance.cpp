// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsGameInstance.h"
#include "GamsAgentsLogs.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"

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

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: resizing controller to 100 agents"));

  // create 100 agents
  controller_.resize(250);

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: creating args knowledge map"));

  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGamsGameInstance::OnPostLoadMap);

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
  TimerManager->ClearTimer(run_timer_handler_);
}

void UGamsGameInstance::ControllerRun()
{
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: controller_run: calling"));

  UWorld * temp_world = GetWorld();

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
float gams_delta_time(1.0);
