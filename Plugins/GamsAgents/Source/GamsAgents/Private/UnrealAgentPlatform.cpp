
#include "UnrealAgentPlatform.h"
#include "Math/UnrealMathUtility.h"
#include "GamsAgentsLogs.h"
#include "GamsGameInstance.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"
#include "Math/Vector.h"
#include "GamsVehicle.h"
#include "GamsDjiPhantom.h"
#include "GamsDjiMavic.h"
#include "GamsArDrone.h"
#include "GamsF16.h"
#include "MadaraUnrealUtility.h"
#include <algorithm>


#pragma warning(push)
#pragma warning(disable:4005)
#pragma warning(disable:4103)
#pragma warning(disable:4191)
#pragma warning(disable:4244)
#pragma warning(disable:4245)
#pragma warning(disable:4251)
#pragma warning(disable:4457)
#pragma warning(disable:4458)
#pragma warning(disable:4459)
#pragma warning(disable:4530)
#pragma warning(disable:4577)
#pragma warning(disable:4583)
#pragma warning(disable:4582)
#pragma warning(disable:4668)
#pragma warning(disable:4996)
#include <algorithm>
#include <math.h>
#include <sstream>

#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "madara/utility/Utility.h"
#include "gams/pose/Pose.h"
#pragma warning(pop)

namespace knowledge = madara::knowledge;
typedef knowledge::KnowledgeRecord  KnowledgeRecord;
typedef knowledge::KnowledgeMap KnowledgeMap;

const std::vector <std::string> unreal_platforms = {
"/Game/Quadcopters/Blueprints/BP_Quadcopter_A.BP_Quadcopter_A",
"/Game/Quadcopters/Blueprints/BP_Quadcopter_B.BP_Quadcopter_B",
"/Game/Quadcopters/Blueprints/BP_Quadcopter_C.BP_Quadcopter_C"
};

//const std::vector <UClass *> platform_classes = {
//  AGamsDjiPhantom::StaticClass()
//};

std::vector <UClass *> platform_classes;

std::vector <UClass *> quad_platform_classes;

std::vector <UClass *> jet_platform_classes;

UnrealAgentPlatformFactory::UnrealAgentPlatformFactory()
{
}

UnrealAgentPlatformFactory::~UnrealAgentPlatformFactory()
{
}

// factory class for creating a UnrealAgentPlatform 
gams::platforms::BasePlatform *
UnrealAgentPlatformFactory::create(
        const madara::knowledge::KnowledgeMap & args,
        madara::knowledge::KnowledgeBase * knowledge,
        gams::variables::Sensors * sensors,
        gams::variables::Platforms * ,
        gams::variables::Self * self)
{
  gams::platforms::BasePlatform * result(0);

  if (knowledge && sensors && self)
  {
    result = new UnrealAgentPlatform(knowledge, sensors, self, platform_type);
  }

  return result;
}

// Constructor
UnrealAgentPlatform::UnrealAgentPlatform(
  madara::knowledge::KnowledgeBase * kb,
  gams::variables::Sensors * sensors,
  gams::variables::Self * self,
  const FString & type)
: gams::platforms::BasePlatform(kb, sensors, self), platform_type(type)
{
  // as an example of what to do here, create a coverage sensor
  if (kb && sensors)
  {
    agent_prefix_ = self_->agent.prefix.c_str ();

    UE_LOG (LogUnrealAgentPlatform, Log,
      TEXT ("%s::constr: entering"),
      *agent_prefix_);

    status_.init_vars(*kb, get_id());

    madara::knowledge::containers::NativeDoubleVector init(
      ".initial_pose", *kb);
    
    if (!init.to_record().is_array_type())
    {
      // set a default position for agents if no initial_pose exists
      KnowledgeRecord::Integer id = self_->id.to_integer();

      float row = (float)((id / 10) % 10);
      float col = (float)(id % 10);
      float level = (float)(id / 100);

      init.set(2, 0.5f + 1.0f * level);
      init.set(1, -22.0f + 1.4f * col);
      init.set(0, -12.0f + 1.4f * row);
    }
    
    char buf[128];

    madara::utility::to_c_str(init.to_record(), (char*)buf, 128);

    FString init_s = buf;
  
    UE_LOG (LogUnrealAgentPlatform, Log,
      TEXT ("%s::constr: init pos=[%s]"),
      *agent_prefix_, *init_s);

    gams::pose::Pose init_pose(get_frame());
    init_pose.from_container(init);

    init_agent(init_pose);

    is_created_ = true;

    status_.movement_available = 1;
  } // (knowledge && sensors)

  if (gams_game_instance)
  {
    ++(gams_game_instance->agents_loaded);
  }
}

