// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamsPlayerController.generated.h"

class AGamsSpectatorPawn;

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API AGamsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
  AGamsPlayerController();

  AGamsSpectatorPawn* GetMainSpectatorPawn() const;

protected:
  virtual void SetupInputComponent() override;
  virtual void ProcessPlayerInput(const float deltaTime, const bool bGamePaused) override;

  void MovePawnForward(float value);
  void MovePawnRight(float value);
  void ZoomIn();
  void ZoomOut();
};
