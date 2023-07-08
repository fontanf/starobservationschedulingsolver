#pragma once

#include "flexiblestarobservationschedulingsolver/solution.hpp"

#include "flexiblestarobservationschedulingsolver/algorithms/column_generation.hpp"

#include <random>

namespace flexiblestarobservationschedulingsolver
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}

