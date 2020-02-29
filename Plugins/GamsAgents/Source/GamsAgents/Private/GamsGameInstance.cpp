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
#include "EngineUtils.h"
#include "GamsControllerThread.h"
#include <stdlib.h>
#include "UnrealAgentPlatform.h"
#include "GamsVehicle.h"
#include "MadaraUnrealUtility.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/String.h"
#include "madara/knowledge/containers/StringVector.h"

namespace knowledge = madara::knowledge;
namespace transport = madara::transport;
namespace threads = madara::threads;
namespace containers = knowledge::containers;

void UGamsGameInstance::Init()
{
  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: entering"));

  FString filename;
  FString filecontents;

  gams_game_instance = this;

  agent_factory_ = new UnrealAgentPlatformFactory();

  // add the dynamic unreal agent platform factory
  std::vector <std::string> aliases;
  aliases.resize(3);
  aliases[0] = "default_agent";
  aliases[1] = "unreal_agent";
  aliases[2] = "dynamic_agent";

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: adding aliases for agent factory"));

  gams::platforms::global_platform_factory()->add(aliases, agent_factory_);

  FString sim_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("sim_settings.mf"));

  FString transport_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("transport_settings.mf"));

  transport_settings.load_text(TCHAR_TO_UTF8(*transport_settings_file));

  FFileHelper::LoadFileToString(filecontents, *sim_settings_file);
  kb.evaluate(TCHAR_TO_UTF8(*filecontents));

  // log the transport settings to help with debugging
  FString debug_type = madara::transport::types_to_string(
    transport_settings.type).c_str();
  FString hosts;
  for (auto host : transport_settings.hosts)
  {
    hosts += ", ";
    hosts += host.c_str();
  }
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("Init: transport settings: type=%s, hosts=[%s]"),
    *debug_type, *hosts);

  kb.attach_transport("GamsPluginsGameInstance", transport_settings);

  // seed the current world
  gams_current_world = GetWorld();
  containers::Integer swarm_size ("swarm.size", kb);
  if (*swarm_size == 0)
  {
    // if no swarm.size is specified, initialize 100
    swarm_size = 100;
  }

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("Init: resizing controller to %d agents"),
    (int)*swarm_size);

  // create 100 agents
  controller.resize((size_t)*swarm_size);

  containers::StringVector karl_files("karl_files", kb);

  if (karl_files.size() > 0)
  {
    for (size_t i = 0; i < karl_files.size(); ++i)
    {
      std::string raw_string = karl_files[i];
      FString path(raw_string.c_str());
      if (madara::utility::begins_with(raw_string, "Scripts"))
      {
        // if the karl file begins with Scripts, then we need
        // to reference the Contents/Scripts directory
        filename = FPaths::Combine(FPaths::ProjectContentDir(),
          *path);
      }
      else
      {
        filename = path;
      }
      UE_LOG(LogGamsGameInstance, Log,
        TEXT("UGamsGameInstance: Init: reading karl init from file %s"),
        *filename);

      FFileHelper::LoadFileToString(filecontents, *filename);
      std::string contents = TCHAR_TO_UTF8(*filecontents);


      UE_LOG(LogGamsGameInstance, Log,
        TEXT("UGamsGameInstance: Init: evaluating %d byte karl logic on each platform"),
        (int32)filecontents.Len());

      controller.evaluate(contents);
    }
  }

  //FString filename = FPaths::Combine(FPaths::ProjectContentDir(),
  //  TEXT("Scripts"), TEXT("galois.mf"));
  //FString filecontents;
  //FFileHelper::LoadFileToString(filecontents, *filename);

  //std::string filename_with_env =
  //  "$(GAMS_ROOT)/scripts/simulation/unreal/move/line.mf";
  //std::string filename = madara::utility::expand_envs(filename_with_env);
  //FString filename_ue (filename.c_str());

  //UE_LOG(LogGamsGameInstance, Log,
  //  TEXT("UGamsGameInstance: Init: reading karl init from file %s"),
  //  *filename);

  //std::string contents = madara::utility::file_to_string(filename.s);

  //std::string contents = TCHAR_TO_UTF8(*filecontents);
  //controller.evaluate(contents);

  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGamsGameInstance::OnPostLoadMap);

  UnrealAgentPlatform::load_platform_classes();

  threader_.set_data_plane(kb);
  threader_.run(2.0f, "controller", new GamsControllerThread(controller), true);

#if UE_EDITOR
  OnPostLoadMap(gams_current_world);
#endif

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("UGamsGameInstance: Init: leaving"));

}

void UGamsGameInstance::OnPostLoadMap(UWorld* new_world)
{
  gams_current_world = new_world;

  threader_.pause("controller");

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

  controller.init_platform("unreal_agent", args);

  kb.send_modifieds();
  last_send_time_ = gams_current_world->UnpausedTimeSeconds;

  threader_.resume("controller");

  GetTimerManager().SetTimer(run_timer_handler_, this,
    &UGamsGameInstance::ControllerRun, gams_delta_time, true, 5.0f);
}

void UGamsGameInstance::Shutdown ()
{
  threader_.terminate();
  threader_.wait();
  UnrealAgentPlatform::unload_platform_classes();
  TimerManager->ClearTimer(run_timer_handler_);
  delete agent_factory_;
  agent_factory_ = 0;
}

void UGamsGameInstance::ControllerRun()
{
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("UGamsGameInstance: controllerrun: calling"));

  UWorld * temp_world = GetWorld();

  gams_delta_time = temp_world->DeltaTimeSeconds;

  if (gams_current_world != temp_world)
  {
    UE_LOG(LogGamsGameInstance, Log,
      TEXT("UGamsGameInstance: world has been changed"));

    gams_current_world = temp_world;
  }

  for (TActorIterator<AGamsVehicle> actor_it(gams_current_world);
      actor_it; ++actor_it)
  {
    AGamsVehicle* actor = *actor_it;
    FVector dest;
    FVector location = actor->GetActorLocation();
    FVector diff;
    FVector next;

    madara::utility::to_vector_multiply(actor->dest, dest);

    diff = dest - location;
    madara::utility::calculate_delta(diff, next,
      actor->max_speed, temp_world->DeltaTimeSeconds);
    next += location;

    UE_LOG(LogGamsGameInstance, Log,
      TEXT("UGamsGameInstance: loc=[%s], dest=[%s], diff=[%s], next=[%s]"),
      *location.ToString(), *dest.ToString(),
      *diff.ToString(), *next.ToString());

    actor->SetActorLocation(next, false, nullptr, ETeleportType::None);
  }

  if (temp_world->UnpausedTimeSeconds > last_send_time_ + 1.0f)
  {
    kb.send_modifieds();
    last_send_time_ = temp_world->UnpausedTimeSeconds;
  }
}

// global static variables
UWorld * gams_current_world(0);
float gams_delta_time(0.06);
UGamsGameInstance* gams_game_instance(0);