// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsVehicle.h"

// Sets default values
AGamsVehicle::AGamsVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  move_forward = 0.0f;
  move_up = 0.0f;
  look_up = 0.0f;
}

// Called when the game starts or when spawned
void AGamsVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGamsVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

