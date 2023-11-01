#pragma once

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/solution.hpp"

#include "starobservationschedulingsolver/flexiblestarobservationscheduling/algorithms/column_generation.hpp"

#include <random>

namespace starobservationschedulingsolver
{
namespace flexiblestarobservationscheduling
{

Output run(
        std::string algorithm,
        const Instance& instance,
        const Solution& initial_solution,
        std::mt19937_64& generator,
        optimizationtools::Info info);

}
}
