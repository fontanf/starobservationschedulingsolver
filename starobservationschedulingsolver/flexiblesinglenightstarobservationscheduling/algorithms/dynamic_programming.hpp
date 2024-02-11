#pragma once

#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblesinglenightstarobservationscheduling
{

struct DynamicProgrammingOptionalParameters: Parameters
{
};

const Output dynamic_programming(
        const Instance& instance,
        const DynamicProgrammingOptionalParameters& parameters = {});

}
}
