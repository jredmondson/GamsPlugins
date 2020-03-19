// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsVehicle.h"
#include "GamsGameInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UnrealAgentPlatform.h"
#include "GamsAgentsLogs.h"

// Sets default values
AGamsVehicle::AGamsVehicle()
  : AActor(), can_fly (false), is_fixed_wing(false), is_jet(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AGamsVehicle::animate(float delta_time)
{
}

void AGamsVehicle::init(TArray<GamsAgentInstance> & gams_info)
{
  // each actor references a gams_info object that is held in the caller
  // @see AGamsAgentManager
  gams_info_ = &gams_info;
}

void AGamsVehicle::clear(void)
{
  //if (actors_ != nullptr)
  //{
  //  actors_->ClearInstances();
  //}
}

void AGamsVehicle::update(float delta_time)
{
  FVector dest;
  FVector diff;
  FVector next;
  FTransform transform(FRotator::ZeroRotator);

  for (auto i : agent_id_to_instance_)
  {
    GamsAgentInstance & instance = (*gams_info_)[i.Key];
    
    // grab the agent destination
    madara::utility::to_vector_multiply(instance.dest, dest);

    // calculate the total diff between current and dest
    diff = dest - transforms_[i.Value].GetLocation();
    
    // calculate the delta that is accomplishable in the time given
    madara::utility::calculate_delta(diff, next,
      instance.platform_speed, delta_time);
    
    // add the delta to the current location
    next += transforms_[i.Value].GetLocation();
  
    // update the transform and agent location
    // instance.transform.SetLocation(next);
    transforms_[i.Value].SetLocation(next);
    madara::utility::from_vector_multiply(next, instance.location);

    // update the orientation to vector of zero (not ideal, temporary)
    instance.orientation.set(2, 0.0f);
    instance.orientation.set(0, 0.0f);
    instance.orientation.set(1, 0.0f);
    
    UE_LOG(LogGamsVehicle, Log,
      TEXT("agent.%d::update: setting current=%s"),
      (int)i.Key, *instance.transform.GetLocation().ToString());
  }

  actors_->BatchUpdateInstancesTransforms(0, transforms_, true, false,
    !gams_game_instance->enable_collisions);
}

void AGamsVehicle::spawn(uint32 id, FTransform transform)
{     
  (*gams_info_)[id].instance = actors_->AddInstanceWorldSpace(transform);
  agent_id_to_instance_.Add(id, (*gams_info_)[id].instance);
  instances_.Add(id);
  transforms_.Add(transform);
}

void AGamsVehicle::update(uint32 id, FTransform transform)
{
  uint32 instance = agent_id_to_instance_[id];
  UE_LOG(LogGamsVehicle, Log,
    TEXT("agent.%d::update: moving to transform[%s]"),
    (int)id, *transform.GetLocation().ToString());

  actors_->UpdateInstanceTransform(instance, transform, true, false,
    !gams_game_instance->enable_collisions);
}

void AGamsVehicle::destroy(uint32 id)
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

void AGamsVehicle::render(void)
{
  actors_->MarkRenderStateDirty();
}

uint32 AGamsVehicle::size(void)
{
  return instances_.Num();
}
