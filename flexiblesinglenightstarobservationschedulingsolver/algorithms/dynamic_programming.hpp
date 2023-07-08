#pragma once

#include "flexiblesinglenightstarobservationschedulingsolver/solution.hpp"

namespace flexiblesinglenightstarobservationschedulingsolver
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

