// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsGameInstance.h"
#include "GamsAgentsLogs.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MoviePlayer.h"
#include "EngineUtils.h"
#include "GamsControllerThread.h"
#include "GamsControllerEvaluateThread.h"
#include <stdlib.h>
#include "UnrealAgentPlatform.h"
#include "GamsAgentManager.h"
#include "GamsVehicle.h"
#include "MadaraUnrealUtility.h"
#include <algorithm>
#include <stdio.h>

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
#pragma warning(disable:4996)
#include "madara/utility/Utility.h"
#include "madara/knowledge/containers/Double.h"
#include "madara/knowledge/containers/Integer.h"
#include "madara/knowledge/containers/String.h"
#include "madara/knowledge/containers/Vector.h"
#include "madara/logger/GlobalLogger.h"
#include "gams/loggers/GlobalLogger.h"
#pragma warning(pop)

#include "GenericPlatform/GenericPlatformProcess.h"

namespace knowledge = madara::knowledge;
namespace transport = madara::transport;
namespace threads = madara::threads;
namespace containers = knowledge::containers;

void UGamsGameInstance::Init()
{
  Super::Init();
  
  // set the threader data plane to the game knowledge base
  threader_.set_data_plane(kb);

  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: entering"));
  
  size_t buf_size(0);
  char buf[128];

  FString filename;

  gams_game_instance = this;

  // set default GAMS and MADARA logging to file only
  FString log_location = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
  
  FString gams_log_location = log_location + TEXT("/gams_log.txt");
  FString madara_log_location = log_location + TEXT("/madara_log.txt");
  FString transport_settings_save = log_location + TEXT("/actual_transport_settings.mf");
  
  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: deleting old MADARA|GAMS log at madara|gams_log.txt"));
  
  remove(TCHAR_TO_UTF8(*gams_log_location));
  remove(TCHAR_TO_UTF8(*madara_log_location));

  gams::loggers::global_logger->clear();
  gams::loggers::global_logger->add_file(TCHAR_TO_UTF8(*gams_log_location));
  gams::loggers::global_logger->set_level(madara::logger::LOG_WARNING);
  
  madara::logger::global_logger.get()->set_timestamp_format(
    "%x %X: ");
  
  gams::loggers::global_logger.get()->set_timestamp_format(
    "%x %X: ");

  madara::logger::global_logger->clear();
  madara::logger::global_logger->add_file(TCHAR_TO_UTF8(*madara_log_location));
  madara::logger::global_logger->set_level(madara::logger::LOG_WARNING);
  
  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: logging MADARA|GAMS to madara|gams_log.txt"));
  
  agent_factory_ = new UnrealAgentPlatformFactory();

  // add the dynamic unreal agent platform factory
  std::vector <std::string> aliases;
  aliases.resize(3);
  aliases[0] = "default_agent";
  aliases[1] = "unreal_agent";
  aliases[2] = "dynamic_agent";

  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: adding aliases for agent factory"));

  gams::platforms::global_platform_factory()->add(aliases, agent_factory_);
  
  // read the transports settings from the file

  FString transport_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("transport_settings.mf"));

  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init: loading transport settings from file %s."),
    *transport_settings_file);

  transport_settings.load_text(TCHAR_TO_UTF8(*transport_settings_file));
  transport_settings.save_text(TCHAR_TO_UTF8(*transport_settings_save));

  filecontents_.SetNum(1);

  // read the sim settings into the knowledge base

  FString sim_settings_file = FPaths::Combine(
    FPaths::ProjectContentDir(),
    TEXT("Scripts"), TEXT("sim_settings.mf"));

  if (FFileHelper::LoadFileToString(filecontents_[0], *sim_settings_file))
  {
    UE_LOG(LogGamsGameInstanceInit, Log,
      TEXT("Init: loading sim settings from file %s."),
      *sim_settings_file);
    kb.evaluate(TCHAR_TO_UTF8(*filecontents_[0]));
  }
  else
  {
    UE_LOG(LogGamsGameInstanceInit, Warning,
      TEXT("Init: failed to load sim settings file. %s did not exist."),
      *sim_settings_file);
  }
  
  // check for MADARA log level change
  if (kb.exists("madara.log.level"))
  {
    int level = kb.get("madara.log.level").to_integer();
    madara::logger::global_logger->set_level(level);

    UE_LOG(LogGamsGameInstanceInit, Log,
        TEXT("Init: setting MADARA log level to %d"), level);
  }
  
  // check for GAMS log level change
  if (kb.exists("gams.log.level"))
  {
    int level = kb.get("gams.log.level").to_integer();
    gams::loggers::global_logger->set_level(level);

    UE_LOG(LogGamsGameInstanceInit, Log,
        TEXT("Init: setting GAMS log level to %d"), level);
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
  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init: transport settings: type=%s, hosts=[%s]"),
    *debug_type, *hosts);

  // attach the transport to the main game knowledge base

  kb.attach_transport("GamsPluginsGameInstance", transport_settings);

  // set agents loaded to 0. could be used in a loading screen

  agents_loaded.set_name("swarm.loaded", kb);
  agents_loaded = 0;


  // seed the current world
  gams_current_world = GetWorld();

  // obtain swarm size and platform animation setting
  swarm_size.set_name("swarm.size", kb);
  containers::Integer platform_animate("platform.animate", kb);

  if (*swarm_size <= 0)
  {
    // if no swarm.size is specified, initialize 100
    swarm_size = 100;
  }

  hive.resize((size_t)*swarm_size);

  // note that the default behavior is to not animate platforms
  should_animate = platform_animate.is_true();
  
  // set the platform type for agent platform creation

  madara::utility::to_c_str(kb.get("platform.type"), (char*)buf, 128);

  platform_type = buf;
  agent_factory_->platform_type = buf;

  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: resizing controller to %d agents"),
    (int)*swarm_size);
  
  size_t num_controllers = 1;

  // check for num_controllers > 1 to use more CPU cores
  if (kb.exists("controller.threads"))
  {
    num_controllers = kb.get("controller.threads").to_integer();

    if (num_controllers <= 0)
    {
      num_controllers = 1;
    }
  }
  
  UE_LOG(LogGamsGameInstanceInit, Log,
      TEXT("Init: setting num controllers (controller.threads) to %d"),
      (int)num_controllers);
  
  controllers.SetNum((int32)num_controllers);

  size_t agents = (size_t)*swarm_size;
  size_t agents_per_controller = agents / num_controllers;
  size_t extra_agents = agents % num_controllers;

  for (int32 i = 0; i < num_controllers; ++i)
  {
    controllers[i].set_hive(hive);

    if (i != num_controllers - 1)
    {
      UE_LOG(LogGamsGameInstanceInit, Log,
          TEXT("Init: resizing controller[%d] to %d agents"),
          (int)i, (int)agents_per_controller);
  
      controllers[i].resize(
        i * agents_per_controller, agents_per_controller);
    }
    else
    {
      UE_LOG(LogGamsGameInstanceInit, Log,
          TEXT("Init: resizing controller[%d] to %d agents"),
          (int)i, (int)(agents_per_controller + extra_agents));
  
      controllers[i].resize(
        i * agents_per_controller, agents_per_controller + extra_agents);
    }
    // create swarm.size agent controllers
  }

  // read the karl files that should be evaluated on each agent controller
  containers::Vector karl_files("karl_files", kb);
  filecontents_.SetNum(karl_files.size());

  if (karl_files.size() > 0)
  {
    for (size_t i = 0; i < karl_files.size(); ++i)
    {
      madara::utility::to_c_str(karl_files[i], (char*)buf, 128);

      filename = madara::utility::create_path("Scripts", buf);

      UE_LOG(LogGamsGameInstanceInit, Log,
        TEXT("Init: reading karl init from file %s"),
        *filename);

      if (FFileHelper::LoadFileToString(filecontents_[i], *filename))
      {
        UE_LOG(LogGamsGameInstanceInit, Log,
          TEXT("Init: loaded %d byte karl logic to run on each platform"),
          (int32)filecontents_[i].Len());
      }
    }
  }
  
  for (int i = 0; i < (int)num_controllers; ++i)
  {
    FString name = "controller.";
    name += FString::FromInt(i);
    threader_.run(TCHAR_TO_UTF8(*name),
      new GamsControllerEvaluateThread(controllers[i], filecontents_));
  }

  // check if platform speed needs to be overridden
  if (kb.exists("platform.max_speed"))
  {
    // GAMS speed is in m/s. Convert it to cm/s
    override_speed = true;
    platform_speed = kb.get("platform.max_speed").to_double() * 100;
    
    UE_LOG(LogGamsGameInstanceInit, Log,
      TEXT("Init: platform speed overridden. speed=%f"), *FString(buf));
  }

  // check if level variables exists and if so, load the level
  if (kb.exists("level"))
  {
    madara::utility::to_c_str(kb.get("level"), (char*)buf, 128);

    FString current_level = GetWorld()->GetMapName();
    FName level_name(*current_level);
    if (level_name != FName(buf))
    {
      UE_LOG(LogGamsGameInstanceInit, Log,
        TEXT("Init: opening map %s"), *FString(buf));

      UGameplayStatics::OpenLevel(this, FName(buf), true);
    }
  }
  else
  {
    // editor requires explicit call to change the world
#if UE_EDITOR
    OnPostLoadMap(gams_current_world);
#endif
  }

  // setup delegates for changing maps (happens automatically on game start)

  FCoreUObjectDelegates::PreLoadMap.AddUObject(
    this, &UGamsGameInstance::OnPreLoadMap);
  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
    this, &UGamsGameInstance::OnPostLoadMap);

  // run GAMS multicontroller at 2hz, unless user overrides with controller.hz
  controller_hz = 2.0f;

  // check if collisions should be enabled
  enable_collisions = kb.get("platform.collisions").is_true();
  collision_type = enable_collisions ?
    ECollisionEnabled::PhysicsOnly : ECollisionEnabled::NoCollision;
  
  // check the hertz rate of the agent controllers

  if (kb.exists("controller.hz"))
  {
    containers::Double temp_hz("controller.hz", kb);
    controller_hz = *temp_hz;
  }
  
  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init: waiting for controller evaluations to finish"), controller_hz);

  threader_.terminate();
  threader_.wait();

  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init: starting GAMS controller at %f hz"), controller_hz);

  for (int i = 0; i < (int)num_controllers; ++i)
  {
    FString name = "controller.";
    name += FString::FromInt(i);
    threader_.run(controller_hz, TCHAR_TO_UTF8(*name),
      new GamsControllerThread(controllers[i]), false);
  }

  UE_LOG (LogGamsGameInstanceInit, Log,
    TEXT ("Init: leaving"));
}