// Destructor
UnrealAgentPlatform::~UnrealAgentPlatform()
{
}

void
UnrealAgentPlatform::init_agent(const gams::pose::Pose & init_pose)
{
  location_.set_name(
    self_->agent.prefix + ".location", gams_game_instance->kb);
  orientation_.set_name(
    self_->agent.prefix + ".orientation", gams_game_instance->kb);
  source_.set_name(
    self_->agent.prefix + ".source", gams_game_instance->kb);
  dest_.set_name(
    self_->agent.prefix + ".dest", gams_game_instance->kb);
  home_.set_name(
    self_->agent.prefix + ".home", gams_game_instance->kb);

  gams::pose::Position location(init_pose);
  gams::pose::Orientation orientation(init_pose);

  // update our local KB and the game KB
  location.to_container(self_->agent.location);
  location.to_container(location_);
  orientation.to_container(self_->agent.orientation);
  orientation.to_container(orientation_);
  location.to_container(source_);
  location.to_container(dest_);

  if (home_.to_record().is_array_type())
  {
    location.to_container(home_);
  }
}

void
UnrealAgentPlatform::calculate_diff(
  const gams::pose::Position& current, const gams::pose::Position& target,
  FVector& diff_location,
  bool& finished)
{
  finished = true;

  last_thrust_timer_.start();

  FString current_str(current.to_string().c_str());
  FString target_str(target.to_string().c_str());

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::calculate_diff: loc=[%s] to tar=[%s]"),
    *agent_prefix_, *current_str, *target_str);

  finished = target.approximately_equal(current, 0.2);

  // convert the distance vector to unreal engine unit (centimeters)
  diff_location.X = (target.x() - current.x()) * 100;
  diff_location.Y = (target.y() - current.y()) * 100;
  diff_location.Z = (target.z() - current.z()) * 100;
}

void
UnrealAgentPlatform::calculate_diff(
  const gams::pose::Orientation& current, const gams::pose::Orientation& target,
  FRotator& diff_rotator,
  bool& finished)
{
  finished = true;

  last_thrust_timer_.start();
  FString current_str(current.to_string().c_str());
  FString target_str(target.to_string().c_str());

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::calculate_diff: rot=[%s] to tar=[%s]"),
    *agent_prefix_, *current_str, *target_str);

  finished = target.approximately_equal(current, 1.0f);

  // convert the distance vector to unreal engine unit (centimeters)
  diff_rotator.Roll = (target.rx() - current.rx()) * 100;
  diff_rotator.Pitch = (target.ry() - current.ry()) * 100;
  diff_rotator.Yaw = (target.rz() - current.rz()) * 100;
}

