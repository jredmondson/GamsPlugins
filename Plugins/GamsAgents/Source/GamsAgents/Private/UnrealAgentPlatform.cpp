
#include "UnrealAgentPlatform.h"
#include "Math/UnrealMathUtility.h"
#include "GamsAgentsLogs.h"
#include "GamsGameInstance.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"
#include "Math/Vector.h"
#include "GamsVehicle.h"
#include "GamsDjiPhantom.h"


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
    result = new UnrealAgentPlatform(knowledge, sensors, self, args);
  }

  return result;
}

// Constructor
UnrealAgentPlatform::UnrealAgentPlatform(
  madara::knowledge::KnowledgeBase * knowledge,
  gams::variables::Sensors * sensors,
  gams::variables::Self * self,
  const madara::knowledge::KnowledgeMap & args)
: gams::platforms::BasePlatform(knowledge, sensors, self)
{
  // as an example of what to do here, create a coverage sensor
  if (knowledge && sensors)
  {
    agent_prefix_ = self_->agent.prefix.c_str ();

    UE_LOG (LogUnrealAgentPlatform, Log,
      TEXT ("%s: constr: entering"),
      *agent_prefix_);

    // create a coverage sensor
    //gams::variables::Sensors::iterator it = sensors->find("coverage");
    //if (it == sensors->end()) // create coverage sensor
    //{
    //  // get origin
    //  gams::pose::Position origin(gams::pose::gps_frame());
    //  madara::knowledge::containers::NativeDoubleArray origin_container;
    //  origin_container.set_name("sensor.coverage.origin", *knowledge, 3);
    //  origin.from_container(origin_container);

    //  // establish sensor
    //  gams::variables::Sensor* coverage_sensor =
    //    new gams::variables::Sensor("coverage", knowledge, 2.5, origin);
    //  (*sensors)["coverage"] = coverage_sensor;
    //}

    //(*sensors_)["coverage"] = (*sensors)["coverage"];
    status_.init_vars(*knowledge, get_id());

    UE_LOG (LogUnrealAgentPlatform, Log,
      TEXT ("%s: constr: searching for relevant args"),
      *agent_prefix_);

    KnowledgeMap::const_iterator location = args.find("location");
    KnowledgeMap::const_iterator orientation = args.find("orientation");
    KnowledgeMap::const_iterator blueprint = args.find ("blueprint");
    //FString class_name;

    //UE_LOG (LogUnrealAgentPlatform, Log,
    //  TEXT ("%s: constr: checking for blueprint"),
    //  *agent_prefix_);

    //if (blueprint != args.end())
    //{
    //  if (blueprint->second.is_string_type() && blueprint->second == "random")
    //  {
    //    class_name = ANSI_TO_TCHAR(unreal_platforms[FMath::Rand() % 3].c_str());
    //  }
    //  else
    //  {
    //    class_name = ANSI_TO_TCHAR(blueprint->second.to_string().c_str());
    //  }
    //}
    //else
    //{
    //  class_name = ANSI_TO_TCHAR(unreal_platforms[FMath::Rand () % 3].c_str ());
    //}

    //UE_LOG (LogUnrealAgentPlatform, Log,
    //  TEXT("%s: selected agent class: %s"),
    //  *agent_prefix_, *class_name);

    knowledge::KnowledgeRecord initial_pose = knowledge->get(".initial_pose");

    is_created_ = knowledge->get(".osc.is_created").is_true();

    if (!initial_pose.is_array_type())
    {
      KnowledgeRecord::Integer id = self_->id.to_integer();

      float row = (float)((id / 10) % 10);
      float col = (float)(id % 10);
      float level = (float)(id / 100);

      // by default initialize agents to [.id, .id, .id]
      initial_pose.set_index(2, 50.0f + 100 * level);
      //initial_pose.set_index(1, -2200.0f + 440 * (*(self_->id) % 10));
      //initial_pose.set_index(0, -1200.0f + 240 * (*(self_->id) / 10));
      initial_pose.set_index(1, -2200.0f + 140 * col);
      initial_pose.set_index(0, -1200.0f + 140 * row);
    }
    else
    {
      double value = initial_pose.retrieve_index(0).to_double() * 100;
      initial_pose.set_index(0, value);
      value = initial_pose.retrieve_index(1).to_double() * 100;
      initial_pose.set_index(1, value);
      value = initial_pose.retrieve_index(2).to_double() * 100;
      initial_pose.set_index(2, value);
    }

    loiter_timeout_ = knowledge->get(".osc.loiter_timeout").to_double();

    respawn_timeout_ = knowledge->get(".osc.respawn_timeout").to_double();

    if (loiter_timeout_ >= 0 && loiter_timeout_ < 5)
    {
      loiter_timeout_ = 5;
    }
    
    last_move_.frame(get_frame());
    last_move_.x(0);
    last_move_.y(0);
    last_move_.z(0);

    last_orient_.frame(get_frame());
    last_orient_.rx(0);
    last_orient_.ry(0);
    last_orient_.rz(0);

    move_timer_.start();
    last_thrust_timer_.start();
    last_position_timer_.start();

    // need to call GetWorld()->SpawnActor(type, loc, rot, spawnparameters)
    // then save the actor into the platform class

    //UE_LOG (LogUnrealAgentPlatform, Log,
    //  TEXT ("%s: spawning %s from global world object"),
    //  *agent_prefix_, *class_name);

    world_ = gams_current_world;

    if (gams_current_world != 0)
    {
      UE_LOG (LogUnrealAgentPlatform, Log,
        TEXT ("%s: global world object is not null"),
        *agent_prefix_);

      //UGamsAssetManager * manager = dynamic_cast<UGamsAssetManager*>(
      //  UAssetManager::Get());

      FVector ue_location(
        float(initial_pose.retrieve_index(0).to_double()),
        float(initial_pose.retrieve_index(1).to_double()),
        float(initial_pose.retrieve_index(2).to_double()));
      FRotator ue_orientation(0.0f, 0.0f, 0.0f);

      FActorSpawnParameters spawn_parameters;
      spawn_parameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

      UE_LOG(LogUnrealAgentPlatform, Log,
        TEXT("%s: spawning actor at %s, rotation=%s."),
        *agent_prefix_, *ue_location.ToString(), *ue_orientation.ToString());

      UClass * actor_class =
        platform_classes[FMath::Rand() % platform_classes.size()];

      actor_ = gams_current_world->SpawnActor<AGamsVehicle>(
        actor_class,
        ue_location, ue_orientation,
        spawn_parameters);

      if (actor_)
      {
        UE_LOG(LogUnrealAgentPlatform, Log,
          TEXT("%s: SUCCESS: actor spawned at %s, rotation=%s."),
          *agent_prefix_, *ue_location.ToString(), *ue_orientation.ToString());

        max_speed_ = actor_->max_speed;
        acceleration_ = actor_->acceleration;

        actor_->init_knowledge(gams_game_instance->kb, self_->agent.prefix);

        //max_speed_ = 200.0f;
        //acceleration_ = 500.0f;
      }
      else
      {
        UE_LOG(LogUnrealAgentPlatform, Warning,
          TEXT("%s: FAILED: actor spawn returned null."),
          *agent_prefix_);
      }

      //UBlueprint * actor_object = LoadObject<UBlueprint> (NULL, *class_name, NULL, LOAD_None, NULL);

      //if (actor_object != 0)
      //{
      //  UE_LOG(LogUnrealAgentPlatform, Log,
      //    TEXT("%s: actor class lookup %s succeeded! Actor can be spawned."),
      //    *agent_prefix_, *class_name);

      //}
      //else
      //{
      //  UE_LOG (LogUnrealAgentPlatform, Warning,
      //    TEXT ("%s: actor class lookup %s failed. No actor can be spawned."),
      //    *agent_prefix_, *class_name);
      //}
    }
    else
    {
      UE_LOG (LogUnrealAgentPlatform, Warning,
        TEXT ("%s: global world object is null. No actor can be spawned."),
        *agent_prefix_);
    }

    status_.movement_available = 1;
  }
}