float UGamsGameInstance::LoadingPercentage() const
{
  float result = (float)*agents_loaded;

  if (*swarm_size > 0)
  {
    result /= (float)*swarm_size;
  }
  else
  {
    result = 0;
  }

  return result;
}

void UGamsGameInstance::OnPreLoadMap(const FString& map_name)
{
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "tearing down past level components. Resuming threads."));

  //threader_.pause("controller");
  //threader_.wait_for_paused("controller");

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "terminating threads"));

  threader_.terminate();
  
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "waiting for threads to terminate"));

  threader_.wait();
  
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "clearing hive"));

  hive.clear();
  
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "launching new restore_vars, eval, and init_platform threads"));

  for (int i = 0; i < (int)controllers.Num(); ++i)
  {
    FString name = "controller.";
    name += FString::FromInt(i);
    threader_.run(TCHAR_TO_UTF8(*name),
      new GamsControllerEvaluateThread(controllers[i], filecontents_));
  }

  agents_loaded = 0;
  
  UE_LOG(LogGamsGameInstance, Log,
    TEXT("pre_level_load: "
      "building load screen"));

  if (!GetTimerManager().IsTimerPaused(run_timer_handler_))
  {
    GetTimerManager().PauseTimer(run_timer_handler_);
  }

  if (!IsRunningDedicatedServer())
  {
    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
    LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    //WidgetBlueprint'/Game/Blueprints/LoadingScreen.LoadingScreen'

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
  }
}

