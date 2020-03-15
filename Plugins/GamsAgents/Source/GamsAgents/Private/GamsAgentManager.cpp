// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsAgentManager.h"
#include "GamsGameInstance.h"
#include "Engine/StaticMesh.h"
#include "MadaraIncludes.h"
#include "GamsAgentsLogs.h"

AGamsAgentManager::AGamsAgentManager()
  : AActor()
{
  actors = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors")); 
  UStaticMesh* root_mesh = 
    Cast<UStaticMesh>(StaticLoadObject(
      UStaticMesh::StaticClass(), NULL, TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Main")));

  //actors->RegisterComponent(); 
  actors->SetStaticMesh(root_mesh); 
  actors->SetFlags(RF_Transactional); 
  this->AddInstanceComponent(actors);
}

void AGamsAgentManager::read(madara::knowledge::KnowledgeBase & kb)
{
  swarm_size_.set_name("swarm.size", kb);

  uint32 num_instances = (uint32)(*swarm_size_ > 0 ? *swarm_size_ : 100);

  gams_info_.SetNum(num_instances);

  UE_LOG(LogGamsAgentManager, Log,
    TEXT("num_instances=%d"),
    (int)num_instances);

  for (uint32 i = 0; i < num_instances; ++i)
  {
    // initialize the agent variables
    gams_info_[i].init(i, kb);

    // write the position and orientation info from the kb
    gams_info_[i].fill_current();

    
    UE_LOG(LogGamsAgentManager, Log,
      TEXT("agent.%d: spawning at location=%s"),
      (int)i, *gams_info_[i].current.GetLocation().ToString());


    spawn(i, gams_info_[i].current);
  }
}

void AGamsAgentManager::spawn(void)
{

}

void AGamsAgentManager::spawn(uint32 id, FTransform transform)
{     
  int32 instance = actors->AddInstanceWorldSpace(transform);
  agent_id_to_instance_.Add(id, instance);
  instances_.Add(id);
}

void AGamsAgentManager::transform(uint32 id, FTransform transform)
{
  uint32 instance = agent_id_to_instance_[id];
  actors->UpdateInstanceTransform(instance, transform, true, true, false);
}

void AGamsAgentManager::destroy(uint32 id) {
  // move removes to end so we maintain sorted instance order
  // if speed becomes an issue, we can ignore/remove much of this
  uint32 instance_to_remove = agent_id_to_instance_[id];
  uint32 last_instance = instances_.Num() - 1;
  uint32 instance_to_move = instances_[last_instance];
  instances_[instance_to_remove] = instance_to_move;
  agent_id_to_instance_[instance_to_move] = agent_id_to_instance_[id];
  agent_id_to_instance_.Remove(id);
  
  actors->RemoveInstance(instance_to_remove);
  instances_.RemoveAt(last_instance);
}

void AGamsAgentManager::render(void) {
  actors->MarkRenderStateDirty();
}

uint32 AGamsAgentManager::size(void)
{
  return instances_.Num();
}