std::vector<double>
UnrealAgentPlatform::calculate_thrust(
  const gams::pose::Position& current, const gams::pose::Position& target,
  bool& finished)
{
  finished = true;
  std::vector<double> difference(std::min(current.size(), target.size()));
  
  last_thrust_timer_.start();

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_MAJOR,
    "UnrealAgentPlatform::calculate_thrust: " \
    "%s: current=[%s], target=[%s]\n",
    self_->agent.prefix.c_str(),
    current.to_string().c_str(), target.to_string().c_str());

  for (size_t i = 0; i < difference.size(); ++i)
  {
    difference[i] = target.get(i) - current.get(i);

    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_TRACE,
      "UnrealAgentPlatform::calculate_thrust: " \
      "%s: difference[%zu]=[%f]\n",
      self_->agent.prefix.c_str(), i, difference[i]);

    if (difference[i] <= 0.3 && difference[i] >= -0.3)
    {
      difference[i] = 0;
    }
    else if (difference[i] <= 0.5 && difference[i] >= -0.5)
    {
      difference[i] /= fabs(difference[i]);
      difference[i] *= 0.25;
      finished ? finished = false : 0;
    }
    else if (difference[i] <= 1.5 && difference[i] >= -1.5)
    {
      difference[i] /= fabs(difference[i]);
      difference[i] *= 0.5;
      finished ? finished = false : 0;
    }
    else
    {
      difference[i] /= fabs(difference[i]);
      finished ? finished = false : 0;
    }
  }
  
  madara::knowledge::KnowledgeRecord record(difference);
  FString difference_str(record.to_string().c_str());

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::calculate_thrust: thrust is [%s]"),
    *agent_prefix_, *difference_str);

  return difference;
}

// Polls the sensor environment for useful information. Required.
int
UnrealAgentPlatform::sense(void)
{
  size_t buf_size(0);
  char buf[128];

  madara::utility::to_c_str(location_.to_record(), (char*)buf, 128);

  FString cur_loc_s = buf;
  
  madara::utility::to_c_str(orientation_.to_record(), (char*)buf, 128);

  FString cur_orient_s = buf;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::sense: copying loc=%s and orient=%s from global kb."),
    *agent_prefix_, *cur_loc_s, *cur_orient_s);

  // copy pose information from game kb
  //location_.transfer_to(self_->agent.location);
  //orientation_.transfer_to(self_->agent.orientation);

  knowledge::KnowledgeRecord location_r = location_.to_record();
  knowledge::KnowledgeRecord orient_r = orientation_.to_record();
  
  self_->agent.location.set(2, location_r.retrieve_index(2).to_double());
  self_->agent.location.set(1, location_r.retrieve_index(1).to_double());
  self_->agent.location.set(0, location_r.retrieve_index(0).to_double());
  
  self_->agent.orientation.set(2, orient_r.retrieve_index(2).to_double());
  self_->agent.orientation.set(1, orient_r.retrieve_index(1).to_double());
  self_->agent.orientation.set(0, orient_r.retrieve_index(0).to_double());

  return gams::platforms::PLATFORM_OK;
}


// Analyzes platform information. Required.
int
UnrealAgentPlatform::analyze(void)
{
  return gams::platforms::PLATFORM_OK;
}


// Gets the name of the platform. Required.
std::string
UnrealAgentPlatform::get_name(void) const
{
  return "UnrealAgentPlatform";
}


// Gets the unique identifier of the platform.
std::string
UnrealAgentPlatform::get_id(void) const
{
  return "UnrealAgentPlatform";
}


// Gets the position accuracy in meters. Optional.
double
UnrealAgentPlatform::get_accuracy(void) const
{
  return 0.3;
}


// Gets sensor radius. Optional.
double
UnrealAgentPlatform::get_min_sensor_range(void) const
{
  // should be in square meters
  return 1.0;
}

// Gets move speed. Optional.
double
UnrealAgentPlatform::get_move_speed(void) const
{
  // should be in meters/s
  return 1.0;
}

// Instructs the agent to return home. Optional.
int
UnrealAgentPlatform::home(void)
{
  /**
   * Movement functions work in a polling manner. In a real implementation,
   * we would want to check a finite state machine, external thread or
   * platform status to determine what to return. For now, we will simply
   * return that we are in the process of moving to the final pose.
   **/
  return gams::platforms::PLATFORM_IN_PROGRESS;
}


// Instructs the agent to take off. Optional.
int
UnrealAgentPlatform::land(void)
{
  return gams::platforms::PLATFORM_OK;
}