void UGamsGameInstance::OnPostLoadMap(UWorld* new_world)
{
  gams_current_world = new_world;

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("post_level_load: "
      "initializing unreal_agent platforms"));

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("post_level_load: creating args knowledge map"));


  // UE4 breaks STL like this. We could make a workaround if args to
  // init_platform become necessary
  // std::string prefix("platform.");
  // madara::knowledge::KnowledgeMap args (kb.to_map_stripped(prefix));
  // args["blueprint"] = "random";
  // args["location"] = "random";
  // args["orientation"] = "random";

  // spawn the Agent Manager (actor managing HISMs of the platforms)

  FActorSpawnParameters spawn_parameters;
  spawn_parameters.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  FTransform transform(
    FQuat(0.0f, 0.0f, 0.0f, 0.0f),
    FVector::ZeroVector);

  manager_ = gams_current_world->SpawnActor<AGamsAgentManager>(
        AGamsAgentManager::StaticClass(), transform, spawn_parameters);

  // read platform settings from the KB to initialize KB bindings
  // note that if we want to remove kb blocking semantics, this is
  // where the variables are setup to the kb where context lock occurs

  manager_->read(kb);

  kb.send_modifieds();
  last_send_time_ = gams_current_world->UnpausedTimeSeconds;
  last_kb_read_time_ = 0;
  last_kb_write_time_ = 0;
  
  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init: waiting for controller evaluations to finish"), controller_hz);
  
  threader_.terminate();
  threader_.wait();
  
  UE_LOG(LogGamsGameInstanceInit, Log,
    TEXT("Init:"), controller_hz);
  
  for (int i = 0; i < (int)controllers.Num(); ++i)
  {
    FString name = "controller.";
    name += FString::FromInt(i);
    threader_.run(controller_hz, TCHAR_TO_UTF8(*name),
      new GamsControllerThread(controllers[i]), false);
  }

  // game loop delta and the delay before starting game thread
  float delta_time = gams_delta_time;
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

  gams_delta_time = delta_time;

  UE_LOG(LogGamsGameInstance, Log,
    TEXT("Init: starting game loop with delta_time=%f after %fs"),
    delta_time, delay);

  GetTimerManager().SetTimer(run_timer_handler_, this,
    &UGamsGameInstance::GameRun, delta_time, true, delay);

  if (!GetTimerManager().IsTimerPaused(run_timer_handler_))
  {
    GetTimerManager().UnPauseTimer(run_timer_handler_);
  }

}

