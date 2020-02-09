// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GamsIncludes.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamsAgentQuad_DjiPhantom.generated.h"

UCLASS()
class GAMSPLUGINS_API AGamsAgentQuad_DjiPhantom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamsAgentQuad_DjiPhantom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
  // GAMS controller for the agent or agents
  gams::controllers::Multicontroller controller;
};
