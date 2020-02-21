// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamsVehicle.generated.h"

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

  UPROPERTY (EditAnywhere, BlueprintReadOnly)
    float move_forward;

  UPROPERTY (EditAnywhere, BlueprintReadOnly)
    float move_right;

  UPROPERTY (EditAnywhere, BlueprintReadOnly)
    float move_up;

  UPROPERTY (EditAnywhere, BlueprintReadOnly)
    float look_up;

};
