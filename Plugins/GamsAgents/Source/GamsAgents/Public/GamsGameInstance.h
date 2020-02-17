// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GamsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API UGamsGameInstance : public UGameInstance
{
  GENERATED_BODY ()

public:

  virtual void Init () override;
  //virtual void OnWorldChanged (
  //  UWorld* OldWorld,
  //  UWorld* NewWorld) override;
};

extern UWorld * gams_current_world;
extern float gams_delta_time;
