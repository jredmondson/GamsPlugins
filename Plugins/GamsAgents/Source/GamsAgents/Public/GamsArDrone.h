// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamsAerialVehicle.h"
#include "GamsArDrone.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API AGamsArDrone : public AGamsAerialVehicle
{
	GENERATED_BODY()

public:
  AGamsArDrone();

  /**
   * Animates the vehicle over the change in time
   * @param  delta_time the time to animate to from current pose
   **/
  virtual void animate(float delta_time) override;
};
