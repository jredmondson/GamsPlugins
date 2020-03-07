#include "GamsCameraComponent.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpectatorPawn.h"


#include "GamsPlayerController.h"
#include "GamsSpectatorPawn.h"
#include "Engine/Engine.h"

UGamsCameraComponent::UGamsCameraComponent()
  : UCameraComponent()
{
  should_clamp_ = true;

  zoom_alpha_ = 1.f;
  scroll_speed_ = 3000.f;
  camera_border_ = 100.f;
  min_zoom_ = 0.4f;
  max_zoom_ = 1.f;
  min_camera_offset_ = 0.f;
  max_camera_offset_ = 20000.f;
  move_speed_ = 100.f;

  fixedCameraAngle_ = FRotator(-60.f, 0.f, 0.f);
}

void UGamsCameraComponent::GetCameraView(float deltaTime, FMinimalViewInfo& desiredView)
{
  APlayerController* playerController = GetPlayerController();

  if (playerController)
  {
    desiredView.FOV = 30.f;
    const float currentOffset = min_camera_offset_ + zoom_alpha_ * (max_camera_offset_ - min_camera_offset_);
    const FVector focalPos = playerController->GetFocalLocation();
    desiredView.Location = focalPos - fixedCameraAngle_.Vector() * currentOffset;
    desiredView.Rotation = fixedCameraAngle_;
  }
}

void UGamsCameraComponent::OnZoomIn()
{
  SetZoomLevel(zoom_alpha_ - 0.1f);
}

void UGamsCameraComponent::OnZoomOut()
{
  SetZoomLevel(zoom_alpha_ + 0.1f);
}

void UGamsCameraComponent::UpdateCameraMovement(const APlayerController* playerController) const
{
  ULocalPlayer* const localPlayer = Cast<ULocalPlayer>(playerController->Player);

  if (localPlayer && localPlayer->ViewportClient && localPlayer->ViewportClient->Viewport)
  {
    FVector2D mousePosition;
    if (!localPlayer->ViewportClient->GetMousePosition(mousePosition))
    {
      return;
    }

    FViewport* viewport = localPlayer->ViewportClient->Viewport;
    const float scrollSpeed = 60.f;
    const FIntPoint viewportSize = viewport->GetSizeXY();

    const uint32 viewLeft = FMath::TruncToInt(localPlayer->Origin.X * viewportSize.X);
    const uint32 viewRight = viewLeft + FMath::TruncToInt(localPlayer->Size.X * viewportSize.X);
    const uint32 viewTop = FMath::TruncToInt(localPlayer->Origin.Y * viewportSize.Y);
    const uint32 viewBottom = viewTop + FMath::TruncToInt(localPlayer->Size.Y * viewportSize.Y);

    const float maxSpeed = scroll_speed_ * FMath::Clamp(zoom_alpha_, min_zoom_, max_zoom_);

    const uint32 mouseX = mousePosition.X;
    const uint32 mouseY = mousePosition.Y;
    float spectatorCameraSpeed = maxSpeed;
    AGamsSpectatorPawn *spectatorPawn = nullptr;

    if (true) //!bNoScrollZone_
    {
      if (mouseX >= viewLeft && mouseX <= (viewLeft + camera_border_))
      {
        const float delta = 1.0f - float(mouseX - viewLeft) / camera_border_;
        spectatorCameraSpeed = delta * maxSpeed;
        MoveRight(-scrollSpeed * delta);
      }
      else if (mouseX >= (viewRight - camera_border_) && mouseX <= viewRight)
      {
        const float delta = float(mouseX - viewRight + camera_border_) / camera_border_;
        spectatorCameraSpeed = delta * maxSpeed;
        MoveRight(scrollSpeed * delta);
      }

      if (mouseY >= viewTop && mouseY <= (viewTop + camera_border_))
      {
        const float delta = 1.0f - float(mouseY - viewTop) / camera_border_;
        spectatorCameraSpeed = delta * maxSpeed;
        MoveForward(scrollSpeed * delta);
      }
      else if (mouseY >= (viewBottom - camera_border_) && mouseY <= viewBottom)
      {
        const float delta = float(mouseY - (viewBottom - camera_border_)) / camera_border_;
        spectatorCameraSpeed = delta * maxSpeed;
        MoveForward(-scrollSpeed * delta);
      }
    }
  }
}

void UGamsCameraComponent::MoveForward(const float value) const
{
  APawn* ownerPawn = GetOwnerPawn();

  if (ownerPawn)
  {
    APlayerController* controller = GetPlayerController();
    if (value != 0.f && controller)
    {
      // If our camera is not rotated or we want to use camera's local coordinates, we can use rotationMatrix
      //const FRotationMatrix rotationMatrix(controller->PlayerCameraManager->GetCameraRotation());
      const FVector worldSpaceAcc = /*rotationMatrix.GetScaledAxis(EAxis::X)*/ FVector(1.f, 0.f, 0.f) * move_speed_;

      ownerPawn->AddMovementInput(worldSpaceAcc, value);
    }
  }
}

void UGamsCameraComponent::MoveRight(const float value) const
{
  APawn* ownerPawn = GetOwnerPawn();

  if (ownerPawn)
  {
    APlayerController* controller = GetPlayerController();
    if (value != 0.f && controller)
    {
      // If our camera is not rotated or we want to use camera's local coordinates, we can use rotationMatrix
      //const FRotationMatrix rotationMatrix(controller->PlayerCameraManager->GetCameraRotation());
      const FVector worldSpaceAcc = /*rotationMatrix.GetScaledAxis(EAxis::Y)*/ FVector(0.f, 1.f, 0.f) * move_speed_;

      ownerPawn->AddMovementInput(worldSpaceAcc, value);
    }
  }
}

void UGamsCameraComponent::SetZoomLevel(float level)
{
  zoom_alpha_ = FMath::Clamp(level, min_zoom_, max_zoom_);
}

APawn* UGamsCameraComponent::GetOwnerPawn() const
{
  return Cast<APawn>(GetOwner());
}

AGamsPlayerController* UGamsCameraComponent::GetPlayerController() const
{
  AGamsPlayerController* controller = nullptr;
  APawn* owner = GetOwnerPawn();
  if (owner)
  {
    controller = Cast<AGamsPlayerController>(owner->GetController());
  }

  return controller;
}
