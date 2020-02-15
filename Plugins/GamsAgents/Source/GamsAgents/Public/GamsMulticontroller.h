// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamsIncludes.h"
#include "GamsMulticontroller.generated.h"

UCLASS()
class GAMSAGENTS_API AGamsMulticontroller : public AActor
{
  GENERATED_BODY()
  
public:  
  // Sets default values for this actor's properties
  AGamsMulticontroller();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:  
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UPROPERTY (EditAnywhere)
    int num_controllers;

  UPROPERTY (EditAnywhere)
    FString platform;

  UPROPERTY (EditAnywhere)
    float controller_hertz;

protected:

  // GAMS multicontroller for controlling multiple platforms
  gams::controllers::Multicontroller controller;

  // the last number of controllers before evaluating blueprint interface
  int last_controllers;

  // the last platform for controllers before evaluating blueprint interface
  FString last_platform;

  // the last controller hertz rate before evaluating blueprint interface
  double last_hertz;
};
