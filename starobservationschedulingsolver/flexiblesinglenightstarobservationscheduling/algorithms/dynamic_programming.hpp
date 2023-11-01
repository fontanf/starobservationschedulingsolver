#pragma once

#include "starobservationschedulingsolver/flexiblesinglenightstarobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexiblesinglenightstarobservationscheduling
{

struct DynamicProgrammingOptionalParameters
{
    /** Info structure. */
    optimizationtools::Info info = optimizationtools::Info();
};

Output dynamic_programming(
        const Instance& instance,
        DynamicProgrammingOptionalParameters parameters = {});

}
}
