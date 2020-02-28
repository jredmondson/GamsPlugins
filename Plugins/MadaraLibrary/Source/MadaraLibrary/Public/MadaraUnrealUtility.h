#pragma once

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
#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#pragma warning(pop)

namespace madara
{
  namespace utility
  {
    /**
     * Converts a MADARA container to a UE4 rotator
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     **/
    inline void to_rotator(
      const knowledge::containers::NativeDoubleVector& source, FRotator & dest)
    {
      dest.Roll = source[0];
      dest.Pitch = source[1];
      dest.Yaw = source[2];
    }

    /**
     * Converts and multiplies a MADARA container to a UE4 vector.  Useful for
     * position conversions between meters and centimeters.
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     * @param multiple the multiplier to apply to the conversion
     **/
    inline void to_vector_multiply(
      const knowledge::containers::NativeDoubleVector& source,
      FVector& dest, float multiple = 100.0f)
    {
      dest.X = multiple * source[0];
      dest.Y = multiple * source[1];
      dest.Z = multiple * source[2];
    }

    /**
     * Converts a MADARA container to a UE4 vector 
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     **/
    inline void to_vector(
      const knowledge::containers::NativeDoubleVector& source, FVector& dest)
    {
      dest.X = source[0];
      dest.Y = source[1];
      dest.Z = source[2];
    }

    /**
     * Converts a UE4 rotator to a MADARA container
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     **/
    inline void from_rotator(
      const FRotator& source, knowledge::containers::NativeDoubleVector& dest)
    {
      dest.set(2, source.Yaw);
      dest.set(0, source.Roll);
      dest.set(1, source.Pitch);
    }

    /**
     * Converts a UE4 vector to a MADARA container
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     **/
    inline void from_vector(
      const FVector& source, knowledge::containers::NativeDoubleVector& dest)
    {
      dest.set(2, source.Z);
      dest.set(0, source.X);
      dest.set(1, source.Y);
    }

    /**
     * Converts and multiplies a UE4 vector to a MADARA container. Useful for
     * position conversions between meters and centimeters.
     * @param source the source container to take values from
     * @param dest   the unreal vector to store values into
     * @param multiple the multiplier to apply to the conversion
     **/
    inline void from_vector_multiply(
      const FVector& source,
      knowledge::containers::NativeDoubleVector& dest,
      float multiple = 0.01f)
    {
      dest.set(2, source.Z * multiple);
      dest.set(0, source.X * multiple);
      dest.set(1, source.Y * multiple);
    }


    void calculate_delta(
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

      //UE_LOG(LogUnrealAgentPlatform, Log,
      //  TEXT("%s: UnrealAgentPlatform::calculate_delta: total=[%s] to tar=[%s]"
      //    " with speed=%f and time=%f"),
      //  *agent_prefix_, *(total_diff.ToString()), *(local_diff.ToString()),
      //  speed, delta_time);
    }

  }
}