void UGamsGameInstance::Shutdown ()
{
  Super::Shutdown();

  threader_.terminate();
  threader_.wait();

  UnrealAgentPlatform::unload_platform_classes();
  TimerManager->ClearTimer(run_timer_handler_);
  delete agent_factory_;
  agent_factory_ = 0;
  manager_->clear();
}

void UGamsGameInstance::GameRun()
{
  UE_LOG(LogGamsGameInstanceRun, Log,
    TEXT("controllerrun: calling"));

  UWorld * temp_world = GetWorld();

 // gams_delta_time = temp_world->DeltaTimeSeconds;

  if (gams_current_world != temp_world)
  {
    UE_LOG(LogGamsGameInstanceRun, Log,
      TEXT("world has been changed"));

    gams_current_world = temp_world;
  }

  // check to see if it's time to populate our source/dest info from KB
  if (temp_world->UnpausedTimeSeconds >
      last_kb_read_time_ + expected_kb_read_period_)
  {
    UE_LOG(LogGamsGameInstanceRun, Log,
      TEXT("controllerrun: reading source/dest from KB at time %fs"),
        temp_world->UnpausedTimeSeconds);

    manager_->read_source_dest(kb);
    last_kb_read_time_ = temp_world->UnpausedTimeSeconds;
  }

  // this is essentially the agent inner loop(s)
  manager_->update(gams_delta_time);
  
  if (temp_world->UnpausedTimeSeconds >
      last_kb_write_time_ + expected_kb_write_period_)
  {
    UE_LOG(LogGamsGameInstanceRun, Log,
      TEXT("controllerrun: writing loc/orient to KB at time %fs"),
        temp_world->UnpausedTimeSeconds);

    manager_->write_location_orientation(kb);
    last_kb_write_time_ = temp_world->UnpausedTimeSeconds;
  }

  // send data from the kb at 1hz to any listeners
  if (temp_world->UnpausedTimeSeconds > last_send_time_ + 1.0f)
  {
    UE_LOG(LogGamsGameInstanceRun, Log,
      TEXT("controllerrun: sending modifieds at time %fs"),
        temp_world->UnpausedTimeSeconds);

    kb.send_modifieds();
    last_send_time_ = temp_world->UnpausedTimeSeconds;
  }
}

// global static variables
UWorld * gams_current_world(0);
float gams_delta_time(0.06);
UGamsGameInstance* gams_game_instance(0);