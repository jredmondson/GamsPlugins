// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsAgentManager.h"
#include "GamsGameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "MadaraIncludes.h"
#include "GamsAgentsLogs.h"
#include "MadaraUnrealUtility.h"
#include "GamsGameInstance.h"
#include "GamsAllVehicles.h"
#include "Math/UnrealMathUtility.h"

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
#include "madara/knowledge/ContextGuard.h"
#pragma warning(pop)

#define GAMS_PLATFORM_RANDOM_ALL          -1
#define GAMS_PLATFORM_RANDOM_QUAD         -2
#define GAMS_PLATFORM_QUAD_MIN             0
#define GAMS_PLATFORM_QUAD_MAX             2
#define GAMS_PLATFORM_JET_MIN              3
#define GAMS_PLATFORM_JET_MAX              3
#define GAMS_PLATFORM_RANDOM_JET          -3
#define GAMS_PLATFORM_RANDOM_BOAT         -4
#define GAMS_PLATFORM_RANDOM_SUBMERSIBLE  -5


AGamsAgentManager::AGamsAgentManager()
  : AActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

AGamsAgentManager::~AGamsAgentManager()
{
  managers_.Empty();
}

void AGamsAgentManager::BeginPlay()
{
  FActorSpawnParameters spawn_parameters;
  spawn_parameters.SpawnCollisionHandlingOverride =
    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  spawn_parameters.Owner = this;
  FTransform transform(
    FQuat(0.0f, 0.0f, 0.0f, 0.0f),
    FVector::ZeroVector);
  
  managers_.Add(GetWorld()->SpawnActor<AGamsArDrone>(
        AGamsArDrone::StaticClass(), transform, spawn_parameters));
  
  managers_.Add(GetWorld()->SpawnActor<AGamsDjiMavic>(
        AGamsDjiMavic::StaticClass(), transform, spawn_parameters));
  
  managers_.Add(GetWorld()->SpawnActor<AGamsDjiPhantom>(
        AGamsDjiPhantom::StaticClass(), transform, spawn_parameters));
  
  managers_.Add(GetWorld()->SpawnActor<AGamsF16>(
        AGamsF16::StaticClass(), transform, spawn_parameters));

  for (auto manager : managers_)
  {
    manager->init(gams_info_);
  }
}

void AGamsAgentManager::clear(void)
{
}

void AGamsAgentManager::destroy(uint32 id)
{
  // move removes to end so we maintain sorted instance order
  // if speed becomes an issue, we can ignore/remove much of this
  uint32 manager = agent_id_to_manager_[id];

  managers_[manager]->destroy(id);
}

void AGamsAgentManager::read(madara::knowledge::KnowledgeBase & kb)
{
  swarm_size_.set_name("swarm.size", kb);
  platform_type_str_ = gams_game_instance->platform_type;
  
  // convert the type to lowercase to reduce human error
  platform_type_str_ = platform_type_str_.ToLower();

  uint32 num_instances = (uint32)(*swarm_size_ > 0 ? *swarm_size_ : 100);

  // random_quad = -2
  // 
  int32 platform_index = GAMS_PLATFORM_RANDOM_QUAD;

  //actors_->InstanceCountToRender = (int32)num_instances;
  gams_info_.SetNum(num_instances);
  
  if (!platform_type_str_.IsEmpty())
  {
    if (platform_type_str_.Equals("ardrone"))
    {
      platform_index = 0;
    }
    else if (platform_type_str_.Equals("djimavic"))
    {
      platform_index = 1;
    }
    else if (platform_type_str_.Equals("djiphantom"))
    {
      platform_index = 2;
    }
    else if (platform_type_str_.Equals("f16"))
    {
      platform_index = 3;
    }
    else if (platform_type_str_.Equals("random_quad"))
    {
      platform_index = GAMS_PLATFORM_RANDOM_QUAD;
    }
    else if (platform_type_str_.Equals("random_all"))
    {
      platform_index = GAMS_PLATFORM_RANDOM_ALL;
    }
    else if (platform_type_str_.Equals("random_jet"))
    {
      platform_index = GAMS_PLATFORM_RANDOM_JET;
    }
  }

  UE_LOG(LogGamsAgentManager, Log,
    TEXT("num_instances=%d"),
    (int)num_instances);

  for (uint32 i = 0; i < num_instances; ++i)
  {
    uint32 manager = (uint32)platform_index;

    // initialize the agent variables
    gams_info_[i].init(i, kb);

    // write the position and orientation info from the kb
    gams_info_[i].write_to(gams_info_[i].transform);

    
    UE_LOG(LogGamsAgentManager, Log,
      TEXT("agent.%d: spawning at location=%s"),
      (int)i, *gams_info_[i].transform.GetLocation().ToString());

    if (platform_index < 0)
    {
      if (platform_index == GAMS_PLATFORM_RANDOM_JET)
      {
        const int32 min_manager(GAMS_PLATFORM_JET_MIN);
        const int32 max_manager(GAMS_PLATFORM_JET_MAX);
        manager = FMath::RandRange(min_manager, max_manager);
      }
      else if (platform_index == GAMS_PLATFORM_RANDOM_ALL)
      {
        // default is random any
        const int32 min_manager(0);
        const int32 max_manager(managers_.Num());
        manager = FMath::RandRange(min_manager, max_manager);
      }
      else // default (platform_index == GAMS_PLATFORM_RANDOM_QUAD)
      {
        const int32 min_manager(GAMS_PLATFORM_QUAD_MIN);
        const int32 max_manager(GAMS_PLATFORM_QUAD_MAX);
        manager = FMath::RandRange(min_manager, max_manager);
      }
    } // if platform_index < some kind of random platform
    
    gams_info_[i].platform_index = manager;
    managers_[manager]->spawn(i, gams_info_[i].transform);

    if (gams_game_instance->override_speed)
    {
      gams_info_[i].platform_speed = gams_game_instance->platform_speed;
    }
    else
    {
      gams_info_[i].platform_speed = managers_[manager]->max_speed;
    }
  }
}

void AGamsAgentManager::read_source_dest(
  madara::knowledge::KnowledgeBase & kb)
{
  madara::knowledge::ContextGuard guard(kb);

  for (auto instance : gams_info_)
  {
    instance.read_source_dest();
  }
}

void AGamsAgentManager::render(void)
{
  for (auto manager : managers_)
  {
    manager->render();
  }
}

uint32 AGamsAgentManager::size(void)
{
  return gams_info_.Num();
}

void AGamsAgentManager::update(float delta_time)
{
  for (int32 i = 0; i < managers_.Num(); ++i)
  {
    managers_[i]->update(delta_time);
  }
  render();
}

void AGamsAgentManager::write_location_orientation(
  madara::knowledge::KnowledgeBase & kb)
{
  madara::knowledge::ContextGuard guard(kb);

  for (auto instance : gams_info_)
  {
    instance.write_location_orientation();
  }
}
