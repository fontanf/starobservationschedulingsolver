#pragma once

#include "starobservationschedulingsolver/flexible_single_night_star_observation_scheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexible_single_night_star_observation_scheduling
{

struct DynamicProgrammingOptionalParameters: Parameters
{
};

const Output dynamic_programming(
        const Instance& instance,
        const DynamicProgrammingOptionalParameters& parameters = {});

}
}
