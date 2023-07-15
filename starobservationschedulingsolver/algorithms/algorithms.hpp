#pragma once

#include "starobservationschedulingsolver/solution.hpp"

#include "starobservationschedulingsolver/algorithms/column_generation.hpp"
#include "starobservationschedulingsolver/algorithms/benders_decomposition.hpp"

#include <random>

namespace starobservationschedulingsolver
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}

