// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Containers/Array.h"
#include "Math/TransformNonVectorized.h"
#include "UObject/UObjectGlobals.h"
#include "GamsAgentInstance.h"

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
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/knowledge/containers/Integer.h"
#pragma warning(pop)

#include "GamsAgentManager.generated.h"

class UStaticMeshComponent;
class UMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAMSAGENTS_API AGamsAgentManager : public AActor
{
	GENERATED_BODY()
	
public:

	AGamsAgentManager();
  
	virtual ~AGamsAgentManager();
  
  void read(madara::knowledge::KnowledgeBase & kb);
  
  void spawn(void);

  void spawn(uint32 id, FTransform transform);
  
  void update(float delta_time);

  void update(uint32 id, FTransform transform);

  void clear(void);

  void destroy(uint32 id);

  uint32 size(void);

  void render(void);

private:
  TMap<uint32, uint32> agent_id_to_instance_;
  
  UHierarchicalInstancedStaticMeshComponent * actors_;

  TArray<uint32> instances_;
  
  madara::knowledge::containers::Integer swarm_size_;

  FString platform_type_;

  TArray<GamsAgentInstance> gams_info_;

  TArray<FTransform> transforms_;
};
