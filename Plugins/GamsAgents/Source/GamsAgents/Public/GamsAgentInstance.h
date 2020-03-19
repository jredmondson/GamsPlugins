#pragma once

#include <string>
#include <sstream>

#include "Math/Rotator.h"
#include "Math/TransformNonVectorized.h"
#include "MadaraUnrealUtility.h"

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

  // initialize the instance for the agent prefix in the kb
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

  inline void init(uint32 i, madara::knowledge::KnowledgeBase & kb)
  {
    std::stringstream buffer;
    buffer << "agent.";
    buffer << i;

    init(buffer.str(), kb);
  }

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
  
  // the platform index
  uint32 platform_index;

  // the instance within the platform index
  uint32 instance;

  // by default, should be set by agent actor. Can be overridden by user.
  float platform_speed;
};