// Fill out your copyright notice in the Description page of Project Settings.


#include "GamsF16.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GamsAgentsLogs.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "MadaraUnrealUtility.h"


AGamsF16::AGamsF16()
  : AGamsJetVehicle()
{
  is_fixed_wing = true;

  PrimaryActorTick.bCanEverTick = false;
  this->max_speed = 2000.0f;

  mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  //mesh->SetupAttachment(RootComponent);
  RootComponent = mesh;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("F16: constr: entering"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(
    TEXT("/Game/Aircraft/meshes/SM_F16_Main.SM_F16_Main"));

  static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> mesh_mat1(
    TEXT("/Game/Aircraft/materials/M_wheels_Inst.M_wheels_Inst"));

  static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> mesh_mat2(
    TEXT("/Game/Aircraft/materials/M_Jet_04.M_Jet_04"));

  if (mesh_asset.Succeeded())
  {
    FRotator rotation;
    rotation.Roll = 0.0f;
    rotation.Pitch = 0.0f;
    rotation.Yaw = 90.0f;

    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("F16: constr: loaded root mesh"));

    mesh->SetStaticMesh(mesh_asset.Object);
    mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    mesh->SetRelativeRotation(rotation);
    mesh->SetWorldScale3D(FVector(1.0f));

    if (mesh_mat1.Succeeded())
    {
      mesh->SetMaterial(0, mesh_mat1.Object);
    }
    else
    {
      UE_LOG(LogUnrealAgentPlatform, Warning,
        TEXT("F16: constr: ERROR: couldn't load root mesh material"));
    }

    if (mesh_mat2.Succeeded())
    {
      mesh->SetMaterial(1, mesh_mat2.Object);
    }
    else
    {
      UE_LOG(LogUnrealAgentPlatform, Warning,
        TEXT("F16: constr: ERROR: couldn't load root mesh material"));
    }


  } // end root mesh check
  else
  {
    UE_LOG(LogUnrealAgentPlatform, Warning,
      TEXT("F16: constr: ERROR: couldn't load root mesh"));
  }
}

void AGamsF16::BeginPlay()
{
}

void AGamsF16::animate(float delta_time)
{
  FVector start, target;

  madara::utility::to_vector_multiply(this->source, start);
  madara::utility::to_vector_multiply(this->dest, target);

  if (!start.Equals(target, 10.0f))
  {
    FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);
    rotation.Yaw += 90.0f;
    this->SetActorRotation(rotation);
  }
}
