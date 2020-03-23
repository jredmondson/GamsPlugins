// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Containers/Array.h"
#include "Math/TransformNonVectorized.h"
#include "UObject/UObjectGlobals.h"
#include "GamsAgentInstance.h"

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
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/knowledge/containers/Integer.h"
#pragma warning(pop)

#include "GamsAgentManager.generated.h"

class UStaticMeshComponent;
class UMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class AGamsVehicle;

UCLASS()
class GAMSAGENTS_API AGamsAgentManager : public AActor
{
	GENERATED_BODY()
	
public:

	AGamsAgentManager();
  
	virtual ~AGamsAgentManager();
  
  /**
   * Called on agent initialization
   **/
  virtual void BeginPlay() override;
  
  /**
   * Clears the agent instances
   **/
  void clear(void);
  
  /**
   * Destroys a managed agent
   * @param  id   Destroys the managed agent instance
   **/
  void destroy(uint32 id);
  
  /**
   * Initializes the manager from the knowledge base information. This method
   * reads swarm.size and spawns all associated managed agent instances
   * @param  kb   the knowledge base to lock for thread-safe reading
   **/
  void read(madara::knowledge::KnowledgeBase & kb);
  
  /**
   * Updates the source and destinations being set by the GAMS multicontroller.
   * This allows very controlled instances of locking the KB and prevents us
   * constantly blocking on the KB mutex and frees us to spin without blocking
   * @param  kb   the knowledge base to lock for thread-safe reading
   **/
  void read_source_dest(madara::knowledge::KnowledgeBase & kb);
  
  /**
   * Renders all agent transforms
   **/
  void render(void);
  
  /**
   * Returns the number of managed agent instances
   * @return  the number of managed agent instances
   **/
  uint32 size(void);

  /**
   * Updates all managed agent instances to move over the time delta
   * @param  delta_time   the change in time expected for movement
   **/
  void update(float delta_time);
  
  /**
   * Writes location and orientation back to the MADARA KB. This should not
   * be done every game loop in order to prevent unnecessary blocking.
   * @param  kb   the knowledge base to lock for thread-safe writing
   **/
  void write_location_orientation(madara::knowledge::KnowledgeBase & kb);

private:
  /**
   * agent.id to instance translations
   **/
  TMap<uint32, uint32> agent_id_to_manager_;
  
  /**
   * Platform type instance managers that do all of the core work of tracking
   * and updating platform instances and all subcomponents
   **/
  TArray<AGamsVehicle *> managers_;
  
  /**
   * Reference to swarm.size in the game knowledge base
   **/
  madara::knowledge::containers::Integer swarm_size_;

  /**
   * The platform.type argument, which currently is tied to the game instance
   * value, but which may one day be allowed to change dynamically at runtime
   **/
  FString platform_type_str_;

  /**
   * Gams Agent information (e.g., location, orientation, dest, etc.)
   **/
  TArray<GamsAgentInstance> gams_info_;
};
