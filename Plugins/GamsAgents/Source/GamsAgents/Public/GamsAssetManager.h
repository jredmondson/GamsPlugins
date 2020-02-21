// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GamsAssetManager.generated.h"

class AGamsVehicle;

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API UGamsAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
  // Constructor and overrides
  UGamsAssetManager () {}
  virtual void StartInitialLoading () override;

  /** Static types for items */
  static const FPrimaryAssetType VehicleType;

  /** Returns the current AssetManager object */
  //static UGamsAssetManager& Get();

  /**
   * Synchronously loads a GamsVehicle subclass
   *
   * @param asset_id The asset identifier to load
   * @param blog_warning If true, log a warning if load failed
   */
  AGamsVehicle* ForceLoadVehicle(
    const FPrimaryAssetId& asset_id, bool blog_warning = true);
};
