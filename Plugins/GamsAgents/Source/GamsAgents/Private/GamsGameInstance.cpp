// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsGameInstance.h"
#include "GamsAgentsLogs.h"

void UGamsGameInstance::Init()
{
  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: entering"));

  //agent_factory_ = new UnrealAgentPlatformFactory ();

  // add the dynamic unreal agent platform factory
  std::vector <std::string> aliases;
  aliases.resize(3);
  aliases[0] = "default_agent";
  aliases[1] = "unreal_agent";
  aliases[2] = "dynamic_agent";

  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: adding aliases for agent factory"));

  gams::platforms::global_platform_factory()->add(aliases, &agent_factory_);

  // seed the current world
  gams_current_world = GetWorld();

  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: resizing controller to 100 agents"));

  // create 100 agents
  controller_.resize(100);

  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: creating args knowledge map"));

  // assign dynamic unreal platforms to the agents
  //madara::knowledge::KnowledgeMap args;
  //args["blueprint"] = "random";
  //args["blueprints.size"] = madara::knowledge::KnowledgeRecord::Integer(3);
  //args["blueprints.0"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_A.BP_Quadcopter_A";
  //args["blueprints.1"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_B.BP_Quadcopter_B";
  //args["blueprints.2"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_C.BP_Quadcopter_C";
  //args["location"] = "random";
  //args["orientation"] = "random";

  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: initializing unreal_agent platforms"));

  controller_.init_platform("unreal_agent");

  //UE_LOG (LogGamsGameInstance, Warning,
  //  TEXT ("UGamsGameInstance: Init: leaving"));
}

UWorld * gams_current_world (0);
float gams_delta_time (0.0);

void UGamsGameInstance::Shutdown ()
{
  //delete agent_factory_;
  //agent_factory_ = 0;
}
