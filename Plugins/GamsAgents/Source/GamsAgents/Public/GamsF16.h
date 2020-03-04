// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamsJetVehicle.h"
#include "GamsF16.generated.h"

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API AGamsF16 : public AGamsJetVehicle
{
	GENERATED_BODY()

public:
  AGamsF16();

  virtual void BeginPlay() override;

  /**
   * Animates the vehicle over the change in time
   * @param  delta_time the time to animate to from current pose
   **/
  virtual void animate(float delta_time) override;

  //UPROPERTY(VisibleAnywhere, Category = Movement)
  //  UStaticMeshComponent* rotor1;

  //UPROPERTY(VisibleAnywhere, Category = Movement)
  //  UStaticMeshComponent* rotor2;

  //UPROPERTY(VisibleAnywhere, Category = Movement)
  //  UStaticMeshComponent* rotor3;

  //UPROPERTY(VisibleAnywhere, Category = Movement)
  //  UStaticMeshComponent* rotor4;
};
