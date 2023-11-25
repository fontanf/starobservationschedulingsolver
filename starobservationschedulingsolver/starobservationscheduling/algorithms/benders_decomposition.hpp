#pragma once

#include "starobservationschedulingsolver/starobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace starobservationscheduling
{

struct BendersDecompositionOptionalParameters
{
    /** Info structure. */
    optimizationtools::Info info = optimizationtools::Info();
};

struct BendersDecompositionGreedyOutput: Output
{
    BendersDecompositionGreedyOutput(
            const Instance& instance,
            optimizationtools::Info& info):
        Output(instance, info) { }

    /** Number of iterations. */
    Counter number_of_iterations = 0;
};

const BendersDecompositionGreedyOutput benders_decomposition(
        const Instance& instance,
        BendersDecompositionOptionalParameters parameters = {});

}
}

