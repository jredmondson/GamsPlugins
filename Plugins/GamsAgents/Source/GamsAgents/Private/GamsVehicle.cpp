// Fill out your copyright notice in the Description page of Project Settings.

#include "GamsVehicle.h"
#include "GamsGameInstance.h"

// Sets default values
AGamsVehicle::AGamsVehicle()
  : AActor(), 
  mesh(0), movement(0), can_fly (false), is_fixed_wing(false), is_jet(false)
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

void AGamsVehicle::init_knowledge(const std::string& prefix)
{
  init_knowledge(gams_game_instance->kb, prefix);
}

void AGamsVehicle::animate(float delta_time)
{
}

void AGamsVehicle::init_knowledge(
  madara::knowledge::KnowledgeBase & knowledge, const std::string & prefix)
{
  agent_prefix = prefix;

  // initialize the variable containers
  location.set_name(prefix + ".location", knowledge, 3);
  orientation.set_name(prefix + ".orientation", knowledge, 3);
  source.set_name(prefix + ".source", knowledge, 3);
  source_orientation.set_name(prefix + ".source_orientation", knowledge, 3);
  dest_orientation.set_name(prefix + ".dest_orientation", knowledge, 3);
  dest.set_name(prefix + ".dest", knowledge, 3);
  velocity.set_name(prefix + ".velocity", knowledge, 3);
}

