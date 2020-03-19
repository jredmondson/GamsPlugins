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
class AGamsVehicle;

UCLASS()
class GAMSAGENTS_API AGamsAgentManager : public AActor
{
	GENERATED_BODY()
	
public:

	AGamsAgentManager();
  
	virtual ~AGamsAgentManager();
  
  virtual void BeginPlay() override;

  void read(madara::knowledge::KnowledgeBase & kb);
  
  void update(float delta_time);

  void clear(void);

  void destroy(uint32 id);

  uint32 size(void);

  void render(void);

private:
  /**
   * agent.id to instance translations
   **/
  TMap<uint32, uint32> agent_id_to_manager_;
  
  /**
   * Platform type instance managers that do all of the core work of tracking
   * and updating platform instances and all subcomponents
   **/
  TArray<AGamsVehicle *> managers_;
  
  /**
   * Reference to swarm.size in the game knowledge base
   **/
  madara::knowledge::containers::Integer swarm_size_;

  /**
   * The platform.type argument, which currently is tied to the game instance
   * value, but which may one day be allowed to change dynamically at runtime
   **/
  FString platform_type_str_;

  /**
   * Gams Agent information (e.g., location, orientation, dest, etc.)
   **/
  TArray<GamsAgentInstance> gams_info_;
};