// Destructor
UnrealAgentPlatform::~UnrealAgentPlatform()
{
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
    TEXT("%s: UnrealAgentPlatform::calculate_diff: loc=[%s] to tar=[%s]"),
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
    TEXT("%s: UnrealAgentPlatform::calculate_diff: rot=[%s] to tar=[%s]"),
    *agent_prefix_, *current_str, *target_str);

  finished = target.approximately_equal(current, 1.0f);

  // convert the distance vector to unreal engine unit (centimeters)
  diff_rotator.Roll = (target.rx() - current.rx()) * 100;
  diff_rotator.Pitch = (target.ry() - current.ry()) * 100;
  diff_rotator.Yaw = (target.rz() - current.rz()) * 100;
}

void UnrealAgentPlatform::calculate_delta(
  const FVector & total_diff, FVector & local_diff,
  float speed, float delta_time)
{
  // check if someone is supplying negative speed and make it positive
  if (speed < 0)
  {
    speed = -speed;
  }

  float max_distance = speed * delta_time;
  float x_abs = std::abs(total_diff.X);
  float y_abs = std::abs(total_diff.Y);
  float z_abs = std::abs(total_diff.Z);

  local_diff.X = std::min<float>(max_distance, x_abs);
  local_diff.Y = std::min<float>(max_distance, y_abs);
  local_diff.Z = std::min<float>(max_distance, z_abs);

  if (total_diff.X < 0)
  {
    local_diff.X = -local_diff.X;
  }

  if (total_diff.Y < 0)
  {
    local_diff.Y = -local_diff.Y;
  }

  if (total_diff.Z < 0)
  {
    local_diff.Z = -local_diff.Z;
  }

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s: UnrealAgentPlatform::calculate_delta: total=[%s] to tar=[%s]"
         " with speed=%f and time=%f"),
    *agent_prefix_, *(total_diff.ToString()), *(local_diff.ToString()),
      speed, delta_time);
}

