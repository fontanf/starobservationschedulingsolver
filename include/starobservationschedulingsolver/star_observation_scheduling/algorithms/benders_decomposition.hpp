#pragma once

#ifdef XPRESS_FOUND

#include "starobservationschedulingsolver/starobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace starobservationscheduling
{

struct BendersDecompositionOptionalParameters: Parameters
{
};

struct BendersDecompositionGreedyOutput: Output
{
    BendersDecompositionGreedyOutput(
            const Instance& instance):
        Output(instance) { }

    /** Number of iterations. */
    Counter number_of_iterations = 0;
};

const BendersDecompositionGreedyOutput benders_decomposition(
        const Instance& instance,
        const BendersDecompositionOptionalParameters& parameters = {});

}
}

#endif
