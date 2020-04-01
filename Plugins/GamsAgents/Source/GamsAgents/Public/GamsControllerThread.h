#pragma once

#include "GamsAgentsLogs.h"

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
#include "madara/threads/BaseThread.h"
#include "madara/utility/Timer.h"
#pragma warning(pop)

namespace gams
{
  namespace controllers
  {
    class Multicontroller;
  }
}

/**
* A thread that runs a GAMS controller
**/

class GamsControllerThread : public madara::threads::BaseThread
{
public:
  GamsControllerThread(gams::controllers::HiveController& controller)
    : controller_ (&controller)
  {

  }

  /**
   * Runs the thread
   **/
  virtual void run()
  {
    UE_LOG(LogGamsControllerThread, Log,
      TEXT("calling controller::run_once"));

    timer_.start();
    controller_->run_once();
    timer_.stop();

    UE_LOG(LogGamsControllerThread, Log,
      TEXT("controller::run_once took %f seconds"),
        (float)timer_.duration_s());

  }
  
  madara::utility::Timer<std::chrono::steady_clock> timer_;
  gams::controllers::HiveController* controller_;
};
