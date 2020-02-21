// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamsAerialVehicle.h"
#include "GamsDjiPhantom.generated.h"

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API AGamsDjiPhantom : public AGamsAerialVehicle
{
	GENERATED_BODY()

public:
  AGamsDjiPhantom();

  virtual void BeginPlay() override;

  virtual void Tick(float delta_time) override;

};
