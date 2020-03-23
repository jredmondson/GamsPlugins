#pragma once

#include <string>
#include <sstream>

#include "Math/Rotator.h"
#include "Math/TransformNonVectorized.h"
#include "MadaraUnrealUtility.h"
#include "GamsAgentsLogs.h"

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
#include "madara/knowledge/KnowledgeBase.h"
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/knowledge/containers/Integer.h"
#pragma warning(pop)

/**
 *
 */
class  GamsAgentInstance
{
public:
  
  /**
   * Initializes the knowledge containers for the agent prefix
   **/
  inline void init(const std::string & prefix, madara::knowledge::KnowledgeBase & kb)
  {
    home.set_name(prefix + ".home", kb);
    location.set_name(prefix + ".location", kb);
    orientation.set_name(prefix + ".orientation", kb);
    source.set_name(prefix + ".source", kb);
    source_orientation.set_name(prefix + ".source_orientation", kb);
    dest.set_name(prefix + ".dest", kb);
    dest_orientation.set_name(prefix + ".dest_orientation", kb);
  }

  /**
   * Initializes the knowledge containers for the agent identifier
   **/
  inline void init(uint32 i, madara::knowledge::KnowledgeBase & kb)
  {
    std::stringstream buffer;
    buffer << "agent.";
    buffer << i;

    agent_id = i;

    init(buffer.str(), kb);
  }
  
  /**
   * Writes data from the location and orientation to the transform
   **/
  inline void write_to(FTransform & new_transform)
  {
    FVector translation;
    madara::utility::to_vector_multiply(location, translation);
    new_transform.SetLocation(translation);
    
    FRotator rotator;
    madara::utility::to_rotator(orientation, rotator);
    new_transform.SetRotation(rotator.Quaternion());
    new_transform.SetScale3D(FVector::OneVector);
  }

  inline void read_source_dest(void)
  {
    char buf[128];
    
    //UE_LOG(LogGamsVehicle, Log,
    //  TEXT("read_source_dest: id %d: instance %d:"
    //       " @instance:%d, @dest_v:%d"),
    //  (int)agent_id, (int64)this, (int64)&dest_v);

    madara::utility::to_c_str(source.to_record(), (char*)buf, 128);
    FString source_s = buf;
    
    madara::utility::to_c_str(dest.to_record(), (char*)buf, 128);
    FString dest_s = buf;

    UE_LOG(LogGamsAgentInstanceKbAction, Log,
      TEXT("read_source_dest: id %d: instance %d:"
           " kb: source=%s, dest=%s"),
        (int)agent_id, (int)instance, *source_s, *dest_s);

    madara::utility::to_vector_multiply(source, source_v);
    madara::utility::to_rotator(source_orientation, source_orient_r);
    
    madara::utility::to_vector_multiply(dest, dest_v);
    madara::utility::to_rotator(dest_orientation, dest_orient_r);

    UE_LOG(LogGamsAgentInstanceKbAction, Log,
      TEXT("read_source_dest: id %d: instance %d:"
           " local: source=%s, dest=%s"),
      (int)agent_id, (int)instance, *source_v.ToString(), *dest_v.ToString());
  }
  
  inline void write_location_orientation(void)
  {
    UE_LOG(LogGamsAgentInstanceKbAction, Log,
      TEXT("write_location_orientation: id %d: instance %d:"
           " local: loc=%s, ori=%s"),
      (int)agent_id, (int)instance,
      *transform.GetLocation().ToString(),
      *transform.GetRotation().Rotator().ToString());

    madara::utility::from_vector_multiply(transform.GetLocation(), location);
    madara::utility::from_rotator(
      transform.GetRotation().Rotator(), orientation);

    char buf[128];
    
    madara::utility::to_c_str(location.to_record(), (char*)buf, 128);
    FString location_s = buf;
    
    madara::utility::to_c_str(orientation.to_record(), (char*)buf, 128);
    FString orientation_s = buf;


    UE_LOG(LogGamsAgentInstanceKbAction, Log,
      TEXT("write_location_orientation: id %d: instance %d: kb:"
           " loc=%s, ori=%s"),
      (int)agent_id, (int)instance, *location_s, *orientation_s);
  }

  // communication variable with main game loop for agent home
  madara::knowledge::containers::NativeDoubleVector home;

  // communication variable with main game loop for agent location
  madara::knowledge::containers::NativeDoubleVector location;

  // communication variable with main game loop for agent orientation
  madara::knowledge::containers::NativeDoubleVector orientation;

  // communication variable with main game loop for agent source
  madara::knowledge::containers::NativeDoubleVector source;

  // communication variable with main game loop for agent source orientation
  madara::knowledge::containers::NativeDoubleVector source_orientation;

  // communication variable with main game loop for agent dest
  madara::knowledge::containers::NativeDoubleVector dest;

  // communication variable with main game loop for agent dest orientation
  madara::knowledge::containers::NativeDoubleVector dest_orientation;

  // fast access for current position, orientation and scale
  FTransform transform;
  
  // fast access for dest position
  FVector home_v;
  
  // fast access for dest position
  FVector dest_v;
  
  // fast access for dest position
  FRotator dest_orient_r;
  
  // fast access for dest position
  FVector source_v;
  
  // fast access for dest position
  FRotator source_orient_r;
  
  // the platform index
  uint32 platform_index;

  // the instance within the platform index
  uint32 instance;

  // the agent identifier
  uint32 agent_id;

  // by default, should be set by agent actor. Can be overridden by user.
  float platform_speed;
};