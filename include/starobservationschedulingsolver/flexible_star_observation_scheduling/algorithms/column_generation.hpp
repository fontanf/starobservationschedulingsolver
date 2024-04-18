/**
 * Dantzig-Wolfe decomposition.
 *
 * The linear programming formulation of the problem based on Dantzig–Wolfe
 * decomposition is written as follows:
 *
 * Variables:
 * - yᵢᵏ ∈ {0, 1} representing a feasible schedule of night i.
 *   yᵢᵏ = 1 iff the corresponding schedule is selected for night i.
 *   xⱼᵢᵏ = 1 iff yᵢᵏ contains target j, otherwise 0.
 *
 * Program:
 *
 * max ∑ᵢ ∑ₖ (∑ⱼ wⱼ xⱼᵢᵏ) yᵢᵏ
 *                                      Note that (∑ⱼ wⱼ xⱼᵢᵏ) is a constant.
 *
 * 0 <= ∑ₖ yᵢᵏ <= 1        for all nights i
 *                         (not more than 1 schedule selected for each night)
 *                                                         Dual variables: uᵢ
 * 0 <= ∑ₖ xⱼᵢᵏ yᵢᵏ <= 1   for all targets j
 *                                        (each target selected at most once)
 *                                                         Dual variables: vⱼ
 *
 * The pricing problem consists in finding a variable of positive reduced cost.
 * The reduced cost of a variable yᵢᵏ is given by:
 * rc(yᵢᵏ) = ∑ⱼ wⱼ xⱼᵢᵏ - uᵢ - ∑ⱼ xⱼᵢᵏ vⱼ
 *         = ∑ⱼ (wⱼ - vⱼ) xⱼᵢᵏ - uᵢ
 *
 * Therefore, finding a variable of maximum reduced cost reduces to solving m
 * Single Night Star Observation Scheduling Problems with targets with profit
 * (wⱼ - vⱼ).
 *
 */

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
