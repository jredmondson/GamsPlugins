// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsMulticontroller.h"

// Sets default values
AGamsMulticontroller::AGamsMulticontroller()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  last_controllers = num_controllers = 1;
  last_platform = platform = TEXT("null");
  last_hertz = 1.0f;
  controller_hertz = 1.0f;
}

// Called when the game starts or when spawned
void AGamsMulticontroller::BeginPlay()
{
  Super::BeginPlay();
  

}

// Called every frame
void AGamsMulticontroller::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (num_controllers != last_controllers && num_controllers >= 0)
  {
    controller.resize((size_t)num_controllers);
    last_controllers = num_controllers;
  }

  if (platform != last_platform && platform != "")
  {
    controller.init_platform(TCHAR_TO_UTF8(*platform));
    last_platform = platform;
  }

  controller.run_once();
}