void UnrealAgentPlatform::calculate_delta(
  const FRotator & total_diff, FRotator & local_diff,
  float speed, float delta_time)
{
  // this is a rotation, not a drag race!
  if (speed > 180)
  {
    speed = 180;
  }

  float max_distance = speed * delta_time;
  float roll_abs = std::abs(total_diff.Roll);
  float pitch_abs = std::abs(total_diff.Pitch);
  float yaw_abs = std::abs(total_diff.Yaw);

  local_diff.Roll = std::min<float>(max_distance, roll_abs);
  local_diff.Pitch = std::min<float>(max_distance, pitch_abs);
  local_diff.Yaw = std::min<float>(max_distance, yaw_abs);

  if (total_diff.Roll < 0)
  {
    local_diff.Roll = -local_diff.Roll;
  }

  if (total_diff.Pitch < 0)
  {
    local_diff.Pitch = -local_diff.Pitch;
  }

  if (total_diff.Yaw < 0)
  {
    local_diff.Yaw = -local_diff.Yaw;
  }

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s: UnrealAgentPlatform::calculate_delta: total=[%s] to tar=[%s]"
      " with speed=%f and time=%f"),
      *agent_prefix_, *(total_diff.ToString()), *(local_diff.ToString()),
      speed, delta_time);
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
    TEXT("%s: UnrealAgentPlatform::calculate_thrust: thrust is [%s]"),
    *agent_prefix_, *difference_str);

  return difference;
}

