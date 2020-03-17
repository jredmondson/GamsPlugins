// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsAgentManager.h"
#include "GamsGameInstance.h"
#include "Engine/StaticMesh.h"
#include "MadaraIncludes.h"
#include "GamsAgentsLogs.h"
#include "MadaraUnrealUtility.h"
#include "GamsGameInstance.h"

AGamsAgentManager::AGamsAgentManager()
  : AActor()
{
  actors_ = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors")); 
  UStaticMesh* root_mesh = 
    Cast<UStaticMesh>(StaticLoadObject(
      UStaticMesh::StaticClass(), NULL, TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Main")));

  //actors_->RegisterComponent(); 
  actors_->SetStaticMesh(root_mesh); 
  actors_->SetFlags(RF_Transactional); 
  //actors_->bDisableCollision = !gams_game_instance->enable_collisions;
  
  //actors_->SetGenerateOverlapEvents(false);
  if (gams_game_instance != nullptr)
  {
    actors_->SetCollisionEnabled(gams_game_instance->collision_type);
  }
  else
  {
    actors_->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }
  this->AddInstanceComponent(actors_);
}

AGamsAgentManager::~AGamsAgentManager()
{
  clear();
}

void AGamsAgentManager::read(madara::knowledge::KnowledgeBase & kb)
{
  swarm_size_.set_name("swarm.size", kb);

  uint32 num_instances = (uint32)(*swarm_size_ > 0 ? *swarm_size_ : 100);

  gams_info_.SetNum(num_instances);
  transforms_.SetNum(num_instances);

  UE_LOG(LogGamsAgentManager, Log,
    TEXT("num_instances=%d"),
    (int)num_instances);

  for (uint32 i = 0; i < num_instances; ++i)
  {
    // initialize the agent variables
    gams_info_[i].init(i, kb);

    // write the position and orientation info from the kb
    gams_info_[i].write_to(transforms_[i]);

    
    UE_LOG(LogGamsAgentManager, Log,
      TEXT("agent.%d: spawning at location=%s"),
      (int)i, *transforms_[i].GetLocation().ToString());


    spawn(i, transforms_[i]);
  }
}

void AGamsAgentManager::clear(void)
{
  if (actors_ != nullptr)
  {
    //actors_->ClearInstances();
    //agent_id_to_instance_.Empty();
    //gams_info_.Empty();
    //instances_.Empty();
    //transforms_.Empty();
  }
}

void AGamsAgentManager::update(float delta_time)
{
  FVector dest;
  FVector diff;
  FVector next;
  FTransform transform(FRotator::ZeroRotator);

  for (int32 i = 0; i < gams_info_.Num(); ++i)
  {
    madara::utility::to_vector_multiply(gams_info_[i].dest, dest);

    diff = dest - transforms_[i].GetLocation();
    madara::utility::calculate_delta(diff, next,
      500.0f, delta_time);
    next += transforms_[i].GetLocation();
    
    UE_LOG(LogGamsAgentManager, Log,
      TEXT("agent.%d::update: setting next=%s from location=%s"),
      (int)i, *next.ToString(),
      *transforms_[i].GetLocation().ToString());

    transform.SetLocation(next);
    transforms_[i] = transform;
    madara::utility::from_vector_multiply(next, gams_info_[i].location);
    gams_info_[i].orientation.set(2, 0.0f);
    gams_info_[i].orientation.set(0, 0.0f);
    gams_info_[i].orientation.set(1, 0.0f);
    
    UE_LOG(LogGamsAgentManager, Log,
      TEXT("agent.%d::update: setting current=%s"),
      (int)i, *transforms_[i].GetLocation().ToString());
  }

  actors_->BatchUpdateInstancesTransforms(0, transforms_, true, false,
    !gams_game_instance->enable_collisions);

  render();
}

void AGamsAgentManager::spawn(void)
{

}

void AGamsAgentManager::spawn(uint32 id, FTransform transform)
{     
  int32 instance = actors_->AddInstanceWorldSpace(transform);
  agent_id_to_instance_.Add(id, instance);
  instances_.Add(id);
}

void AGamsAgentManager::update(uint32 id, FTransform transform)
{
  uint32 instance = agent_id_to_instance_[id];
  UE_LOG(LogGamsAgentManager, Log,
    TEXT("agent.%d::update: moving to transform[%s]"),
    (int)id, *transform.GetLocation().ToString());

  actors_->UpdateInstanceTransform(instance, transform, true, false,
    !gams_game_instance->enable_collisions);
}

void AGamsAgentManager::destroy(uint32 id)
{
  // move removes to end so we maintain sorted instance order
  // if speed becomes an issue, we can ignore/remove much of this
  uint32 instance_to_remove = agent_id_to_instance_[id];
  uint32 last_instance = instances_.Num() - 1;
  uint32 instance_to_move = instances_[last_instance];
  instances_[instance_to_remove] = instance_to_move;
  agent_id_to_instance_[instance_to_move] = agent_id_to_instance_[id];
  agent_id_to_instance_.Remove(id);
  
  actors_->RemoveInstance(instance_to_remove);
  instances_.RemoveAt(last_instance);
}

void AGamsAgentManager::render(void)
{
  actors_->MarkRenderStateDirty();
}

uint32 AGamsAgentManager::size(void)
{
  return instances_.Num();
}
