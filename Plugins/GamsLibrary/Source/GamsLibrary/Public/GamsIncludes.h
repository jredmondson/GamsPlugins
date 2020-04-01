// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * @file GamsIncludes.h
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * Includes common GAMS and MADARA headers and ensures builds in UE4.
 * Unreal is very picky about warnings, and there are lots due to
 * dependencies. To get around this, this file disables warnings that cause
 * Unreal Build system to fail. There's no way to work with the library
 * otherwise without removing Boost and other dependencies.
 **/

#include "MadaraIncludes.h"

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
#pragma warning(disable:4800)
#pragma warning(disable:4996)
#include "gams/controllers/Multicontroller.h"
#include "gams/controllers/HiveController.h"
#include "gams/controllers/BaseController.h"
#include "gams/platforms/PlatformFactoryRepository.h"
#pragma warning(pop)


