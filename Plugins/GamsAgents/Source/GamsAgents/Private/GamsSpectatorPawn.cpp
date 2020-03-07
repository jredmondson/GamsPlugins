#include "GamsSpectatorPawn.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/InputComponent.h"
#include "GamsCameraComponent.h"

AGamsSpectatorPawn::AGamsSpectatorPawn(const FObjectInitializer& ObjectInitializer)
  : ASpectatorPawn(ObjectInitializer)
{
  PrimaryActorTick.bCanEverTick = false;

  GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
  bAddDefaultMovementBindings = false;

  camera_ = CreateDefaultSubobject<UGamsCameraComponent>(TEXT("Camera Component"));
}

void AGamsSpectatorPawn::BeginPlay()
{
  Super::BeginPlay();

  SetActorLocation(FVector::ZeroVector);
}

void AGamsSpectatorPawn::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}

void AGamsSpectatorPawn::MoveForward(const float value)
{
  if (value != 0.f)
  {
    camera_->MoveForward(value);
  }
}

void AGamsSpectatorPawn::MoveRight(const float value)
{
  if (value != 0.f)
  {
    camera_->MoveRight(value);
  }
}

void AGamsSpectatorPawn::OnMouseScrollUp()
{
  camera_->OnZoomIn();
}

void AGamsSpectatorPawn::OnMouseScrollDown()
{
  camera_->OnZoomOut();
}