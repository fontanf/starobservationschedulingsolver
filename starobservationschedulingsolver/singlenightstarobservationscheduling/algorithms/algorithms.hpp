#pragma once

#include "starobservationschedulingsolver/singlenightstarobservationscheduling/solution.hpp"

#include "starobservationschedulingsolver/singlenightstarobservationscheduling/algorithms/dynamic_programming.hpp"

#include <random>

namespace starobservationschedulingsolver
{
namespace singlenightstarobservationscheduling
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}
}

