/**
 * Since 2 pⱼ ≥ dⱼ - rⱼ, targets must be scheduled by non-decreasing order of
 * rⱼ + dⱼ. Therefore, the problem can be solved in pseudo-polynomial time by
 * dynamic programming.
 * w(j, t): maximum profit of a schedule ending before t with targets 1..j.
 * w(0, t) = 0
 * w(j, t) = min w(j - 1, t - pⱼ) - wⱼ  if t ≤ dⱼ
 *               w(j - 1, t)
 *              if j != 0
 */

#pragma once

#include "starobservationschedulingsolver/singlenightstarobservationscheduling/solution.hpp"

namespace starobservationschedulingsolver
{
namespace singlenightstarobservationscheduling
{

struct DynamicProgrammingOptionalParameters: Parameters
{
};

const Output dynamic_programming(
        const Instance& instance,
        const DynamicProgrammingOptionalParameters& parameters = {});

}
}

