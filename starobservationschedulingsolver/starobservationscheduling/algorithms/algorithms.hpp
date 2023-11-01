#pragma once

#include "starobservationschedulingsolver/starobservationscheduling/solution.hpp"

#include "starobservationschedulingsolver/starobservationscheduling/algorithms/column_generation.hpp"
#include "starobservationschedulingsolver/starobservationscheduling/algorithms/benders_decomposition.hpp"

#include <random>

namespace starobservationschedulingsolver
{
namespace starobservationscheduling
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}
}

