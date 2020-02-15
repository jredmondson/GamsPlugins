#include "UnrealAgentPlatform.h"


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

UnrealAgentPlatformFactory::UnrealAgentPlatformFactory(const std::string & type)
: type_(type)
{
  
}

UnrealAgentPlatformFactory::~UnrealAgentPlatformFactory()
{
  
}

// factory class for creating a UnrealAgentPlatform 
gams::platforms::BasePlatform *
UnrealAgentPlatformFactory::create(
        const madara::knowledge::KnowledgeMap &,
        madara::knowledge::KnowledgeBase * knowledge,
        gams::variables::Sensors * sensors,
        gams::variables::Platforms * ,
        gams::variables::Self * self)
{
  gams::platforms::BasePlatform * result(0);

  if (type_ != "quadcopter" &&
      type_ != "satellite")
  {
    type_ = "quadcopter";
  }
  
  if (knowledge && sensors && self)
  {
    result = new UnrealAgentPlatform(knowledge, sensors, self,
      type_);
  }

  return result;
}

// Constructor
UnrealAgentPlatform::UnrealAgentPlatform(
  madara::knowledge::KnowledgeBase * knowledge,
  gams::variables::Sensors * sensors,
  gams::variables::Self * self,
  const std::string & type)        
: gams::platforms::BasePlatform(knowledge, sensors, self),
  type_(type)
{
  // as an example of what to do here, create a coverage sensor
  if (knowledge && sensors)
  {
    // create a coverage sensor
    gams::variables::Sensors::iterator it = sensors->find("coverage");
    if (it == sensors->end()) // create coverage sensor
    {
      // get origin
      gams::pose::Position origin(gams::pose::gps_frame());
      madara::knowledge::containers::NativeDoubleArray origin_container;
      origin_container.set_name("sensor.coverage.origin", *knowledge, 3);
      origin.from_container(origin_container);

      // establish sensor
      gams::variables::Sensor* coverage_sensor =
        new gams::variables::Sensor("coverage", knowledge, 2.5, origin);
      (*sensors)["coverage"] = coverage_sensor;
    }

    (*sensors_)["coverage"] = (*sensors)["coverage"];
    status_.init_vars(*knowledge, get_id());
    
    build_prefixes();

    settings_.hosts.push_back(
      knowledge->get(".osc.local.endpoint").to_string());
    settings_.hosts.push_back(
      knowledge->get(".osc.server.endpoint").to_string());

    knowledge::KnowledgeRecord initial_pose = knowledge->get(".initial_pose");

    is_created_ = knowledge->get(".osc.is_created").is_true();

    if (!initial_pose.is_array_type())
    {
      // by default initialize agents to [.id, .id, .id]
      initial_pose.set_index(2, 200);
      initial_pose.set_index(1, *(self_->id) * 300);
      initial_pose.set_index(0, *(self_->id) * 300);
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

    status_.movement_available = 1;
  }
}


// Destructor
UnrealAgentPlatform::~UnrealAgentPlatform()
{
}

void
UnrealAgentPlatform::build_prefixes(void)
{
}

std::vector<double>
UnrealAgentPlatform::calculate_thrust(
  const gams::pose::Position & current, const gams::pose::Position & target,
  bool & finished)
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
    else if (difference[i] <= 1.5 && difference[i] >= -1,5)
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

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_MAJOR,
    "UnrealAgentPlatform::calculate_thrust: " \
    "%s: returning thrust of [%s]\n",
    self_->agent.prefix.c_str(),
    record.to_string().c_str());

  return difference;
}

// Polls the sensor environment for useful information. Required.
int
UnrealAgentPlatform::sense(void)
{
  // need to get actor position and rotation
  // remember to convert to meters by dividing location parameters by 100
  // as the UE system will return centimeters.

  // convert osc order to the frame order
  gams::pose::Position loc (get_frame());
  gams::pose::Rotation rot (get_frame());
  //loc.from_array (value.second.to_doubles ());

  // The UnrealEngine provides us with centimeters. Convert to meters.
  //loc.x (loc.x () / 100);
  //loc.y (loc.y () / 100);
  //loc.z (loc.z () / 100);

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
  return 0.5;
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
  // update variables
  gams::platforms::BasePlatform::move(target, bounds);
  int result = gams::platforms::PLATFORM_MOVING;

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_TRACE,
    "UnrealAgentPlatform::move:" \
    " %s: requested target \"%f,%f,%f\"\n",
    self_->agent.prefix.c_str(),
    target.x(), target.y(), target.z());

  // convert from input reference frame to vrep reference frame, if necessary
  gams::pose::Position new_target(get_frame(), target);

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_TRACE,
    "UnrealAgentPlatform::move:" \
    " %s: target \"%f,%f,%f\"\n",
    self_->agent.prefix.c_str(),
    new_target.x(), new_target.y(), new_target.z());

  // are we moving to a new location? If so, start an acceleration timer
  if (!last_move_.approximately_equal(new_target, 5.0))
  {
    move_timer_.start();
    last_move_ = new_target;
  }

  gams::pose::Position cur_loc = get_location();

  std::vector<double> xy_velocity;
  std::vector<double> z_velocity;

  // if (bounds.check_position(cur_loc, new_target))
  // quick hack to check position is within 0.5 meters

  bool finished = false;
  xy_velocity = calculate_thrust(cur_loc, new_target, finished);

  // if we have just started our movement, modify velocity to account
  // for acceleration. This will help animation be smooth in Unreal.
  move_timer_.stop();
  double move_time = move_timer_.duration_ds();
  if (move_time < 1.0)
  {
    // have acceleration ramp up over a second at .1 per 1/20 second
    for (size_t i = 0; i < xy_velocity.size(); ++i)
    {
      double abs_velocity = fabs(xy_velocity[i]);
      double signed_move_time = xy_velocity[i] < 0 ? -move_time : move_time;
      xy_velocity[i] = move_time < abs_velocity ?
        signed_move_time : xy_velocity[i];
    }

    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_MAJOR,
      "UnrealAgentPlatform::move:" \
      " %s: moving to new target at time %f with modified velocity "
      "[%f,%f,%f]\n",
      self_->agent.prefix.c_str(),
      move_time,
      xy_velocity[0], xy_velocity[1], xy_velocity[2]);

  }

  z_velocity.push_back(xy_velocity[2]);
  xy_velocity.resize(2);

  if (finished)
  {
    madara_logger_ptr_log(gams::loggers::global_logger.get(),
      gams::loggers::LOG_MINOR,
      "UnrealAgentPlatform::move:" \
      " %s: ARRIVED at target \"%f,%f,%f\"\n",
      self_->agent.prefix.c_str(),
      new_target.x(), new_target.y(), new_target.z());

    result = gams::platforms::PLATFORM_ARRIVED;
  }
  
  return result;
}

int
UnrealAgentPlatform::orient(const gams::pose::Orientation & target,
        const gams::pose::OrientationBounds &bounds)
{
  // update variables
  gams::platforms::BasePlatform::orient(target, bounds);

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
    gams::loggers::LOG_TRACE,
    "UnrealAgentPlatform::orient:" \
    " %s: requested target \"%f,%f,%f\"\n",
    self_->agent.prefix.c_str(), target.rx(), target.ry(), target.rz());

  // convert from input reference frame to vrep reference frame, if necessary
  gams::pose::Orientation new_target(get_frame(), target);

  last_orient_ = new_target;

  // we're not changing orientation. this has to be done for move alg to work
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
