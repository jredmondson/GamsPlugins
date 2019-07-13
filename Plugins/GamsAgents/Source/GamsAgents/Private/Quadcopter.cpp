// Fill out your copyright notice in the Description page of Project Settings.

#include "Quadcopter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AQuadcopter::AQuadcopter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  // Add static mesh component to actor
  agent_mesh_ = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("Quadcopter Model"));

  //static ConstructorHelpers::FObjectFinder<UStaticMesh> quad_mesh (
  //  TEXT ("StaticMesh'/Game/Quadcopters/Mesh/SM_QuadcopterB_Main.SM_QuadcopterB_Main'"));
  //agent_mesh_->SetStaticMesh (quad_mesh.Object);
  //static ConstructorHelpers::FObjectFinder<UMaterial> quad_material (
  //  TEXT ("Material'/Game/Quadcopters/Materials/MI_QuadcopterB_01.MI_QuadcopterB_01'"));
  //agent_mesh_->GetStaticMesh()->SetMaterial (0, quad_material.Object);
}

void AQuadcopter::PostInitializeComponents()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> quad_mesh (
    TEXT ("StaticMesh'/Game/Quadcopters/Mesh/SM_QuadcopterB_Main.SM_QuadcopterB_Main'"));
  agent_mesh_->SetStaticMesh (quad_mesh.Object);
  //static ConstructorHelpers::FObjectFinder<UStaticMeshComponent>MeshAsset (
  //  TEXT ("StaticMesh'/Game/Quadcopters/Mesh/SM_QuadcopterA_Main.SM_QuadcopterA_Main'"));

  //static ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset (
  //  TEXT ("MaterialInstanceConstant'/Game/Quadcopters/Materials/MI_QuadcopterB_01.MI_QuadcopterB_01'"));

  //agent_mesh_->SetStaticMesh(MeshAsset.Object->GetStaticMesh());

}

// Called when the game starts or when spawned
void AQuadcopter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuadcopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

