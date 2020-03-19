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
#include "GamsGameInstance.h"


AGamsF16::AGamsF16()
  : AGamsJetVehicle()
{
  is_fixed_wing = true;

  this->max_speed = 10000.0f;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("F16: constr: entering"));
  
  actors_ = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(
    TEXT("InstancedActors")); 
  UStaticMesh* root_mesh =  Cast<UStaticMesh>(StaticLoadObject(
    UStaticMesh::StaticClass(), NULL, TEXT("/Game/Aircraft/meshes/SM_F16_Main")));

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
}

void AGamsF16::animate(float delta_time)
{
  //FVector start, target;

  //madara::utility::to_vector_multiply(this->source, start);
  //madara::utility::to_vector_multiply(this->dest, target);

  //if (!start.Equals(target, 10.0f))
  //{
  //  FRotator rotation = UKismetMathLibrary::FindLookAtRotation(start, target);
  //  rotation.Yaw += 90.0f;
  //  this->SetActorRotation(rotation);
  //}
}
