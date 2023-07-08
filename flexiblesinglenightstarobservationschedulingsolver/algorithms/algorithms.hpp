#pragma once

#include "flexiblesinglenightstarobservationschedulingsolver/solution.hpp"

#include "flexiblesinglenightstarobservationschedulingsolver/algorithms/dynamic_programming.hpp"

#include <random>

namespace flexiblesinglenightstarobservationschedulingsolver
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}

