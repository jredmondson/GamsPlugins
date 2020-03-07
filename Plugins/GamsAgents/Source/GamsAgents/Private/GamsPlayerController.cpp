// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsPlayerController.h"

#include "Engine/LocalPlayer.h"

#include "GamsCameraComponent.h"
#include "GamsSpectatorPawn.h"

AGamsPlayerController::AGamsPlayerController()
  : APlayerController()
{
  PrimaryActorTick.bCanEverTick = true;
  SetHidden(false);
  bShowMouseCursor = true;
}

AGamsSpectatorPawn* AGamsPlayerController::GetMainSpectatorPawn() const
{
  return Cast<AGamsSpectatorPawn>(GetPawn());
}

void AGamsPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AGamsPlayerController::ZoomIn);
  InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AGamsPlayerController::ZoomOut);

  InputComponent->BindAxis("MoveForward", this, &AGamsPlayerController::MovePawnForward);
  InputComponent->BindAxis("MoveRight", this, &AGamsPlayerController::MovePawnRight);
}

void AGamsPlayerController::ProcessPlayerInput(const float deltaTime, const bool bGamePaused)
{
  Super::ProcessPlayerInput(deltaTime, bGamePaused);

  {
    const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);
    AGamsSpectatorPawn* pawn = GetMainSpectatorPawn();

    if (pawn && localPlayer)
    {
      if (localPlayer->ViewportClient)
      {
        pawn->GetCameraComponent()->UpdateCameraMovement(this);
      }
    }
  }
}

void AGamsPlayerController::MovePawnForward(const float value)
{
  AGamsSpectatorPawn* pawn = Cast<AGamsSpectatorPawn>(GetPawn());
  if (value != 0.f && pawn != nullptr)
  {
    pawn->MoveForward(value);
  }
}

void AGamsPlayerController::MovePawnRight(const float value)
{
  AGamsSpectatorPawn* pawn = Cast<AGamsSpectatorPawn>(GetPawn());
  if (value != 0.f && pawn != nullptr)
  {
    pawn->MoveRight(value);
  }
}

void AGamsPlayerController::ZoomIn()
{
  AGamsSpectatorPawn* pawn = Cast<AGamsSpectatorPawn>(GetPawn());
  if (pawn != nullptr)
  {
    pawn->OnMouseScrollUp();
  }
}

void AGamsPlayerController::ZoomOut()
{
  AGamsSpectatorPawn* pawn = Cast<AGamsSpectatorPawn>(GetPawn());
  if (pawn != nullptr)
  {
    pawn->OnMouseScrollDown();
  }
}