// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsVehicle.h"

// Sets default values
AGamsVehicle::AGamsVehicle()
  : mesh(0), movement(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

