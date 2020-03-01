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
#pragma warning(pop)

#include "GamsGameInstance.generated.h"

class UnrealAgentPlatformFactory;

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

  void OnPostLoadMap(UWorld * new_world);

  void GameRun();

  gams::controllers::Multicontroller controller;

  madara::knowledge::KnowledgeBase kb;

  madara::transport::QoSTransportSettings transport_settings;

  bool should_animate;

private:

  UnrealAgentPlatformFactory * agent_factory_;

  madara::threads::Threader threader_;

  FTimerHandle run_timer_handler_;

  float last_send_time_;
};

extern UWorld* gams_current_world;
extern float gams_delta_time;
extern UGamsGameInstance* gams_game_instance;
