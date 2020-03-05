#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GamsCameraComponent.generated.h"

class AGamsPlayerController;

UCLASS()
class GAMSAGENTS_API UGamsCameraComponent : public UCameraComponent
{
  GENERATED_BODY()

public:
  UGamsCameraComponent();

  virtual void GetCameraView(float deltaTime, FMinimalViewInfo& desiredView) override;

  void OnZoomIn();
  void OnZoomOut();
  void UpdateCameraMovement(const APlayerController* playerController) const;
  void MoveForward(float value) const;
  void MoveRight(float value) const;
  void SetZoomLevel(float level);
  void ClampCameraLocation(const APlayerController *playerController, FVector& outCameraLocation) const;

private:
  float min_camera_offset_;
  float max_camera_offset_;
  FRotator fixedCameraAngle_;

  float scroll_speed_;
  uint32 camera_border_;
  float min_zoom_;
  float max_zoom_;
  float zoom_alpha_;
  float move_speed_;

  bool should_clamp_ : 1;

  FBox cameraMovementBounds_;
  FVector2D cameraMovementViewportSize_;

  APawn* GetOwnerPawn() const;
  AGamsPlayerController *GetPlayerController() const;
  //void UpdateCameraBounds(const APlayerController* playerController) const;
};
