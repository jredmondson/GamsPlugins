// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamsAerialVehicle.h"
#include "GamsDjiMavic.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API AGamsDjiMavic : public AGamsAerialVehicle
{
	GENERATED_BODY()

public:
  AGamsDjiMavic();

  virtual void BeginPlay() override;

  /**
   * Animates the vehicle over the change in time
   * @param  delta_time the time to animate to from current pose
   **/
  virtual void animate(float delta_time) override;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  UStaticMeshComponent* rotor1;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  UStaticMeshComponent* rotor2;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  UStaticMeshComponent* rotor3;

  UPROPERTY(VisibleAnywhere, Category = Movement)
  UStaticMeshComponent* rotor4;
};