// Polls the sensor environment for useful information. Required.
int
UnrealAgentPlatform::sense(void)
{
  // need to get actor position and rotation
  // remember to convert to meters by dividing location parameters by 100
  // as the UE system will return centimeters.

  if (world_ == gams_current_world)
  {
    gams::pose::Position loc(get_frame());
    gams::pose::Rotation rot(get_frame());

    ue_location_ = actor_->GetActorLocation();
    ue_orientation_ = actor_->GetActorRotation();
    FString is_init = FString(actor_->IsActorInitialized() ? "Init" : "Uninit");
    FString in_world = FString(world_ == gams_current_world ? "Yes" : "No");

    // UE provides location in centimeters. Convert to meters.
    loc.x(ue_location_.X / 100);
    loc.y(ue_location_.Y / 100);
    loc.z(ue_location_.Z / 100);

    rot.rx(ue_orientation_.Roll);
    rot.ry(ue_orientation_.Pitch);
    rot.rz(ue_orientation_.Yaw);

    loc.to_container(self_->agent.location);
    rot.to_container(self_->agent.orientation);
    loc.to_container(actor_->location);
    loc.to_container(actor_->orientation);

    FString loc_str(loc.to_string().c_str());
    FString orient_str(rot.to_string().c_str());

    UE_LOG(LogUnrealAgentPlatform, Log,
      TEXT("%s: UnrealAgentPlatform::sense: location=[%s], orientation=[%s]"
        ", init=%s, world=%s"),
      *agent_prefix_, *loc_str, *orient_str, *is_init, *in_world);
  }

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

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s: UnrealAgentPlatform::move: beginning move to position"),
    *agent_prefix_);

  if (world_ == gams_current_world)
  {
    // convert from input reference frame to vrep reference frame, if necessary
    gams::pose::Position new_target(get_frame(), target);

    // are we moving to a new location? If so, start an acceleration timer
    if (!last_move_.approximately_equal(new_target, 0.1))
    {
      move_timer_.start();
      // update variables
      gams::platforms::BasePlatform::move(target, bounds);
      new_target.to_container(actor_->dest);
      last_move_ = new_target;

      last_ue_target_location_.X = new_target.x() * 100;
      last_ue_target_location_.Y = new_target.y() * 100;
      last_ue_target_location_.Z = new_target.z() * 100;
    }

    //gams::pose::Position cur_loc = get_location();

    //bool finished = false;

    //FVector diff_location, delta_location;
    //FRotator diff_rotator, delta_rotator;

    //calculate_diff(cur_loc, new_target, diff_location, finished);
    //calculate_delta(diff_location, delta_location, max_speed_, gams_delta_time);

    //actor_->AddActorWorldOffset(delta_location, false, nullptr, ETeleportType::TeleportPhysics);

    if (ue_location_.Equals(last_ue_target_location_, 1.0f))
    {
      UE_LOG(LogUnrealAgentPlatform, Log,
        TEXT("%s: UnrealAgentPlatform::move: arrived at target."),
        *agent_prefix_);

      result = gams::platforms::PLATFORM_ARRIVED;
    }
    else
    {
      //FVector next_location;

      //next_location = FMath::VInterpTo(ue_location_, last_ue_target_location_,
      //  world_->DeltaTimeSeconds, 3);

      //calculate_delta(last_ue_target_location_ - ue_location_, next_location,
      //  max_speed_, world_->DeltaTimeSeconds);
      //next_location += ue_location_;

      //UE_LOG(LogUnrealAgentPlatform, Log,
      //  TEXT("%s: UnrealAgentPlatform::move: [%s] -> [%s]."
      //       " next=[%s] with delta_t=%f and max_speed=%f"),
      //  *agent_prefix_,
      //  *ue_location_.ToString(), *last_ue_target_location_.ToString(),
      //  *next_location.ToString(), world_->DeltaTimeSeconds, max_speed_);

      //actor_->SetActorLocation(next_location,
      //  false, nullptr, ETeleportType::None);
    }
  }

  return result;
}

int
UnrealAgentPlatform::orient(const gams::pose::Orientation & target,
        const gams::pose::OrientationBounds &bounds)
{
  // update variables
  gams::platforms::BasePlatform::orient(target, bounds);
  int result = gams::platforms::PLATFORM_MOVING;

  UE_LOG(LogUnrealAgentPlatform, Log,
    TEXT("%s: UnrealAgentPlatform::move: beginning orient/rotate"),
    *agent_prefix_);

  if (world_ == gams_current_world)
  {
    // convert from input reference frame to vrep reference frame, if necessary
    gams::pose::Orientation new_target(get_frame(), target);
    gams::pose::Orientation cur_orient = get_orientation();

    bool finished = false;
    FRotator diff_rotator, delta_rotator;

    calculate_diff(cur_orient, new_target, diff_rotator, finished);
    calculate_delta(diff_rotator, delta_rotator, 180.0f, gams_delta_time);

    actor_->AddActorWorldRotation(delta_rotator);

    if (finished)
    {
      UE_LOG(LogUnrealAgentPlatform, Log,
        TEXT("%s: UnrealAgentPlatform::orient: arrived at orientation."),
        *agent_prefix_);

      result = gams::platforms::PLATFORM_ARRIVED;
    }

    last_orient_ = new_target;
  }

  return result;
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
}

void UnrealAgentPlatform::unload_platform_classes(void)
{
  platform_classes.clear();
}
