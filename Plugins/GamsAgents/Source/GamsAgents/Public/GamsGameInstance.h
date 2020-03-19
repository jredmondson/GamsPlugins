// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GamsIncludes.h"

#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4103)
#pragma warning(disable:4191)
#pragma warning(disable:4457)
#pragma warning(disable:4458)
#pragma warning(disable:4459)
#pragma warning(disable:4530)
#pragma warning(disable:4577)
#pragma warning(disable:4583)
#pragma warning(disable:4582)
#pragma warning(disable:4668)
#pragma warning(disable:4996)
#include "madara/threads/Threader.h"
#include "madara/knowledge/containers/Integer.h"
#pragma warning(pop)

#include "Engine/EngineTypes.h"

#include "GamsGameInstance.generated.h"

class UnrealAgentPlatformFactory;
class AGamsAgentManager;

/**
 * 
 */
UCLASS()
class GAMSAGENTS_API UGamsGameInstance : public UGameInstance
{
  GENERATED_BODY()

public:

  virtual void Init() override;

  virtual void Shutdown() override;

  void OnPreLoadMap(const FString& map_name);

  void OnPostLoadMap(UWorld * new_world);

  void GameRun();

  gams::controllers::Multicontroller controller;

  madara::knowledge::KnowledgeBase kb;

  madara::transport::QoSTransportSettings transport_settings;

  bool should_animate;

  UPROPERTY(VisibleAnywhere, Category = Map)
    float minimap_height;

  UFUNCTION(BlueprintPure, Category = Loading)
    float LoadingPercentage() const;

  madara::knowledge::containers::Integer  agents_loaded;

  madara::knowledge::containers::Integer  swarm_size;

  bool enable_collisions;

  ECollisionEnabled::Type collision_type;
  
  FString platform_type;

  bool override_speed = false;

  float platform_speed = 500.0f;

private:

  UnrealAgentPlatformFactory * agent_factory_;

  madara::threads::Threader threader_;

  FTimerHandle run_timer_handler_;

  float last_send_time_;
  float end_loop_time_ = -1.0f;

  double controller_hz = 2.0f;

  TArray<FString> filecontents_;

  AGamsAgentManager * manager_;
};

extern UWorld* gams_current_world;
extern float gams_delta_time;
extern UGamsGameInstance* gams_game_instance;
