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
#include "madara/knowledge/containers/Double.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/String.h"
#include "madara/knowledge/containers/StringVector.h" 	

#include "GenericPlatform/GenericPlatformProcess.h"

namespace knowledge = madara::knowledge;
namespace transport = madara::transport;
namespace threads = madara::threads;
namespace containers = knowledge::containers;

void UGamsGameInstance::Init()
{
  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("Init: entering"));

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
    TEXT ("Init: adding aliases for agent factory"));

  gams::platforms::global_platform_factory()->add(aliases, agent_factory_);

  FString sim_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("sim_settings.mf"));

  FString transport_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("transport_settings.mf"));

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("Init: loading transport settings from file %s."),
    *transport_settings_file);

  transport_settings.load_text(TCHAR_TO_UTF8(*transport_settings_file));

  if (FFileHelper::LoadFileToString(filecontents, *sim_settings_file))
  {
    UE_LOG(LogGamsGameInstance, Log,
      TEXT("Init: loading sim settings from file %s."),
      *sim_settings_file);
    kb.evaluate(TCHAR_TO_UTF8(*filecontents));
  }
  else
  {
    UE_LOG(LogGamsGameInstance, Warning,
      TEXT("Init: failed to load sim settings file. %s did not exist."),
      *sim_settings_file);
  }

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
      std::string raw_string (karl_files[i]);
      const std::string prefix("Scripts");
      FString path(raw_string.c_str());
      if (madara::utility::begins_with(raw_string, prefix))
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
        TEXT("Init: reading karl init from file %s"),
        *filename);

      //const std::string filename_str(TCHAR_TO_UTF8(*filename));
      //std::string contents = madara::utility::file_to_string(filename_str);

      //UE_LOG(LogGamsGameInstance, Log,
      //  TEXT("Init: evaluating %d byte karl logic on each platform"),
      //  (int32)contents.length());

      //controller.evaluate(contents);

      if (FFileHelper::LoadFileToString(filecontents, *filename))
      {
        //std::string contents = TCHAR_TO_UTF8(*filecontents);

        UE_LOG(LogGamsGameInstance, Log,
          TEXT("Init: evaluating %d byte karl logic on each platform"),
          (int32)filecontents.Len());

        controller.evaluate(TCHAR_TO_UTF8(*filecontents));
      }
      //else
      //{
      //  UE_LOG(LogGamsGameInstance, Warning,
      //    TEXT("Init: failed to load file. File %s did not exist."),
      //    (int32)filecontents.Len());
      //}
    }
  }

  // setup a delegate for changing maps (happens automatically on game start)
  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
    this, &UGamsGameInstance::OnPostLoadMap);

  UnrealAgentPlatform::load_platform_classes();

  // run GAMS multicontroller at 2hz, unless user overrides with controller.hz
  double controller_hz = 2.0f;

  if (kb.exists("controller.hz"))
  {
    // because of STL funkiness in UE4, we try to be safe with memory
    containers::Double temp_hz("controller.hz", kb);
    controller_hz = *temp_hz;
  }

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("Init: starting GAMS controller at %f hz"), controller_hz);

  threader_.set_data_plane(kb);
  threader_.run(controller_hz, "controller",
    new GamsControllerThread(controller), true);

  // editor requires explicit call to change the world
#if UE_EDITOR
  OnPostLoadMap(gams_current_world);
#endif

  UE_LOG (LogGamsGameInstance, Log,
    TEXT ("Init: leaving"));

  // Try to sleep so UE4 cleans up its memory issues
  FPlatformProcess::Sleep(3.0f);
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

  std::string platform_prefix("platform");

  // assign dynamic unreal platforms to the agents
  //madara::knowledge::KnowledgeMap args = kb.to_map_stripped(platform_prefix);
  //args["blueprint"] = "random";
  //args["blueprints.size"] = madara::knowledge::KnowledgeRecord::Integer(3);
  //args["blueprints.0"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_A.BP_Quadcopter_A";
  //args["blueprints.1"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_B.BP_Quadcopter_B";
  //args["blueprints.2"] = "/Game/Quadcopters/Blueprints/BP_Quadcopter_C.BP_Quadcopter_C";
  //args["location"] = "random";
  //args["orientation"] = "random";

  //controller.init_platform("unreal_agent", kb.to_map_stripped(platform_prefix));
  controller.init_platform("unreal_agent");

  //madara::knowledge::safe_clear(args);

  kb.send_modifieds();
  last_send_time_ = gams_current_world->UnpausedTimeSeconds;

  threader_.resume("controller");

  // defaults are 60hz game loop and a 5 second delay.
  float delta_time = 0.06f;
  float delay = 5.0f;

  // allow overrides for game hertz and delay
  if (kb.exists("game.hz"))
  {
    // because of STL funkiness in UE4, we try to be safe with memory
    containers::Double temp_hz("game.hz", kb);
    delta_time = (float)(*temp_hz / 1000.0f);
  }

  if (kb.exists("game.delay"))
  {
    // because of STL funkiness in UE4, we try to be safe with memory
    containers::Double temp_delay("game.delay", kb);
    delay = (float)(*temp_delay);
  }

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("Init: starting game loop with delta_time=%f after %fs"),
    delta_time, delay);

  GetTimerManager().SetTimer(run_timer_handler_, this,
    &UGamsGameInstance::GameRun, delta_time, true, delay);
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

void UGamsGameInstance::GameRun()
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
    actor->animate(temp_world->DeltaTimeSeconds);
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