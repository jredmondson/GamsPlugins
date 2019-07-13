// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Quadcopter.generated.h"

UCLASS()
class GAMSAGENTS_API AQuadcopter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuadcopter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  // Called when the game starts or when spawned
  virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UPROPERTY (VisibleAnywhere)
  UStaticMeshComponent * agent_mesh_;
};
