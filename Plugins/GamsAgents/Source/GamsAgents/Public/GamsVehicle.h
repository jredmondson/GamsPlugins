// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Containers/Array.h"
#include "Math/TransformNonVectorized.h"
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
#pragma warning(pop)

#include "GamsVehicle.generated.h"

class UStaticMesh;
class UMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class GAMSAGENTS_API AGamsVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamsVehicle();

public:	
  void init(TArray<GamsAgentInstance> & gams_info);

  void clear(void);
  
  void destroy(uint32 id);
  
  void render(void);

  uint32 size(void);

  void spawn(uint32 id, FTransform transform);
  
  void update(float delta_time);
  
  void update(uint32 id, FTransform transform);

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float max_speed;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float acceleration;
  
  UPROPERTY(VisibleAnywhere, Category = Movement)
  UHierarchicalInstancedStaticMeshComponent * actors_;

  /**
   * Animates the vehicle over the change in time
   * @param  delta_time the time to animate to from current pose
   **/
  virtual void animate(float delta_time);

  bool can_fly;

  bool is_fixed_wing;

  bool is_jet;

  TMap<uint32, uint32> agent_id_to_instance_;
  
  TArray<uint32> instances_;
  
  FString platform_type_;

  TArray<GamsAgentInstance> * gams_info_;

  TArray<FTransform> transforms_;
};
