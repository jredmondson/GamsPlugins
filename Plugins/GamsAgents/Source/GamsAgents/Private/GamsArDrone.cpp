// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsArDrone.h"
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

AGamsArDrone::AGamsArDrone()
  : AGamsAerialVehicle()
{
  this->max_speed = 1100.0f;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("DjiPhantom: constr: entering"));
  
  actors_ = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors"));
  UStaticMesh* root_mesh =  Cast<UStaticMesh>(StaticLoadObject(
    UStaticMesh::StaticClass(), NULL, TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Main")));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Main.SM_QuadcopterB_Main"));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> rotor_asset(
  //  TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Rotor.SM_QuadcopterB_Rotor"));
  
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
  //  FVector(-13.941086f, 13.752662f, 6.546223f),
  //  rotation);

  //rotor2->SetRelativeLocationAndRotation(
  //  FVector(-13.946138f, -13.827914f, 6.546223f),
  //  rotation);

  //rotor3->SetRelativeLocationAndRotation(
  //  FVector(13.942262f, -13.795097f, 6.546223f),
  //  rotation);

  //rotation.Yaw = 45.0f;
  //rotor4->SetStaticMesh(rotor_asset.Object);
  //rotor4->SetMobility(EComponentMobility::Movable);
  //rotor4->SetRelativeLocationAndRotation(
  //  FVector(13.963983f, 13.841137f, 6.546223f),
  //  rotation);
  //rotor4->SetWorldScale3D(FVector(1.0f));
}

void AGamsArDrone::animate(float delta_time)
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
