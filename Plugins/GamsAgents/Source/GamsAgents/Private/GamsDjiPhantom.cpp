// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsDjiPhantom.h"
//#include "UObject/ConstructorHelpers.h"
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
#include "GamsGameInstance.h"

AGamsDjiPhantom::AGamsDjiPhantom()
  : AGamsAerialVehicle()
{
  this->max_speed = 2000.0f;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("DjiPhantom: constr: entering"));
  
  actors_ = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors"));
  UStaticMesh* root_mesh =  Cast<UStaticMesh>(StaticLoadObject(
    UStaticMesh::StaticClass(), NULL, TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterA_Main")));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterA_Main.SM_QuadcopterA_Main"));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> rotor_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterA_Rotor.SM_QuadcopterA_Rotor"));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> cam_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterA_Cam.SM_QuadcopterA_Cam"));

  actors_->SetStaticMesh(root_mesh);
  actors_->SetFlags(RF_Transactional);

  if (gams_game_instance != nullptr)
  {
    actors_->SetCollisionEnabled(gams_game_instance->collision_type);
  }
  else
  {
    actors_->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }
  this->AddInstanceComponent(actors_);

  //rotor1->SetRelativeLocationAndRotation(
  //  FVector(-12.106963f, 11.918432f, 16.028538f),
  //  rotation);
  //rotor2->SetRelativeLocationAndRotation(
  //  FVector(-12.096648f, -11.978424f, 16.028538f),
  //  rotation);
  //rotor3->SetRelativeLocationAndRotation(
  //  FVector(12.108811f, -11.961754f, 16.028538f),
  //  rotation);
  //rotor4->SetRelativeLocationAndRotation(
  //  FVector(12.097581f, 11.974735f, 16.028526f),
  //  rotation);
}

void AGamsDjiPhantom::animate(float delta_time)
{
  //FRotator rotation;
  //rotation.Roll = 0.0f;
  //rotation.Pitch = 0.0f;
  //rotation.Yaw = FMath::FRandRange(1.0f, 90.0f);
  //rotor1->AddRelativeRotation(rotation);

  //rotation.Yaw = FMath::FRandRange(-1.0f, -90.0f);
  //rotor2->AddRelativeRotation(rotation);

  //rotation.Yaw = FMath::FRandRange(1.0f, 90.0f);
  //rotor3->AddRelativeRotation(rotation);

  //rotation.Yaw = FMath::FRandRange(-1.0f, -90.0f);
  //rotor4->AddRelativeRotation(rotation);
}
