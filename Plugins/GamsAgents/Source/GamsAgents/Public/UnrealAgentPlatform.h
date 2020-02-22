
#ifndef   _GAMS_UNREALAGENTPLATFORM_H_
#define   _GAMS_UNREALAGENTPLATFORM_H_

#include "GamsVehicle.h"

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
#include <vector>

#include "madara/knowledge/KnowledgeBase.h"
#include "madara/utility/EpochEnforcer.h"
#include "madara/utility/Timer.h"

#include "gams/variables/Self.h"
#include "gams/variables/Sensor.h"
#include "gams/variables/PlatformStatus.h"
#include "gams/platforms/BasePlatform.h"
#include "gams/platforms/PlatformFactory.h"
#include "gams/pose/CartesianFrame.h"
#pragma warning(pop)
    
  /**
  * A GAMS platform for controlling Unreal agents
  **/
  class UnrealAgentPlatform : public gams::platforms::BasePlatform
  {
  public:

    enum MovementTypes
    {
      MOVEMENT_STOP_ON_ARRIVAL = 0,
      MOVEMENT_PASSTHROUGH = 1
    };

    /**
     * Constructor
     * @param  knowledge  context containing variables and values
     * @param  sensors    map of sensor names to sensor information
     * @param  self       self referencing variables for the agent
     **/
    UnrealAgentPlatform(
      madara::knowledge::KnowledgeBase * knowledge = 0,
      gams::variables::Sensors * sensors = 0,
      gams::variables::Self * self = 0,
      const madara::knowledge::KnowledgeMap & args = {});

    /**
     * Destructor
     **/
    virtual ~UnrealAgentPlatform();

    /**
     * Polls the sensor environment for useful information. Required.
     * @return number of sensors updated/used
     **/
    virtual int sense(void) override;

    /**
     * Analyzes platform information. Required.
     * @return bitmask status of the platform. @see PlatformAnalyzeStatus.
     **/
    virtual int analyze(void) override;

    /**
     * Gets the name of the platform. Required.
     **/
    virtual std::string get_name() const override;

    /**
     * Gets the unique identifier of the platform. This should be an
     * alphanumeric identifier that can be used as part of a MADARA
     * variable(e.g. vrep_ant, autonomous_snake, etc.) Required.
     * @return the id of the platform to use in factory methods
     **/
    virtual std::string get_id() const override;

    /**
     * Gets the position accuracy in meters. Optional.
     * @return position accuracy
     **/
    virtual double get_accuracy(void) const override;

    /**
     * Gets sensor radius. Optional.
     * @return minimum radius of all available sensors for this platform
     **/
    virtual double get_min_sensor_range(void) const override;

    /**
     * Gets move speed. Optional.
     * @return speed in meters per second
     **/
    virtual double get_move_speed(void) const override;

    /**
     * Instructs the agent to return home. Optional.
     * @return the status of the home operation, @see PlatformReturnValues
     **/
    virtual int home(void) override;

    /**
     * Instructs the agent to land. Optional.
     * @return the status of the land operation, @see PlatformReturnValues
     **/
    virtual int land(void) override;

    /**
     * Moves the platform to a location
     * @param   target    the coordinates to move to
     * @param   bounds   approximation value
     * @return the status of the move operation, @see PlatformReturnValues
     **/
    int move(const gams::pose::Position & location,
      const gams::pose::PositionBounds &bounds) override;

    using BasePlatform::move;

    /**
     * Rotates the platform to a specified Orientation
     * @param   target    the coordinates to move to
     * @param   bounds   approximation value, in radians
     * @return the status of the orient operation, @see PlatformReturnValues
     **/
    int orient(const gams::pose::Orientation & location,
    const gams::pose::OrientationBounds &bounds) override;

    using BasePlatform::orient;

    /**
     * Pauses movement, keeps source and dest at current values. Optional.
     **/
    virtual void pause_move(void) override;

    /**
     * Set move speed. Optional.
     * @param speed new speed in meters/second
     **/
    virtual void set_move_speed(const double& speed) override;

    /**
     * Stops movement, resetting source and dest to current location.
     * Optional.
     **/
    virtual void stop_move(void) override;

    /**
     * Instructs the agent to take off. Optional.
     * @return the status of the takeoff, @see PlatformReturnValues
     **/
    virtual int takeoff(void) override;
    
    /**
     * Returns the world reference frame for the platform(e.g. GPS or cartesian)
     **/
    virtual const gams::pose::ReferenceFrame & get_frame(void) const override;
    
  private:

    /**
     * Calculate velocity/thrust necessary to send in OSC
     * @param current  current location
     * @param target   target location
     * @param type     0(try to stop at target), 1(keep on trucking)
     * @return  thrust vector to get from current to target according to
     *          movement profile
     **/
    std::vector<double> calculate_thrust(
      const gams::pose::Position & current, const gams::pose::Position & target,
      bool & finished);

    /// holds whether the agent is known to exist in the simulator
    bool is_created_ = false;

    /// last move target
    gams::pose::Position last_move_;

    /// last orient target
    gams::pose::Orientation last_orient_;

    /// timer for checking last movement
    madara::utility::Timer<madara::utility::Clock> move_timer_;

    /// timer for last calculate thrust call
    madara::utility::Timer<madara::utility::Clock> last_thrust_timer_;

    /// timer for last updated position
    madara::utility::Timer<madara::utility::Clock> last_position_timer_;

    /// actor being manipulated in the world
    AActor * actor_;

    /// world actor to keep track of whether our actor is still valid
    UWorld * world_;

    /// local, non-mutexed copy of self_->prefix
    FString agent_prefix_;

    /// timeout for when to loiter
    double loiter_timeout_;

    /// timeout for calling respawn(-1 means don't respawn)
    double respawn_timeout_;
  }; // end UnrealAgentPlatform class
    

  /**
   * A factory class for creating UnrealAgentPlatform platforms
   **/
  class UnrealAgentPlatformFactory : public gams::platforms::PlatformFactory
  {
  public:

    /**
     * Constructor
     * @param type   the type of robotics system to simulate(quadcopter,
     *               satellite)
     **/
    UnrealAgentPlatformFactory();

    /**
     * Destructor. Shouldn't be necessary but trying to find vtable issue
     **/
    virtual ~UnrealAgentPlatformFactory();

    /**
     * Creates a UnrealAgentPlatform platform.
     * @param   args      no arguments are necessary for this platform
     * @param   knowledge the knowledge base. This will be set by the
     *                    controller in init_vars.
     * @param   sensors   the sensor info. This will be set by the
     *                    controller in init_vars.
     * @param   platforms status inform for all known agents. This
     *                    will be set by the controller in init_vars
     * @param   self      self-referencing variables. This will be
     *                    set by the controller in init_vars
     * @param   type      self-referencing variables. This will be
     *                    set by the controller in init_vars
     **/
    virtual gams::platforms::BasePlatform * create(
      const madara::knowledge::KnowledgeMap & args,
      madara::knowledge::KnowledgeBase * knowledge,
      gams::variables::Sensors * sensors,
      gams::variables::Platforms * platforms,
      gams::variables::Self * self);

  };

#endif // _GAMS_UNREALAGENTPLATFORM_H_
