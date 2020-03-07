// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsAssetManager.h"
#include "GamsAgentsLogs.h"
#include "GamsVehicle.h"


const FPrimaryAssetType	UGamsAssetManager::VehicleType = TEXT("Vehicle");

UGamsAssetManager::UGamsAssetManager()
  : UAssetManager()
{

}
//UGamsAssetManager& UGamsAssetManager::Get()
//{
//  UGamsAssetManager* manager = Cast<UGamsAssetManager>(GEngine->AssetManager);
//
//  if (manager)
//  {
//    return *manager;
//  }
//  else
//  {
//    UE_LOG (LogActionRPG, Fatal, TEXT(
//      "Invalid AssetManager in DefaultEngine.ini. Use UGamsAssetManager!"));
//    return *NewObject<UGamsAssetManager>(); // never calls this
//  }
//}

void UGamsAssetManager::StartInitialLoading()
{
  Super::StartInitialLoading();

  //UAbilitySystemGlobals::Get().InitGlobalData();
}


AGamsVehicle* UGamsAssetManager::ForceLoadVehicle(
  const FPrimaryAssetId& asset_id, bool blog_warning)
{
  FSoftObjectPath path = GetPrimaryAssetPath(asset_id);

  // This does a synchronous load and may hitch
  AGamsVehicle* vehicle = Cast<AGamsVehicle>(path.TryLoad());

  if (blog_warning && vehicle == nullptr)
  {
    UE_LOG(LogGamsAssetManager, Warning,
      TEXT("Failed to load vehicle for identifier %s!"),
      *asset_id.ToString());
  }

  return vehicle;
}