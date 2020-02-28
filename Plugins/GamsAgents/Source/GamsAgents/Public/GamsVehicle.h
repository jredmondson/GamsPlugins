// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

class UStaticMeshComponent;
class UMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAMSAGENTS_API AGamsVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamsVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UPROPERTY(VisibleAnywhere, Category = Vehicle)
    UStaticMeshComponent* mesh;

  UPROPERTY(VisibleAnywhere, Category = Movement)
    UMovementComponent* movement;

  UPROPERTY(VisibleAnywhere, Category = Camera)
    USpringArmComponent* camera_boom;

  UPROPERTY(VisibleAnywhere, Category = Camera)
    UCameraComponent* camera;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float max_speed;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float acceleration;

  void init_knowledge(
    madara::knowledge::KnowledgeBase & knowledge,
    const std::string& prefix);

  void init_knowledge(const std::string& prefix);

  /// for checkpointing and interacting with main render loop,
  /// keep track of the actor location, orientation, and targets
  madara::knowledge::containers::NativeDoubleVector dest;
  madara::knowledge::containers::NativeDoubleVector dest_orientation;
  madara::knowledge::containers::NativeDoubleVector location;
  madara::knowledge::containers::NativeDoubleVector orientation;
  madara::knowledge::containers::NativeDoubleVector source;
  madara::knowledge::containers::NativeDoubleVector source_orientation;
  madara::knowledge::containers::NativeDoubleArray velocity;

  std::string agent_prefix;
};
