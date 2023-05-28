#pragma once

#include "singlenightstarobservationschedulingsolver/solution.hpp"

#include "singlenightstarobservationschedulingsolver/algorithms/dynamic_programming.hpp"

#include <random>

namespace singlenightstarobservationschedulingsolver
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}

