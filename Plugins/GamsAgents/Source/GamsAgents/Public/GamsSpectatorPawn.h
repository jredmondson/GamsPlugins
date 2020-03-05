#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "GamsSpectatorPawn.generated.h"

class UGamsCameraComponent;

/**
 *
 */
UCLASS()
class GAMSAGENTS_API AGamsSpectatorPawn : public ASpectatorPawn
{
  GENERATED_BODY()

    friend class AGamsPlayerController;

public:
  AGamsSpectatorPawn(const FObjectInitializer& ObjectInitializer);
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

  UGamsCameraComponent* GetCameraComponent()
  {
    return camera_;
  }

private:
  void MoveForward(float value);
  void MoveRight(float value);
  void OnMouseScrollUp();
  void OnMouseScrollDown();

  UGamsCameraComponent* camera_;
};