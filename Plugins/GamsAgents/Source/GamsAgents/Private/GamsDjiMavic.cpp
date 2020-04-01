// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsDjiMavic.h"
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

AGamsDjiMavic::AGamsDjiMavic()
  : AGamsAerialVehicle()
{
  this->max_speed = 1800.0f;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("DjiMavic: constr: entering"));
  
  actors_ = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors"));
  UStaticMesh* root_mesh =  Cast<UStaticMesh>(StaticLoadObject(
    UStaticMesh::StaticClass(), NULL, TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterC_Main")));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterC_Main.SM_QuadcopterC_Main"));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> rotor_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterC_Rotor.SM_QuadcopterC_Rotor"));

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
  //  FVector(-12.897691f, 14.643147f, 7.243829f),
  //  rotation);

  //rotation.Yaw = 45.0f;
  //rotor2->SetRelativeLocationAndRotation(
  //  FVector(-12.887679f, -14.644179f, 7.243829f),
  //  rotation);
  //rotor2->SetWorldScale3D(FVector(1.0f));

  //rotation.Yaw = -45.0f;
  //rotor3->SetRelativeLocationAndRotation(
  //  FVector(12.807207f, -15.152422f, 12.236739f),
  //  rotation);

  //rotation.Yaw = 45.0f;
  //rotor4->SetRelativeLocationAndRotation(
  //  FVector(12.826573f, 15.175086f, 12.236739f),
  //  rotation);
}

void AGamsDjiMavic::animate(float delta_time)
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