int
UnrealAgentPlatform::move(const gams::pose::Position & target,
  const gams::pose::PositionBounds & bounds)
{
  int result = gams::platforms::PLATFORM_MOVING;
  FString target_s = target.to_string().c_str();
  FString last_move_s = last_move_.to_string().c_str();

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::move: move to position [%s]"),
    *agent_prefix_, *target_s);

  // convert from input reference frame to vrep reference frame, if necessary
  gams::pose::Position new_target(get_frame(), target);
  gams::pose::Position cur_loc(get_frame());
  cur_loc.from_container(self_->agent.location);
  FString cur_loc_s = cur_loc.to_string().c_str();

  // are we moving to a new location? If so, start an acceleration timer
  if (!last_move_.approximately_equal(new_target, 0.1))
  {
    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("%s::move: last_move [%s] != target [%s]."
        " New move."),
      *agent_prefix_, *last_move_s, *target_s);

    move_timer_.start();
    // update variables
    gams::platforms::BasePlatform::move(target, bounds);
    new_target.to_container(dest_);
    cur_loc.to_container(source_);

    last_move_ = new_target;
  }

  if (cur_loc.approximately_equal(new_target, 0.5f))
  {
    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("%s::move: arrived at target [%s]."),
      *agent_prefix_, *target_s);

    result = gams::platforms::PLATFORM_ARRIVED;
  }
  else
  {
    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("%s::move: cur_loc[%s] != target."
        " Still moving to target [%s]."),
      *agent_prefix_, *cur_loc_s, *target_s);
  }

  return result;
}

int
UnrealAgentPlatform::orient(const gams::pose::Orientation & target,
        const gams::pose::OrientationBounds &bounds)
{
  // update variables
  int result = gams::platforms::PLATFORM_MOVING;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s::orient: beginning orient/rotate"),
    *agent_prefix_);

  // convert from input reference frame to vrep reference frame, if necessary
  gams::pose::Orientation new_target(get_frame(), target);
  gams::pose::Orientation cur_orient = get_orientation();

  // are we moving to a new location? If so, start an acceleration timer
  if (!last_orient_.approximately_equal(new_target, 1.0f))
  {
    // update variables
    gams::platforms::BasePlatform::orient(target, bounds);
    new_target.to_container(dest_orientation_);
    cur_orient.to_container(source_orientation_);
    last_orient_ = new_target;
  }

  if (cur_orient.approximately_equal(new_target, 1.0f))
  {
    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("%s::orient: arrived at target."),
      *agent_prefix_);

    result = gams::platforms::PLATFORM_ARRIVED;
  }

  return gams::platforms::PLATFORM_ARRIVED;
}

// Pauses movement, keeps source and dest at current values. Optional.
void
UnrealAgentPlatform::pause_move(void)
{
}


// Set move speed. Optional.
void
UnrealAgentPlatform::set_move_speed(const double& )
{
}


// Stops movement, resetting source and dest to current location. Optional.
void
UnrealAgentPlatform::stop_move(void)
{
}

// Instructs the agent to take off. Optional.
int
UnrealAgentPlatform::takeoff(void)
{
  return gams::platforms::PLATFORM_OK;
}

const gams::pose::ReferenceFrame &
UnrealAgentPlatform::get_frame(void) const
{
  // For cartesian, replace with gams::pose::default_frame()
  return gams::pose::default_frame();
}

void UnrealAgentPlatform::load_platform_classes(void)
{
  platform_classes.push_back(AGamsDjiPhantom::StaticClass());
  platform_classes.push_back(AGamsDjiMavic::StaticClass());
  platform_classes.push_back(AGamsArDrone::StaticClass());
  platform_classes.push_back(AGamsF16::StaticClass());

  quad_platform_classes.push_back(AGamsDjiPhantom::StaticClass());
  quad_platform_classes.push_back(AGamsDjiMavic::StaticClass());
  quad_platform_classes.push_back(AGamsArDrone::StaticClass());

  jet_platform_classes.push_back(AGamsF16::StaticClass());
}

void UnrealAgentPlatform::unload_platform_classes(void)
{
  platform_classes.clear();
}
