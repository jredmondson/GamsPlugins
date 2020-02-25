// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamsVehicle.generated.h"

class UStaticMeshComponent;
class UMovementComponent;

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

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float max_speed;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  float acceleration;
};
