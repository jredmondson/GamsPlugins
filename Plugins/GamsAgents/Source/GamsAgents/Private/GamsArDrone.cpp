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

AGamsArDrone::AGamsArDrone()
  : AGamsAerialVehicle()
{
  PrimaryActorTick.bCanEverTick = false;
  this->max_speed = 500.0f;

  mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  //mesh->SetupAttachment(RootComponent);
  RootComponent = mesh;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("DjiPhantom: constr: entering"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(
    TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Main.SM_QuadcopterB_Main"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> rotor_asset(
    TEXT("/Game/Quadcopters/Mesh/SM_QuadcopterB_Rotor.SM_QuadcopterB_Rotor"));

  static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> mesh_mat(
    TEXT("/Game/Quadcopters/Materials/MI_QuadcopterB_01.MI_QuadcopterB_01"));

  static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> rotor_mat(
    TEXT("/Game/Quadcopters/Materials/MI_QuadcopterB_02.MI_QuadcopterB_02"));

  if (mesh_asset.Succeeded())
  {
    FRotator rotation;
    rotation.Roll = 0.0f;
    rotation.Pitch = 0.0f;

    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("DjiPhantom: constr: loaded root mesh"));

    mesh->SetStaticMesh(mesh_asset.Object);
    mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    mesh->SetWorldScale3D(FVector(1.0f));

    //movement = CreateDefaultSubobject<UMovementComponent>(TEXT("MovementComponent"));
    //movement->SetUpdatedComponent(mesh);
    //movement->InitialSpeed = 10.0f;
    //movement->MaxSpeed = 30.0f;
    //movement->bRotationFollowsVelocity = true;
    //movement->bShouldBounce = true;
    //movement->Bounciness = 0.3f;

    // setup the rotors
    if (rotor_asset.Succeeded())
    {
      rotor1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor1"));
      rotor2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor2"));
      rotor3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor3"));
      rotor4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor4"));

      rotor1->SetupAttachment(mesh);
      rotor2->SetupAttachment(mesh);
      rotor3->SetupAttachment(mesh);
      rotor4->SetupAttachment(mesh);

      rotation.Yaw = -45.0f;
      rotor1->SetStaticMesh(rotor_asset.Object);
      rotor1->SetMobility(EComponentMobility::Movable);
      rotor1->SetRelativeLocationAndRotation(
        FVector(-13.941086f, 13.752662f, 6.546223f),
        rotation);
      rotor1->SetWorldScale3D(FVector(1.0f));

      rotation.Yaw = 45.0f;
      rotor2->SetStaticMesh(rotor_asset.Object);
      rotor2->SetMobility(EComponentMobility::Movable);
      rotor2->SetRelativeLocationAndRotation(
        FVector(-13.946138f, -13.827914f, 6.546223f),
        rotation);
      rotor2->SetWorldScale3D(FVector(1.0f));

      rotation.Yaw = -45.0f;
      rotor3->SetStaticMesh(rotor_asset.Object);
      rotor3->SetMobility(EComponentMobility::Movable);
      rotor3->SetRelativeLocationAndRotation(
        FVector(13.942262f, -13.795097f, 6.546223f),
        rotation);
      rotor3->SetWorldScale3D(FVector(1.0f));

      rotation.Yaw = 45.0f;
      rotor4->SetStaticMesh(rotor_asset.Object);
      rotor4->SetMobility(EComponentMobility::Movable);
      rotor4->SetRelativeLocationAndRotation(
        FVector(13.963983f, 13.841137f, 6.546223f),
        rotation);
      rotor4->SetWorldScale3D(FVector(1.0f));

      if (rotor_mat.Succeeded())
      {
        rotor1->SetMaterial(0, rotor_mat.Object);
        rotor2->SetMaterial(0, rotor_mat.Object);
      }
      if (mesh_mat.Succeeded())
      {
        rotor3->SetMaterial(0, mesh_mat.Object);
        rotor3->SetMaterial(0, mesh_mat.Object);
      }

      //camera_boom = CreateDefaultSubobject<USpringArmComponent>(
      //  TEXT("CameraBoom"));
      //camera_boom->SetupAttachment(mesh);
      //camera_boom->TargetArmLength = 500.0f;
      //camera_boom->bUsePawnControlRotation = true;

      //camera->CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
      //camera->SetupAttachment(camera_boom, USpringArmComponent::SocketName);
      //camera->bUsePawnControlRotation = false;
    }
    else
    {
      UE_LOG(LogUnrealAgentPlatform, Warning,
        TEXT("DjiPhantom: constr: ERROR: couldn't load rotors"));
    }

    if (mesh_mat.Succeeded())
    {
      mesh->SetMaterial(0, mesh_mat.Object);
    }
    else
    {
      UE_LOG(LogUnrealAgentPlatform, Warning,
        TEXT("DjiPhantom: constr: ERROR: couldn't load root mesh material"));
    }


  } // end root mesh check
  else
  {
    UE_LOG(LogUnrealAgentPlatform, Warning,
      TEXT("DjiPhantom: constr: ERROR: couldn't load root mesh"));
  }
}

void AGamsArDrone::BeginPlay()
{
}

void AGamsArDrone::animate(float delta_time)
{
  FRotator rotation;
  rotation.Roll = 0.0f;
  rotation.Pitch = 0.0f;
  rotation.Yaw = FMath::FRandRange(1.0f, 90.0f);
  rotor1->AddRelativeRotation(rotation);

  rotation.Yaw = FMath::FRandRange(-1.0f, -90.0f);
  rotor2->AddRelativeRotation(rotation);

  rotation.Yaw = FMath::FRandRange(1.0f, 90.0f);
  rotor3->AddRelativeRotation(rotation);

  rotation.Yaw = FMath::FRandRange(-1.0f, -90.0f);
  rotor4->AddRelativeRotation(rotation);
}
