// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsGameInstance.h"

void UGamsGameInstance::Init()
{
  gams_current_world = GetWorld();
}

//void UGamsGameInstance::OnWorldChanged(
//  UWorld* OldWorld,
//  UWorld* NewWorld)
//{
//  gams_current_world = NewWorld;
//}

UWorld * gams_current_world (0);
float gams_delta_time (0.0);