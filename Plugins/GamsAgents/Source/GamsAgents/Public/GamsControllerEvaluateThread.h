#pragma once

#include "GamsAgentsLogs.h"

#include "GamsIncludes.h"
#include "UnrealAgentPlatform.h"

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

class GamsControllerEvaluateThread : public madara::threads::BaseThread
{
public:
  GamsControllerEvaluateThread(gams::controllers::HiveController& controller,
    TArray<FString>& filecontents)
    : controller_ (&controller), filecontents_(&filecontents)
  {

  }

  /**
   * Runs the thread
   **/
  virtual void run()
  {
    UE_LOG(LogGamsControllerEvaluateThread, Log,
      TEXT("calling controller::run"));

    timer_.start();

    //if (!gams::platforms::global_platform_factory()->exists("unreal_agent"))
    //{
      //UE_LOG(LogGamsControllerEvaluateThread, Log,
      //  TEXT("creating unreal_agent platform alias in global_platform_factory"));

      //// add the dynamic unreal agent platform factory
      //std::vector <std::string> aliases;
      //aliases.resize(3);
      //aliases[0] = "default_agent";
      //aliases[1] = "unreal_agent";
      //aliases[2] = "dynamic_agent";

      //UE_LOG(LogGamsControllerEvaluateThread, Log,
      //  TEXT("adding aliases for agent factory"));

      //gams::platforms::global_platform_factory()->add(aliases, new UnrealAgentPlatformFactory());
    //}

    UE_LOG(LogGamsControllerEvaluateThread, Log,
      TEXT("refreshing controller variables"));

    controller_->refresh_vars();

    UE_LOG(LogGamsControllerEvaluateThread, Log,
      TEXT("evaluating karl files"));

    for (auto contents : *filecontents_)
    {
      controller_->evaluate(TCHAR_TO_UTF8(*contents));
    }

    UE_LOG(LogGamsControllerEvaluateThread, Log,
      TEXT("initializing unreal_agent platform"));

    controller_->init_platform("unreal_agent");
    timer_.stop();

    UE_LOG(LogGamsControllerEvaluateThread, Log,
      TEXT("controller::evaluate took %f seconds"),
      (float)timer_.duration_s());

  }
  
  madara::utility::Timer<std::chrono::steady_clock> timer_;
  gams::controllers::HiveController* controller_;
  TArray<FString> * filecontents_;
};
