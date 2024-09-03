#pragma once

#include "starobservationschedulingsolver/flexible_star_observation_scheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace flexible_star_observation_scheduling
{

struct ColumnGenerationOptionalParameters: Parameters
{
    std::string linear_programming_solver = "CLP";
};

struct ColumnGenerationGreedyOutput: Output
{
    ColumnGenerationGreedyOutput(
            const Instance& instance):
        Output(instance) { }


    std::vector<double> solution;

    std::vector<std::vector<double>> x;
};

const ColumnGenerationGreedyOutput column_generation_greedy(
        const Instance& instance,
        const ColumnGenerationOptionalParameters& parameters = {});

}
}
