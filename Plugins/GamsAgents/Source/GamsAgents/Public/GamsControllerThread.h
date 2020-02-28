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
  GamsControllerThread(gams::controllers::Multicontroller& controller)
    : controller_ (&controller)
  {

  }

  /**
   * Runs the thread
   **/
  virtual void run()
  {
    UE_LOG(LogGamsControllerThread, Log,
      TEXT("GamsControllerThread: calling controller::run_once"));

    controller_->run_once();

    UE_LOG(LogGamsControllerThread, Log,
      TEXT("GamsControllerThread: finished controller::run_once"));

  }

  gams::controllers::Multicontroller * controller_;
};
